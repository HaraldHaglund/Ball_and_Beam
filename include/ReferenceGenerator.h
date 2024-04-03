#ifndef REFGENMONIOR_H
#define REFGENMONIOR_H

#include <pthread.h>

//use key 'R' when accesing shared memory
typedef struct ReferenceGenerator_t
{
    //vad ska vara här?
    volatile double ref;
    pthread_mutex_t mutex;

}ReferenceGenerator_t;

#endif