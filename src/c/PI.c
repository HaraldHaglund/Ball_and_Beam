#include <stdlib.h>
#include <pthread.h>
#include "../../include/PI.h"

typedef struct PI_t
{
    volatile double K;
    volatile double Ti;
    volatile double Tr;
    volatile double Beta;
    volatile double H;
    volatile bool IntegratorOn;
    volatile double I;
    volatile double v;
    volatile double e;
} PI_t;

void initialize_PI(PI_t *pi, double K, double Ti, double Tr, double Beta, double H, bool IntegratorOn, double I, double v, double e)
{
    pi->K = K;
    pi->Ti = Ti;
    pi->Tr = Tr;
    pi->Beta = Beta;
    pi->H = H;
    pi->IntegratorOn = IntegratorOn;
    pi->I = I;
    pi->v = v;
    pi->e = e;
}
