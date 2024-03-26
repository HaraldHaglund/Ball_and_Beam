#ifndef PIMONITOR_H
#define PIMONITOR_H

#include <stdbool.h>

//use key 'I' when accessing shared memory
struct PI_monitor
{
    volatile double K;
    volatile double Ti;
    volatile double Tr;
    volatile double Beta;
    volatile double H;
    volatile bool IntegratorOn;
};

#endif