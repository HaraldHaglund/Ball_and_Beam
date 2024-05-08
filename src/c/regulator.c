#include "../../include/regulator.h"
#include "../../include/comms.h"
#include "../../include/DataMonitor.h"
#include <time.h>
#include <stdio.h>

#define MAX (10)
#define MIN (-10)

void initialize_regulator(Regulator_t *regulator, PI_t *pi, PID_t *pid, ModeMonitor_t *modeMon, ReferenceGenerator_t *refGen)
{
    regulator->pi = pi;
    regulator->pid = pid;
    regulator->refGen = refGen;
    regulator->modeMon = modeMon;
    regulator->should_run = true;
    pthread_mutex_init(&(regulator->mutex_pi), NULL);
    pthread_mutex_init(&(regulator->mutex_pid), NULL);
}

void sendDataToOpCom(double yRef, double y, double u, struct timespec *start, Data_t *datamonitor)
{
    struct timespec current;
    clock_gettime(CLOCK_MONOTONIC_RAW, &current);
    int t_us = (current.tv_sec - start->tv_sec) * 1000000 + (current.tv_nsec - start->tv_nsec) / 1000;
    double t = (double)t_us / 1000000;
    putData(datamonitor, t, yRef, y, u);
}

// move this functionality to receiver.c so we can call it from the GUI
void shutDown(Regulator_t *regulator)
{
    regulator->should_run = false;
}

double limit(double v)
{
    if (v < MIN)
        v = MIN;
    else if (v > MAX)
        v = MAX;
    return v;
}

void *run_regulator(void *arg)
{
  int prio = 1;

  pthread_setschedprio(pthread_self(), prio);
    regulator_arguments *args = (regulator_arguments *)arg;
    Regulator_t *regulator = args->regulator;
    Data_t *dataMonitor = args->data_monitor;

    struct moberg *moberg = moberg_new(NULL);
    if (!moberg)
    {
        fprintf(stderr, "NEW failed\n");
    }

    struct moberg_analog_out analogOut_1;
    struct moberg_analog_in analogInAngle_1;
    struct moberg_analog_in analogInPosition_0;

    if (!moberg_OK(moberg_analog_out_open(moberg, 1, &analogOut_1)))
    {
        fprintf(stderr, "OPEN analog out failed\n");
        goto free;
    }

    if (!moberg_OK(moberg_analog_in_open(moberg, 1, &analogInAngle_1)))
    {
        fprintf(stderr, "OPEN analog in angle failed\n");
        goto free;
    }

    if (!moberg_OK(moberg_analog_in_open(moberg, 0, &analogInPosition_0)))
    {
        fprintf(stderr, "OPEN analog position failed\n");
        goto free;
    }

    double duration;
    int current = getMode(regulator->modeMon);
    int previous = current;

    struct timespec start_time, end_time, start_time_abs;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_time);
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_time_abs);

    double yRef = 0;
    double y_position = 0;
    double y_angle = 0;
    double u_1 = 0;
    double u_2 = 0;

    while (regulator->should_run) // Is something similar to interrupted() necessary here?
    {

        if (previous != current)
        {
            resetPI(regulator->pi);
            resetPID(regulator->pid);
        }

        switch (current)
        {

        case OFF:

            yRef = 0;
            y_position = 0;
            u_2 = 0;

            break;

        case BEAM:

            readInput(analogInAngle_1, &y_angle, 1, moberg);
            yRef = getRef(regulator->refGen);

            pthread_mutex_lock(&(regulator->mutex_pi));

            u_2 = limit(calculateOutputPI(regulator->pi, y_angle, yRef));
            writeOutput(analogOut_1, u_2, 1, moberg);
            updateStatePI(regulator->pi, u_2);

            pthread_mutex_unlock(&(regulator->mutex_pi));
            sendDataToOpCom(yRef, y_angle, u_2, &start_time_abs, dataMonitor);

            break;

        case BALL:

            readInput(analogInPosition_0, &y_position, 0, moberg);
            yRef = getRef(regulator->refGen);

            pthread_mutex_lock(&(regulator->mutex_pid));

            u_1 = calculateOutputPID(regulator->pid, y_position, yRef) + getPhiff(regulator->refGen);

            pthread_mutex_lock(&(regulator->mutex_pi));

            readInput(analogInAngle_1, &y_angle, 1, moberg);
            u_2 = limit(calculateOutputPI(regulator->pi, y_angle, u_1) + getUff(regulator->refGen));
            writeOutput(analogOut_1, u_2, 1, moberg);
            updateStatePI(regulator->pi, u_2 - getUff(regulator->refGen));

            pthread_mutex_unlock(&(regulator->mutex_pi));

            updateStatePID(regulator->pid, u_1 - getPhiff(regulator->refGen));

            pthread_mutex_unlock(&(regulator->mutex_pid));
	    sendDataToOpCom(yRef, y_position, u_2, &start_time_abs, dataMonitor);
            break;

        default:
            printf("Error: Illegal mode.");
            break;
        }

        previous = current;

        current = getMode(regulator->modeMon);

        clock_gettime(CLOCK_MONOTONIC_RAW, &end_time);

        int tot_time = (end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_nsec - start_time.tv_nsec) / 1000;

        duration = getHPID(regulator->pid) * 1000000 - tot_time;

        if (duration > 0)
        {
            usleep(duration);
        }
        else
        {
            printf("Lagging behind...");
        }

        start_time.tv_nsec += getHPID(regulator->pid) * 1000000000;
    }

    writeOutput(analogOut_1, 0.0, 1, moberg);
    //pthread_mutex_destroy(&(regulator->mutex_pi));
    //pthread_mutex_destroy(&(regulator->mutex_pid));

free:
    printf("Freeing in regulator\n");
    writeOutput(analogOut_1, 0.0, 1, moberg);
    moberg_free(moberg);
    destroy_PID(regulator->pid);
    destroy_PI(regulator->pi);
    destroy_ModeMonitor(regulator->modeMon);
    destroy_ReferenceGenerator(regulator->refGen);
    destroy_DataMonitor(dataMonitor);
    return 0;
}

void writeOutput(struct moberg_analog_out out, double u, int port, struct moberg *moberg)
{
  // printf("writeOutput: %f\n", u);
    if (!moberg_OK(out.write(out.context, u, &u)))
    {
        fprintf(stderr, "WRITE failed\n");
        moberg_analog_out_close(moberg, port, out);
    }
}

void readInput(struct moberg_analog_in in, double *value, int port, struct moberg *moberg)
{
    if (!moberg_OK(in.read(in.context, value)))
    {
        fprintf(stderr, "READ analog in failed\n");
        moberg_analog_in_close(moberg, port, in);
    }
}
