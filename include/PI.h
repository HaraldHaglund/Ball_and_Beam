#ifndef PI_H
#define PI_H

#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

//use key 'I' when accessing shared memory
typedef struct PI_t
{
    volatile double K;
    volatile double Ti;
    volatile double Tr;
    volatile double Beta;
    volatile double H;
    volatile bool integratorOn;
    volatile double I;
    volatile double v;
    volatile double e;
    pthread_mutex_t mutex;
} PI_t;

void initialize_PI(PI_t *pi);
void destroy_PI(PI_t *pi);
double calculateOutputPI(PI_t *pi, double y, double yref);
void updateStatePI(PI_t *pi, double u);
double getHPI(PI_t *pi);
void setParametersPI(PI_t *pi, double K, double Ti, double Tr, double Beta, double H, bool integratorOn);
void getParametersPI(PI_t *pi, double *K, double *Ti, double *Tr, double *Beta, double *H, int *integratorOn);
void resetPI(PI_t *pi);

#endif