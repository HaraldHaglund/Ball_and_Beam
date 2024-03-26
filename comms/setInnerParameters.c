//set parameters of the inner controller
#include <stdio.h>
#include <stdbool.h>
#include "../monitors/PIMonitor.h"
#include <sys/ipc.h>
#include <sys/shm.h>

#define buff_size 1024
#define SHM_SIZE sizeof(struct PI_monitor)

int main()
{
    char buffer[buff_size];

    if(fgets(buffer, buff_size, stdin) !=NULL)
    {
        double K, Ti, Td, Tr, N, Beta, H;
        int integratorOn;
        sscanf(buffer, "%lf %lf %d %lf %lf %lf", &Beta, &H, &integratorOn, &K, &Ti, &Tr);

        key_t key = ftok("/tmp", 'I');
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
    else
    {
        fprintf(stderr, "Error: Failed to read input from stdin.\n");
        return 1;
    }
}