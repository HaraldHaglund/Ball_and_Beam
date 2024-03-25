//set parameters of the inner controller
#include<stdio.h>
#include "../PIMonitor.h"
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
        sscanf(buffer, "%lf", &K);
        sscanf(buffer, "%lf", &Ti);
        sscanf(buffer, "%lf", &Tr);
        sscanf(buffer, "%lf", &Beta);
        sscanf(buffer, "%lf", &H);

        key_t key = ftok("/tmp", "PI");
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