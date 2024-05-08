#include "../../include/modeMonitor.h"
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void initialize_ModeMonitor(ModeMonitor_t *modeMon, int shmid)
{
    modeMon->mode = OFF;
    modeMon->shmid = shmid;
    pthread_mutex_init(&(modeMon->mutex), NULL);
}

void destroy_ModeMonitor(ModeMonitor_t *modeMon)
{
    pthread_mutex_destroy(&(modeMon->mutex));
    int shmid = modeMon->shmid;

    if(shmdt(modeMon) == -1) {
      perror("shmdt ModeMonitor");
    }

    if(shmctl(shmid, IPC_RMID, NULL) == -1) {
      perror("shmcl ModeMonitor");
    }

    printf("Mode Monitor destroyed\n");
}

int setMode(ModeMonitor_t *modeMon, int mode)
{
    pthread_mutex_lock(&(modeMon->mutex));
    
    modeMon->mode = mode;
    pthread_mutex_unlock(&(modeMon->mutex));
    return mode;
}

int getMode(ModeMonitor_t *modeMon)
{
    pthread_mutex_lock(&(modeMon->mutex));
    int mode = modeMon->mode;
    pthread_mutex_unlock(&(modeMon->mutex));
    return mode;
}
