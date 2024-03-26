#include <stdio.h>
#include <unistd.h>
#include "PIMonitor.h"
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE sizeof(struct PI_monitor)

int main()
{
    key_t key = ftok("/tmp", 'I'); //Gen key for shared memeory segment
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666); //Create shared memory with approriate permission
    struct PI_monitor* ts = shmat(shmid, NULL, 0);

    ts->K = 1;
    ts->Ti = 1;
    ts->Tr = 1;
    ts->Beta = 1;
    ts->H = 1;
    ts->IntegratorOn = 1;

    while(1)
    {
        printf("K in PI: %lf\n", ts->K);
        printf("Ti in PI: %lf\n", ts->Ti);
        printf("Tr in PI: %lf\n", ts->Tr);
        printf("Beta in PI: %lf\n", ts->Beta);
        printf("H in PI: %lf\n", ts->H);
        printf("IntegratorOn in PI: %d\n", ts->IntegratorOn);
        printf("\n");
        sleep(1);
    }
}