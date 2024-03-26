#include<stdio.h>
#include "../../../include/refGenMonitor.h"
#include <sys/ipc.h>
#include <sys/shm.h>

#define buff_size 1024
#define SHM_SIZE sizeof(struct refGen_monitor)

int main()
{
    char buffer[buff_size];

    if(fgets(buffer, buff_size, stdin) !=NULL)
    {
        double ref;
        sscanf(buffer, "%lf", &ref);

        key_t key = ftok("/tmp", 'R');
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

        struct refGen_monitor* rgm = shmat(shmid, NULL, 0);


        //need to make sure we have monitor lock here before changing value
        rgm->ref = ref;

        if(shmdt(rgm) == -1)
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