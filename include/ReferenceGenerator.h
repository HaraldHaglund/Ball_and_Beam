#ifndef REFGENMONIOR_H
#define REFGENMONIOR_H
#define MANUAL (0)
#define SQUARE (1)
#define OPTIMAL (2)
#include <stdbool.h>

//use key 'R' when accesing shared memory
typedef struct ReferenceGenerator_t
{
    volatile double amplitude;
    volatile double period;
    volatile double max_ctrl;
    volatile double sign;
    volatile double ref;
    volatile double uff;
    volatile double phiff;
    volatile double manual;
    volatile int mode;
    bool ampChanged, periodChanged, maxCtrlChanged;
    bool parChanged;
    pthread_mutex_t mutex;
}ReferenceGenerator_t;

#endif