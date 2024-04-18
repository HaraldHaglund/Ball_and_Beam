#include <stdio.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include "../../include/PI.h"
#include "../../include/PID.h"
#include "../../include/modeMonitor.h"
#include "../../include/ReferenceGenerator.h"
#include "../../include/DataMonitor.h"
#include "../../include/regulator.h"

#define SHM_PI_SIZE sizeof(struct PI_t)
#define SHM_PID_SIZE sizeof(struct PID_t)
#define SHM_MODE_SIZE sizeof(struct ModeMonitor_t)
#define SHM_REF_SIZE sizeof(struct ModeMonitor_t)
#define SHM_DATA_SIZE sizeof(struct Data_t)
#define SHM_REG_SIZE sizeof(struct Regulator_t)
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

    struct PI_t* PI = shmat(shmid, NULL, 0);

    setParametersPI(PI, K, Ti, Tr, Beta, H, (bool)integratorOn);

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

    struct PI_t* PI = shmat(shmid, NULL, 0);

    getParametersPI(PI, K, Ti, Tr, Beta, H, integratorOn);

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

    struct PID_t* PID = shmat(shmid, NULL, 0);

    setParametersPID(PID, K, Ti, Tr, Td, Beta, H, N, (bool)integratorOn);

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

    struct PID_t* PID = shmat(shmid, NULL, 0);

    getParametersPID(PID, K, Ti, Td, Tr, N, Beta, H, integratorOn);

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
int setModePy(int mode)
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

    struct ModeMonitor_t* mm = shmat(shmid, NULL, 0);
    setMode(mm, mode);

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
int setRefPy(double ref)
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

    struct ReferenceGenerator_t* rgm = shmat(shmid, NULL, 0);
    setRef(rgm, ref);

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
int getControlDataPy(double *x, double *u)
{
    key_t key = ftok("/tmp", 'E');
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

    struct Data_t* dm = shmat(shmid, NULL, 0);
    getControlData(dm, x, u);

    if(shmdt(dm) == -1)
    {
        perror("Error, shmdt: ");
        return 1;
    }
    return 0;
}

/**
 * @brief Gets latest measurement data. Should only be called by python script.
 * @param x: pointer to double to get value of x
 * @param yRef: pointer to double to get value of yRef
 * @param y: pointer to double to get value of y
 * @return 0 if no error was encountered, otherwise non-0
 */
int getMeasurementDataPy(double *x, double *yRef, double *y)
{
    key_t key = ftok("/tmp", 'E');
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

    struct Data_t* dm = shmat(shmid, NULL, 0);
    getMeasurementData(dm, x, yRef, y);

    if(shmdt(dm) == -1)
    {
        perror("Error, shmdt: ");
        return 1;
    }
    return 0;
}

/**
 * @brief shuts down regulation by calling shutdown in the regulator
 * @return 0 if no error was encountered, otherwise non-0
 */
int shutDownPy()
{
    key_t key = ftok("/tmp", 'G');
    if(key == -1)
    {
        perror("Error, ftok:");
        return 1;
    }

    int shmid = shmget(key, SHM_REG_SIZE, PERMS);
    if(shmid == -1)
    {
        perror("Error, shmget: ");
        return 1;
    }

    struct Regulator_t* reg = shmat(shmid, NULL, 0);
    shutDown(reg);

    if(shmdt(reg) == -1)
    {
        perror("Error, shmdt: ");
        return 1;
    }
    return 0;
}

//logical entry point, is not used for anything ever
int main()
{
    return 0;
}