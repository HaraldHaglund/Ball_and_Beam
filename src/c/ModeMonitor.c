#include "../../include/ModeMonitor.h"

void initialize_ModeMonitor(ModeMonitor_t *modeMon)
{
    modeMon->mode = OFF;
    pthread_mutex_init(&(modeMon->mutex), NULL);
}

void setMode(ModeMonitor_t *modeMon, int mode)
{
    pthread_mutex_lock(&(modeMon->mutex));
    modeMon->mode = mode;
    pthread_mutex_unlock(&(modeMon->mutex));
}

int getMode(ModeMonitor_t *modeMon)
{
    pthread_mutex_lock(&(modeMon->mutex));
    int mode = modeMon->mode;
    pthread_mutex_unlock(&(modeMon->mutex));
    return mode;
}