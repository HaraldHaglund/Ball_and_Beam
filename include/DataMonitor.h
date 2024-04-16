#ifndef DATAMONITOR_H
#define DATAMONITOR_H

#include <pthread.h>

typedef struct Data_t
{
    volatile double t;
    volatile double yRef;
    volatile double y;
    volatile double u;
    pthread_mutex_t mutex;

} Data_t;

int putData(Data_t *md, double t, double yRef, double y, double u);
int getMeasurementData(Data_t *md, double *t, double *yRef, double *y);
int getControlData(Data_t *md, double *t, double *u);
void initialize_DataMonitor(Data_t *md);

#endif