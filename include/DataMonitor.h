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

//ignore t for our implementation
int putData(Data_t *md, double t, double yRef, double y, double u)
{
    pthread_mutex_lock(&(md->mutex));
    md->t = t;
    md->yRef = yRef;
    md->y = y;
    md->u = u;
    pthread_mutex_unlock(&(md->mutex));
    return 0;
}

int getMeasurementData(Data_t *md, double *t, double *yRef, double *y)
{
    pthread_mutex_lock(&(md->mutex));
    *t = md->t;
    *yRef = md->yRef;
    *y = md->y;
    pthread_mutex_unlock(&(md->mutex));
    return 0;
}

int getControlData(Data_t *md, double *t, double *u)
{
    pthread_mutex_lock(&(md->mutex));
    *t = md->t;
    *u = md->u;
    pthread_mutex_unlock(&(md->mutex));
    return 0;
}

void initialize_DataMonitor(Data_t *md)
{
    md->t = 0;
    md->y = 0;
    md->yRef = 0;
    md->u = 0;
    pthread_mutex_init(&(md->mutex), NULL);
}

#endif