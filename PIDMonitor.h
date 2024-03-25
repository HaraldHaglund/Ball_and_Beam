#ifndef PIDMONITOR_H
#define PIDMONITOR_H

//Use key "PID" when accessing shjared memory
struct PID_monitor
{
    volatile double K;
    volatile double Ti;
    volatile double Td;
    volatile double Tr;
    volatile double N;
    volatile double Beta;
    volatile double H;
};

#endif