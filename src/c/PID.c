#include <stdlib.h>
#include <pthread.h>
#include "../../include/PID.h"

typedef struct PID_t
{
    volatile double K;
    volatile double Ti;
    volatile double Td;
    volatile double Tr;
    volatile double N;
    volatile double Beta;
    volatile double H;
    volatile bool integratorOn;
}PID_t;