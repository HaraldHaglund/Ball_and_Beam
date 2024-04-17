#include "../../include/regulator.h"
#include "../../include/comms.h"
#include "../../include/DataMonitor.h"

#define MAX (10)
#define MIN (-10)

void initialize_regulator(Regulator_t *regulator, PI_t *pi, PID_t *pid, ModeMonitor_t *modeMon)
{
    regulator->pi = pi;
    regulator->pid = pid;
    regulator->modeMon = modeMon;
    regulator->should_run = true;
    pthread_mutex_init(&(regulator->mutex_pi), NULL);
    pthread_mutex_init(&(regulator->mutex_pid), NULL);
}

void set_reference_generator(Regulator_t *regul, ReferenceGenerator_t *refgen)
{
    regul->refgen = refgen;
}

void sendDataToOpCom(double yRef, double y, double u, clock_t *start, Data_t *datamonitor)
{
    clock_t current = clock();

    double t = (double)((current - *start) / (long)CLOCKS_PER_SEC) / 1000;

    putData(&datamonitor, t, yRef, y, u);
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

void* run_regulator(void *arg)
{
    regulator_arguments *args = (regulator_arguments*)arg;
    Regulator_t *regulator = args->regulator;
    Data_t *dataMonitor = args->data_monitor;

    struct moberg *moberg = moberg_new(NULL);
    if (!moberg)
    {
        fprintf(stderr, "NEW failed\n");
        goto out;
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

    long duration;
    clock_t start = clock();

    int previous = getMode(regulator->modeMon);
    int current;

    double yRef = 0;
    double y_position = 0;
    double y_angle = 0;
    double u_1 = 0;
    double u_2 = 0;

    while (regulator->should_run) // Is something similar to interrupted() necessary here?
    {
        switch (current = getMode(regulator->modeMon))
        {
            if (previous != current)
            {
                resetPI(regulator->pi);
                resetPID(regulator->pid);
            }

        case OFF:

            yRef = 0;
            y_position = 0;
            u_2 = 0;

            writeOutput(analogOut_1, u_2, 1);

            break;

        case BEAM:

            readInput(analogInAngle_1, &y_angle, 1);
            yRef = getRef(&refgen);

            pthread_mutex_lock(&(regulator->mutex_pi));

            u_2 = limit(calculateOutputPI(regulator->pi, y_angle, yRef));
            writeOutput(analogOut_1, u_2, 1);
            updateStatePI(regulator->pi, u_2);

            pthread_mutex_unlock(&(regulator->mutex_pi));

            break;

        case BALL:

            readInput(analogInPosition_1, &y_position, 0);
            yRef = getRef(&refgen);

            pthread_mutex_lock(&(regulator->mutex_pid));

            u_1 = calculateOutputPID(regulator->pid, y_position, yRef);

            pthread_mutex_lock(&(regulator->mutex_pi));

            readInput(analogInAngle_1, &y_angle, 1); 
            u_2 = limit(calculateOutputPI(regulator->pi, y_angle, u_1));
            writeOutput(analogOut_1, u_2, 1);
            updateStatePI(regulator->pi, u_2);

            pthread_mutex_lock(&(regulator->mutex_pi));

            updateStatePID(regulator->pid, u_1);

            pthread_mutex_unlock(&(regulator->mutex_pid));
            break;

        default:
            printf("Error: Illegal mode.");
            break;
        }

        previous = getMode(regulator->modeMon);
        sendDataToOpCom(yRef, y_position, u_2, &start, &dataMonitor);

        clock_t end = clock();
        duration = (end - start) / (long)CLOCKS_PER_SEC;
        if (duration > 0)
        {
            sleep(duration);
        }
        else
        {
            printf("Lagging behind...");
        }
    }

    writeOutput(analogOut_1, 0.0, 1);
    pthread_mutex_destroy(&(regulator->mutex_pi));
    pthread_mutex_destroy(&(regulator->mutex_pid));

free:
    moberg_free(moberg);
    return 0;
}

void writeOutput(struct moberg_analog_out out, double u, int port)
{
    if (!moberg_OK(ao0.write(ao0.context, u, &u)))
    {
        fprintf(stderr, "READ failed\n");
        moberg_analog_out_close(moberg, port, out);
    }
}

void readInput(struct moberg_analog_in in, double *value, int port)
{
    if (!moberg_OK(in.read(in.context, value)))
    {
        fprintf(stderr, "READ analog in failed\n");
        moberg_analog_in_close(moberg, port, in);
    }
}
