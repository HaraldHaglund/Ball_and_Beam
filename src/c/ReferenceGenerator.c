#include "../../include/ReferenceGenerator.h"
#include <stdio.h>

void initialize_referenceGenerator(ReferenceGenerator_t *refgen)
{
    refgen->amplitude = 5.0;
    refgen->period = 15.0;
    refgen->ref = 0.0;
    refgen->max_ctrl = 0.1;
    refgen->sign = -1.0;
    refgen->uff = 0.0;
    refgen->phiff = 0.0;
    refgen->parChanged = false;
}

void setRef(ReferenceGenerator_t *refgen, double newRef)
{
    pthread_mutex_lock(&(refgen->mutex));
    refgen->ref = newRef;
    pthread_mutex_unlock(&(refgen->mutex));
}

void setUff(ReferenceGenerator_t *refgen, double newUff)
{
    pthread_mutex_lock(&(refgen->mutex));
    refgen->uff = newUff;
    pthread_mutex_unlock(&(refgen->mutex));
}

void setPhiff(ReferenceGenerator_t *refgen, double newPhiff)
{
    pthread_mutex_lock(&(refgen->mutex));
    refgen->phiff = newPhiff;
    pthread_mutex_unlock(&(refgen->mutex));
}

double getRef(ReferenceGenerator_t *refgen)
{
    pthread_mutex_lock(&(refgen->mutex));
    double value = refgen->ref;
    pthread_mutex_unlock(&(refgen->mutex));
    return value;
}

double getUff(ReferenceGenerator_t *refgen)
{
    pthread_mutex_lock(&(refgen->mutex));
    double value = refgen->uff;
    pthread_mutex_unlock(&(refgen->mutex));
    return value;
}

double getPhiff(ReferenceGenerator_t *refgen)
{
    pthread_mutex_lock(&(refgen->mutex));
    double value = refgen->phiff;
    pthread_mutex_unlock(&(refgen->mutex));
    return value;
}
