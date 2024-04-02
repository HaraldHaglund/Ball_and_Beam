#ifndef PID_H
#define PID_H

#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

//Use key 'D' when accessing shared memory
typedef struct PID_t
{
    volatile double K;
    volatile double Ti;
    volatile double Tr;
    volatile double Td;
    volatile double Beta;
    volatile double H;
    volatile double N;
    volatile bool integratorOn;
    volatile double I;
    volatile double D;
    volatile double v;
    volatile double e;
    volatile double e_old;
    volatile double ad;
    volatile double bd;
    pthread_mutex_t mutex;
} PID_t;

void initialize_pid(PID_t *pid);
void destroy_PID(PID_t *pi);
double calculateOutputPID(PID_t *pid, double y, double yref);
void updateStatePID(PID_t *pid, double u);
long getHMillisPID(PID_t *pid);
void setParametersPID(PID_t *pid, double K, double Ti, double Tr, double Td, double Beta, double H, double N, bool integratorOn);
void resetPID(PID_t *pid);

#endif