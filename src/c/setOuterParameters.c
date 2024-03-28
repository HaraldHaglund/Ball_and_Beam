//sets parameters of the outer controller
#include<stdio.h>
#include "../../include/PIDMonitor.h"
#include <sys/ipc.h>
#include <sys/shm.h>

#define buff_size 1024
#define SHM_SIZE sizeof(struct PID_monitor)

int main()
{
    char buffer[buff_size];

    if(fgets(buffer, buff_size, stdin) !=NULL)
    {
        double K, Ti, Td, Tr, N, Beta, H;
        int integratorOn;
        sscanf(buffer, "%lf %lf %d %lf %lf %lf %lf %lf", &Beta, &H, &integratorOn, &K, &N, &Td, &Ti, &Tr);

        key_t key = ftok("/tmp", 'D');
        if(key == -1)
        {
            perror("Error, ftok:");
            return 1;
        }

        int shmid = shmget(key, SHM_SIZE, 0666);
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
    else
    {
        fprintf(stderr, "Error: Failed to read input from stdin.\n");
        return 1;
    }
}