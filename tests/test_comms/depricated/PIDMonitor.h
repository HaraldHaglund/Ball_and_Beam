#ifndef PIDMONITOR_H
#define PIDMONITOR_H

#include <stdbool.h>

//Use key 'D' when accessing shared memory
struct PID_monitor
{
    volatile double K;
    volatile double Ti;
    volatile double Td;
    volatile double Tr;
    volatile double N;
    volatile double Beta;
    volatile double H;
    volatile bool integratorOn;
};

#endif