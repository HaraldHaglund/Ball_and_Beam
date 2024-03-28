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

/**
 * @brief Sets inner parameters of controller. Should only be called by python script
 * @param K: double value K
 * @param Ti: double value Ti
 * @param Tr: double value Tr
 * @param Beta: double value Beta
 * @param H: double value H
 * @param integratorOn: int value IntegratorOn. 1 - on, 0- off
 * @return 0 if no error was encountered, otherwise non-0
 */
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
    return 0;
}

/**
 * @brief Gets inner parameters of controller. Should only be called by python script
 * @param K: pointer to double to get value of K
 * @param Ti: pointer to double to get value of Ti
 * @param Tr: pointer to double to get value of Tr
 * @param Beta: pointer to double to get value of Beta
 * @param H: pointer to double to get value of H
 * @param integratorOn: pointer to int to get value of integratorOn. 1 - on, 0- off
 * @return 0 if no error was encountered, otherwise non-0
 */
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
    return 0;
}

/**
 * @brief Sets outer parameters of controller. Should only be called by python script
 * @param K: double value K
 * @param Ti: double value Ti
 * @param Td: double value Td
 * @param Tr: double value Tr
 * @param N: double value N
 * @param Beta: double value Beta
 * @param H: double value H
 * @param integratorOn: int value IntegratorOn. 1 - on, 0- off
 * @return 0 if no error was encountered, otherwise non-0
 */
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
    return 0;
}

/**
 * @brief Gets outer parameters of controller. Should only be called by python script
 * @param K: pointer to double to get value of K
 * @param Ti: pointer to double to get value of Ti
 * @param Td: pointer to double to get value of Td
 * @param Tr: pointer to double to get value of Tr
 * @param N: pointer to double to get value of N
 * @param Beta: pointer to double to get value of Beta
 * @param H: pointer to double to get value of H
 * @param integratorOn: pointer to int to get value of integratorOn. 1 - on, 0- off
 * @return 0 if no error was encountered, otherwise non-0
 */
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
    return 0;
}

/**
 * @brief Sets mode of controller. 0 = OFF, 1 = BEAM, 2 = BALL. Should only be called by python script
 * @param mode: int indication mode to set to. 0 = OFF, 1 = BEAM, 2 = BALL
 * @return 0 if no error was encountered, otherwise non-0
 */
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
    return 0;
}

/**
 * @brief Sets reference value. Should only be called by python script
 * @param ref: double value of new reference
 * @return 0 if no error was encountered, otherwise non-0
 */
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
    return 0;
}

/**
 * @brief Gets latest control data. Should only be called by python script
 * @param x: pointer to double to get value of x
 * @param u: pointer to double to get value of u
 * @return 0 if no error was encountered, otherwise non-0
 */
int getControlData(double *x, double *u)
{
    *x = 1;//get value from somewhere shared memory
    *u = 1;//get value from somewhere shared memory
    return 0;
}

/**
 * @brief Gets latest measurement data. Should only be called by python script.
 * @param x: pointer to double to get value of x
 * @param yRef: pointer to double to get value of yRef
 * @param y: pointer to double to get value of y
 * @return 0 if no error was encountered, otherwise non-0
 */
int getMeasurementData(double *x, double *yRef, double *y)
{
    *x = 1;//get value from somewhere shared memory
    *yRef = 1;//get value from somewhere shared memory
    *y = 1;//get value from somewhere shared memory
    return 0;
}

/**
 * @brief shuts down regulation. Does nothing yet
 * @return 0 if no error was encountered, otherwise non-0
 */
int shutDown()
{
    //shutdown
    return 0;
}