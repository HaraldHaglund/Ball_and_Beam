#include <sys/ipc.h>
#include <sys/shm.h>
#include "../../include/DataMonitor.h"
#include "../../include/modeMonitor.h"
#include "../../include/PI.h"
#include "../../include/PID.h"
#include "../../include/ReferenceGenerator.h"
#include "../../include/regulator.h"
#include "../../include/comms.h"

#define SHM_PI_SIZE sizeof(struct PI_t)
#define SHM_PID_SIZE sizeof(struct PID_t)
#define SHM_MODE_SIZE sizeof(struct ModeMonitor_t)
#define SHM_REF_SIZE sizeof(struct ModeMonitor_t)
#define SHM_DATA_SIZE sizeof(struct Data_t)
#define SHM_REG_SIZE sizeof(struct Regulator_t)
#define PERMS 0666

int main()
{
    // mode monitor
    key_t key_mode = ftok("/tmp", 'M');
    if (key_mode == -1)
    {
        perror("Error, ftok:");
        return 1;
    }

    int shmid_mode = shmget(key_mode, SHM_MODE_SIZE, IPC_CREAT | PERMS);
    if (shmid_mode == -1)
    {
        perror("Error, shmget: ");
        return 1;
    }
    struct ModeMonitor_t *mode_monitor = shmat(shmid_mode, NULL, 0);

    // PI innner controller
    key_t key_PI = ftok("/tmp", 'I');
    if (key_PI == -1)
    {
        perror("Error, ftok:");
        return 1;
    }
    int shmid_PI = shmget(key_PI, SHM_PI_SIZE, IPC_CREAT | PERMS);
    if (shmid_PI == -1)
    {
        perror("Error, shmget: ");
        return 1;
    }
    struct PI_t *PI = shmat(shmid_PI, NULL, 0);

    // PID outer fontroller
    key_t key_PID = ftok("/tmp", 'D');
    if (key_PID == -1)
    {
        perror("Error, ftok:");
        return 1;
    }
    int shmid_PID = shmget(key_PID, SHM_PID_SIZE, IPC_CREAT | PERMS);
    if (shmid_PID == -1)
    {
        perror("Error, shmget: ");
        return 1;
    }
    struct PID_t *PID = shmat(shmid_PID, NULL, 0);

    // Refgen monitor
    key_t key_refGen = ftok("/tmp", 'R');
    if (key_refGen == -1)
    {
        perror("Error, ftok:");
        return 1;
    }
    int shmid_refGen = shmget(key_refGen, SHM_REF_SIZE, IPC_CREAT | PERMS);
    if (shmid_refGen == -1)
    {
        perror("Error, shmget: ");
        return 1;
    }
    struct ReferenceGenerator_t *refGen = shmat(shmid_refGen, NULL, 0);

    // data monitor
    key_t key_data = ftok("/tmp", 'E');
    if (key_data == -1)
    {
        perror("Error, ftok:");
        return 1;
    }
    int shmid_data = shmget(key_data, SHM_DATA_SIZE, IPC_CREAT | PERMS);
    if (shmid_data == -1)
    {
        perror("Error, shmget: ");
        return 1;
    }
    struct Data_t *data_monitor = shmat(shmid_data, NULL, 0);

    // regulator
    key_t key_reg = ftok("/tmp", 'G');
    if (key_reg == -1)
    {
        perror("Error, ftok:");
        return 1;
    }
    int shmid_reg = shmget(key_reg, SHM_REG_SIZE, IPC_CREAT | PERMS);
    if (shmid_reg == -1)
    {
        perror("Error, shmget: ");
        return 1;
    }
    struct Regulator_t *regulator = shmat(shmid_reg, NULL, 0);

    // initialization of all structs
    initialize_ModeMonitor(mode_monitor);
    initialize_PI(PI);
    initialize_PID(PID);
    initialize_referenceGenerator(refGen);
    initialize_DataMonitor(data_monitor);
    initialize_regulator(regulator, PI, PID, mode_monitor, refGen);

    // start threads

    // start regul thread, see the run_regulator function and the regulator argument struct in regulator.h
    // for understanding how to start a "run"-function.

    pthread_t regulator_thread;
    regulator_arguments args;

    args.regulator = regulator;
    args.data_monitor = data_monitor;

    if (pthread_create(&regulator_thread, NULL, run_regulator, (void*)&args) != 0)
    {
        perror("Error starting regulator thread");
        return 1;
    }

    // start GUI Need to start a separate thread that runs this otrherwise main gets stuck here.
    if (startOpcom() != 0)
    {
        return -1;
    }

    // done
    if (pthread_join(regulator_thread, NULL) != 0)
    {
        perror("Error joining regulator thread");
        return 1;
    }

    return 0;
}
