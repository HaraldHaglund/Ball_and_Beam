#ifndef REFGENMONIOR_H
#define REFGENMONIOR_H
#define MANUAL (0)
#define SQUARE (1)
#define OPTIMAL (2)
#include <stdbool.h>

#include <pthread.h>

//use key 'R' when accesing shared memory
typedef struct ReferenceGenerator_t
{
    volatile double amplitude;
    volatile double period;
    volatile double max_ctrl;
    volatile double sign;
    volatile double ref;
<<<<<<< HEAD
    volatile double uff;
    volatile double phiff;
    volatile double manual;
    volatile int mode;
    bool ampChanged, periodChanged, maxCtrlChanged;
    bool parChanged;
    pthread_mutex_t mutex;
=======
    pthread_mutex_t mutex;

>>>>>>> 84dad792b6258740141c5226b0e9a7945947744d
}ReferenceGenerator_t;

#endif