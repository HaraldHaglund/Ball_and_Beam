#include "../../include/ReferenceGenerator.h"
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void initialize_ReferenceGenerator(ReferenceGenerator_t *refgen, int shmid)
{
    refgen->amplitude = 5.0;
    refgen->period = 15.0;
    refgen->ref = 0.0;
    refgen->max_ctrl = 0.1;
    refgen->sign = -1.0;
    refgen->uff = 0.0;
    refgen->phiff = 0.0;
    refgen->parChanged = false;
    pthread_mutex_init(&(refgen->mutex), NULL);
    refgen->shmid = shmid;
}

void destroy_ReferenceGenerator(ReferenceGenerator_t *refgen)
{
    pthread_mutex_destroy(&(refgen->mutex));
    int shmid = refgen->shmid;

    if(shmdt(refgen) == -1) {
      perror("shmdt refGen");
    }

    if(shmctl(shmid, IPC_RMID, NULL) == -1) {
      perror("shmcl refGen");
    }

    printf("Reference Generator destroyed\n");
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
