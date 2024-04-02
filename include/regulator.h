#ifndef REGULATOR_H
#define REGULATOR_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include "PI.h"
#include "PID.h"
#include "comms.h"
#include "refGenMonitor.h"
#include "ModeMonitor.h"

typedef struct Regulator_t
{
    PI_t *pi;
    PID_t *pid;
    ReferenceGenerator_t *refgen;
    ModeMonitor_t *modeMon;
    bool should_run;
    pthread_mutex_t mutex_pi;
    pthread_mutex_t mutex_pid;
} Regulator_t;

void initialize_regulator(Regulator_t *regulator, PI_t *pi, PID_t *pid, ModeMonitor_t *modeMon);
void shutDown(Regulator_t *regulator);
double limit(double v);
void run_regulator(Regulator_t *regulator);

#endif