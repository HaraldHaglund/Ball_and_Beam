#ifndef MODEMONITOR_H
#define MODEMONITOR_H
#define OFF (0)
#define BEAM (1)
#define BALL (2)

#include <stdlib.h>
#include <pthread.h>

// use key 'M' when accesing shared memory
typedef struct ModeMonitor_t
{
    // OFF = 0, BEAM = 1, BALL = 2
    volatile int mode;
    pthread_mutex_t mutex;

} ModeMonitor_t;


void initialize_ModeMonitor(ModeMonitor_t *modeMon);
int setMode(ModeMonitor_t *modeMon, int mode);
int getMode(ModeMonitor_t *modeMon);
#endif