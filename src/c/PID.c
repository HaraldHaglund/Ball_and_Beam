#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "../../include/PID.h"

void initialize_PID(PID_t *pid, int shmid)
{
    pid->K = -0.12;
    pid->Ti = 30.5;
    pid->Tr = 5.0;
    pid->Td = 1.3;
    pid->Beta = 1.0;
    pid->H = 0.05;
    pid->N = 10.0;
    pid->integratorOn = false;
    pid->I = 0.0;
    pid->D = 0.0;
    pid->v = 0.0;
    pid->e = 0.0;
    pid->y_old = 0.0;
    pid->e_old = 0;
    pthread_mutex_init(&(pid->mutex), NULL); // free this memory!
    pid->ad = pid->Td / (pid->Td + pid->N * pid->H);
    pid->bd = pid->K * pid->ad * pid->N;
    pid->shmid = shmid;
}

void destroy_PID(PID_t *pid)
{
    pthread_mutex_destroy(&(pid->mutex));
    int shmid = pid->shmid;
    
    if(shmdt(pid) == -1){
      perror("shmdt PID");
    }

    if(shmctl(shmid, IPC_RMID, NULL) == -1) {
      perror("shctl PID");
    }

    printf("PID destroyed\n");
    
}

double calculateOutputPID(PID_t *pid, double y, double yref)
{
    pthread_mutex_lock(&(pid->mutex));
    pid->e = yref - y;
    
    //for usage without feed forward
    //pid->D = pid->ad * pid->D - (pid->bd * (y - pid->y_old));

    //for usage with feed forrward
    pid->D = pid->ad * pid->D + (pid->bd * (pid->e - pid->e_old));
    
    pid->v = pid->K * ((pid->Beta * yref) - y) + pid->I + pid->D;
        pid->y_old = y;
	pid->e_old = pid->e;
        double v = pid->v;
    pthread_mutex_unlock(&(pid->mutex));
    return v;
}

void updateStatePID(PID_t *pid, double u)
{
    pthread_mutex_lock(&(pid->mutex));
    if (pid->integratorOn) {
      pid->I = pid->I + ((pid->K * pid->H / pid->Ti) * pid->e) + (pid->H / pid->Tr) * (u - pid->v);
        } else {
            pid->I = 0.0;
        }
    pthread_mutex_unlock(&(pid->mutex));
}

double getHPID(PID_t *pid)
{
    pthread_mutex_lock(&(pid->mutex));
    double h = pid->H;
    pthread_mutex_unlock(&(pid->mutex));
    return h;
}

void setParametersPID(PID_t *pid, double K, double Ti, double Tr, double Td, double Beta, double H, double N, bool integratorOn)
{
    pthread_mutex_lock(&(pid->mutex));
    pid->K = K;
    pid->Ti = Ti;
    pid->Tr = Tr;
    pid->Td = Td;
    pid->Beta = Beta;
    pid->H = H;
    pid->N = N;
    pid->integratorOn = integratorOn;
    if (!pid->integratorOn)
    {
        pid->I = 0.0;
    }
    pid->ad = pid->Td / (pid->Td + pid->N * pid->H);
    pid->bd = pid->K * pid->ad * pid->N;
    pthread_mutex_unlock(&(pid->mutex));
}

void getParametersPID(PID_t *pid, double *K, double *Ti, double *Td, double *Tr, double *N, double *Beta, double *H, int *integratorOn)
{
    pthread_mutex_lock(&(pid->mutex));

    *K = pid->K;
    *Ti = pid->Ti;
    *Td = pid->Td;
    *Tr = pid->Tr;
    *N = pid->N;
    *Beta = pid->Beta;
    *H = pid->H;
    *integratorOn = (int) pid->integratorOn;

    pthread_mutex_unlock(&(pid->mutex));
}

void resetPID(PID_t *pid)
{
    pthread_mutex_lock(&(pid->mutex));
    pid->I = 0.0;
    pthread_mutex_unlock(&(pid->mutex));
}
