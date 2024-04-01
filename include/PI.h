#ifndef PI_H
#define PI_H

#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>

//use key 'I' when accessing shared memory
typedef struct PI_t;
void initialize_PI(PI_t *pi, double K, double Ti, double Tr, double Beta, double H, bool IntegratorOn, double I, double v, double e);

#endif