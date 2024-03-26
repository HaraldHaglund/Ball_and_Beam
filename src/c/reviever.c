#include <stdio.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "../../include/PIMonitor.h"
#include "../../include/PIDMonitor.h"
#include "../../include/modeMonitor.h"
#include "../../include/refGenMonitor.h"

#define SHM_PI_SIZE sizeof(struct PI_monitor)
#define SHM_PID_SIZE sizeof(struct PID_monitor)
#define SHM_MODE_SIZE sizeof(struct mode_monitor)
#define SHM_REF_SIZE sizeof(struct refGen_monitor)
#define PERMS 0666

int setInnerParameters(double K, double Ti, double Tr, double Beta, double H, int integratorOn)
{
    key_t key = ftok("/tmp", 'I');
    if(key == -1)
    {
        perror("Error, ftok:");
        return 1;
    }

    int shmid = shmget(key, SHM_PI_SIZE, PERMS);
    if(shmid == -1)
    {
        perror("Error, shmget: ");
        return 1;
    }

    struct PI_monitor* PI = shmat(shmid, NULL, 0);
    //need to make sure we have monitor lock here before changing value
    PI->K = K;
    PI->Ti = Ti;
    PI->Tr = Tr;
    PI->Beta = Beta;
    PI->H = H;
    PI->IntegratorOn = (bool) integratorOn;

    if(shmdt(PI) == -1)
    {
        perror("Error, shmdt: ");
        return 1;
    }
}

int getInnerParameters(double *K, double *Ti, double *Tr, double *Beta, double *H, int *integratorOn)
{
    key_t key = ftok("/tmp", 'I');
    if(key == -1)
    {
        perror("Error, ftok:");
        return 1;
    }

    int shmid = shmget(key, SHM_PI_SIZE, PERMS);
    if(shmid == -1)
    {
        perror("Error, shmget: ");
        return 1;
    }

    struct PI_monitor* PI = shmat(shmid, NULL, 0);
    //need to make sure we have monitor lock
    *K = PI->K;
    *Ti = PI->Ti;
    *Tr = PI->Tr;
    *Beta = PI->Beta;
    *H = PI->H;
    *integratorOn = PI->IntegratorOn;

    if(shmdt(PI) == -1)
    {
        perror("Error, shmdt: ");
        return 1;
    }
}

int setOuterParameters(double K, double Ti, double Td, double Tr, double N, double Beta, double H, int integratorOn)
{
    key_t key = ftok("/tmp", 'D');
    if(key == -1)
    {
        perror("Error, ftok:");
        return 1;
    }

    int shmid = shmget(key, SHM_PID_SIZE, PERMS);
    if(shmid == -1)
    {
        perror("Error, shmget: ");
        return 1;
    }

    struct PID_monitor* PID = shmat(shmid, NULL, 0);

    //need to make sure we have monitor lock here before changing value
    PID->K = K;
    PID->Ti = Ti;
    PID->Td = Td;
    PID->Tr = Tr;
    PID->N = N;
    PID->Beta = Beta;
    PID->H = H;
    PID->integratorOn = (bool) integratorOn;

    if(shmdt(PID) == -1)
    {
        perror("Error, shmdt: ");
        return 1;
    }
}

int getOuterParameters(double *K, double *Ti, double *Td, double *Tr, double *N, double *Beta, double *H, int *integratorOn)
{
    key_t key = ftok("/tmp", 'D');
    if(key == -1)
    {
        perror("Error, ftok:");
        return 1;
    }

    int shmid = shmget(key, SHM_PID_SIZE, PERMS);
    if(shmid == -1)
    {
        perror("Error, shmget: ");
        return 1;
    }

    struct PID_monitor* PID = shmat(shmid, NULL, 0);
    //need to make sure we have monitor lock
    *K = PID->K;
    *Ti = PID->Ti;
    *Td = PID->Td;
    *Tr = PID->Tr;
    *N = PID->N;
    *Beta = PID->Beta;
    *H = PID->H;
    *integratorOn = PID->integratorOn;

    if(shmdt(PID) == -1)
    {
        perror("Error, shmdt: ");
        return 1;
    }
}

int setMode(int mode)
{

    key_t key = ftok("/tmp", 'M');
    if(key == -1)
    {
        perror("Error, ftok:");
        return 1;
    }

    int shmid = shmget(key, SHM_MODE_SIZE, PERMS);
    if(shmid == -1)
    {
        perror("Error, shmget: ");
        return 1;
    }

    struct mode_monitor* mm = shmat(shmid, NULL, 0);


    //need to make sure we have monitor lock here before changing value
    mm->mode = mode;

    if(shmdt(mm) == -1)
    {
        perror("Error, shmdt: ");
        return 1;
    }
}

int setRef(double ref)
{
    key_t key = ftok("/tmp", 'R');
    if(key == -1)
    {
        perror("Error, ftok:");
        return 1;
    }

    int shmid = shmget(key, SHM_REF_SIZE, PERMS);
    if(shmid == -1)
    {
        perror("Error, shmget: ");
        return 1;
    }

    struct refGen_monitor* rgm = shmat(shmid, NULL, 0);


    //need to make sure we have monitor lock here before changing value
    rgm->ref = ref;

    if(shmdt(rgm) == -1)
    {
        perror("Error, shmdt: ");
        return 1;
    }
}

int getControlData(double *x, double *u)
{
    *x = 1;//get value from somewhere shared memory
    *u = 1;//get value from somewhere shared memory
}

int getMeasurementData(double *x, double *yRef, double *y)
{
    *x = 1;//get value from somewhere shared memory
    *yRef = 1;//get value from somewhere shared memory
    *y = 1;//get value from somewhere shared memory
}