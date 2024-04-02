#ifndef REGULATOR_H
#define REGULATOR_H

#include <stdlib.h>
#include <stdbool.h>
#include "PI.h"
#include "PID.h"
#include "comms.h"
#include "refGenMonitor.h"
#include "modeMonitor.h"

typedef struct Regul_t
{
    PI_t pi;
    PID_t pid;
    refGen_monitor refgen;
    mode_monitor modeMon;
    bool should_run;
    long start_time;
} Regul_t;

#endif