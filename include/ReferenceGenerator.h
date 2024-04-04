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
    volatile double uff;
    volatile double phiff;
    volatile double manual;
    volatile int mode;
    bool ampChanged, periodChanged, maxCtrlChanged;
    bool parChanged;
    pthread_mutex_t mutex;
}ReferenceGenerator_t;

void initialize_referenceGenerator(ReferenceGenerator_t *refgen);
void setRef(ReferenceGenerator_t *refgen, double newRef);
void setParChanged(ReferenceGenerator_t *refgen);
bool getParChanged(ReferenceGenerator_t *refgen);
void setManual(ReferenceGenerator_t *refgen, double newManual);
void setSqMode(ReferenceGenerator_t *refgen);
void setManMode(ReferenceGenerator_t *refgen);
void setOptMode(ReferenceGenerator_t *refgen);
double getRef(ReferenceGenerator_t *refgen);
double getUff(ReferenceGenerator_t *refgen);
double getPhiff(ReferenceGenerator_t *refgen);

#endif