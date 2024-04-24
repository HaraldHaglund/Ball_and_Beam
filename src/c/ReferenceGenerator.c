#include "../../include/ReferenceGenerator.h"
#include <stdio.h>

void initialize_referenceGenerator(ReferenceGenerator_t *refgen) {
        refgen->amplitude = 5.0;
        refgen->period = 15.0; 
        refgen->manual = 0.0;
        refgen->ref = 0.0;
        refgen->max_ctrl = 0.1;
        refgen->sign = -1.0;
        refgen->uff = 0.0;
        refgen->phiff = 0.0;
        refgen->parChanged = false;
        refgen->mode = MANUAL;
    }

   void setRef(ReferenceGenerator_t *refgen, double newRef) {
        pthread_mutex_lock(&(refgen->mutex));
	printf("setRef: %f\n", newRef);
        refgen->ref = newRef;
	printf("set setRef: %f\n", refgen->ref);
        pthread_mutex_unlock(&(refgen->mutex));
    }

    void setParChanged(ReferenceGenerator_t *refgen) {
        pthread_mutex_lock(&(refgen->mutex));
        refgen->parChanged = true;
        pthread_mutex_unlock(&(refgen->mutex));
    }

    bool getParChanged(ReferenceGenerator_t *refgen) {
        pthread_mutex_lock(&(refgen->mutex));
        bool val = refgen->parChanged;
        refgen->parChanged = false;
        pthread_mutex_unlock(&(refgen->mutex));
        return val;
    }


    void setManual(ReferenceGenerator_t *refgen, double newManual) {
        pthread_mutex_lock(&(refgen->mutex));
        refgen->manual = newManual;
        pthread_mutex_unlock(&(refgen->mutex));
    }

    void setSqMode(ReferenceGenerator_t *refgen) {
        pthread_mutex_lock(&(refgen->mutex));
        refgen->mode = SQUARE;
        pthread_mutex_unlock(&(refgen->mutex));
    }

    void setManMode(ReferenceGenerator_t *refgen) {
        pthread_mutex_lock(&(refgen->mutex));
        refgen->mode = MANUAL;
        pthread_mutex_unlock(&(refgen->mutex));
    }

    void setOptMode(ReferenceGenerator_t *refgen) {
        pthread_mutex_lock(&(refgen->mutex));
        refgen->mode = OPTIMAL;
        pthread_mutex_unlock(&(refgen->mutex));
    }

    double getRef(ReferenceGenerator_t *refgen) 
    {
        pthread_mutex_lock(&(refgen->mutex));
        double value = refgen->ref; //(refgen->mode == MANUAL) ? refgen->manual : refgen->ref;
        pthread_mutex_unlock(&(refgen->mutex));
        return value;
    }

    double getUff(ReferenceGenerator_t *refgen) 
    {
        pthread_mutex_lock(&(refgen->mutex));
        double value = (refgen->mode == OPTIMAL) ? refgen->uff : 0.0;
        pthread_mutex_unlock(&(refgen->mutex));
        return value;
    }

    double getPhiff(ReferenceGenerator_t *refgen) 
    {
        pthread_mutex_lock(&(refgen->mutex));
        double value = (refgen->mode == OPTIMAL) ? refgen->phiff : 0.0;
        pthread_mutex_unlock(&(refgen->mutex));
        return value;
    }
