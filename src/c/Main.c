#include <sys/ipc.h>
#include <sys/shm.h>
#include "../../include/DataMonitor.h"
#include "../../include/ModeMonitor.h"
#include "../../include/PI.h"
#include "../../include/PID.h"
#include "../../include/referenceGenerator.h"
#include "../../include/regulator.h"
#define SHM_PI_SIZE sizeof(struct PI_t)
#define SHM_PID_SIZE sizeof(struct PID_t)
#define SHM_MODE_SIZE sizeof(struct ModeMonitor_t)
#define SHM_REF_SIZE sizeof(struct ModeMonitor_t)
#define SHM_DATA_SIZE sizeof(struct Data_t)
#define SHM_REG_SIZE sizeof(struct Regulator_t)
#define PERMS 0666


int main ()
{
    //mode monitor
    key_t key_mode = ftok("/tmp", 'M');
    int shmid_mode = shmget(key_mode, SHM_MODE_SIZE, IPC_CREAT | PERMS);
    struct ModeMonitor_t* mode_monitor = shmat(shmid_mode, NULL, 0);

    //PI innner controller
    key_t key_PI = ftok("/tmp", 'I');
    int shmid_PI = shmget(key_PI, SHM_PI_SIZE, IPC_CREAT | PERMS);
    struct PI_t* PI = shmat(shmid_PI, NULL, 0);

    //PID outer fontroller
    key_t key_PID = ftok("/tmp", 'D');
    int shmid_PID = shmget(key_PID, SHM_PID_SIZE, IPC_CREAT | PERMS);
    struct PID_t* PId = shmat(shmid_PID, NULL, 0);
    
    //Refgen monitor
    key_t key_refGen = ftok("/tmp", 'R');
    int shmid_refGen = shmget(key_refGen, SHM_REF_SIZE, IPC_CREAT | PERMS);
    struct ReferenceGenerator_t* RefGen = shmat(shmid_refGen, NULL, 0);

    //data monitor
    key_t key_data = ftok("/tmp", 'E');
    int shmid_data = shmget(key_data, SHM_DATA_SIZE, IPC_CREAT | PERMS);
    struct Data_t* data_monitor = shmat(shmid_data, NULL, 0);

    //regulator
    key_t key_reg = ftok("/tmp", 'G');
    int shmid_reg = shmget(key_reg, SHM_REG_SIZE, IPC_CREAT | PERMS);
    struct Regulator_t* regulator = shmat(shmid_reg, NULL, 0);

    
}