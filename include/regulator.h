#ifndef REGULATOR_H
#define REGULATOR_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <moberg.h>
#include "PI.h"
#include "PID.h"
#include "comms.h"
#include "ReferenceGenerator.h"
#include "modeMonitor.h"
#include "DataMonitor.h"

typedef struct Regulator_t
{
    PI_t *pi;
    PID_t *pid;
    ReferenceGenerator_t *refGen;
    ModeMonitor_t *modeMon;
    bool should_run;
    pthread_mutex_t mutex_pi;
    pthread_mutex_t mutex_pid;
} Regulator_t;

typedef struct
{
    Regulator_t *regulator;
    Data_t *data_monitor;
} regulator_arguments;

void initialize_regulator(Regulator_t *regulator, PI_t *pi, PID_t *pid, ModeMonitor_t *modeMon, ReferenceGenerator_t *refGen);
void sendDataToOpCom(double yRef, double y, double u, clock_t *start, Data_t *datamonitor);
void shutDown(Regulator_t *regulator);
double limit(double v);
void *run_regulator(void *arg);
void writeOutput(struct moberg_analog_out out, double u, int port, struct moberg *moberg);
void readInput(struct moberg_analog_in in, double *value, int port, struct moberg *moberg);

#endif
