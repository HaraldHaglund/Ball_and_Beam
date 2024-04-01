#ifndef PID_H
#define PID_H

#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

//Use key 'D' when accessing shared memory
typedef struct PID_t;

void initialize_pid(PID_t *pid);
void destroy_PID(PID_t *pi);
double calculateOutput(PID_t *pid, double y, double yref);
void updateState(PID_t *pid, double u);
long getHMillis(PID_t *pid);
void setParameters(PID_t *pid, double K, double Ti, double Tr, double Td, double Beta, double H, double N, bool integratorOn);
void reset(PID_t *pid);

#endif