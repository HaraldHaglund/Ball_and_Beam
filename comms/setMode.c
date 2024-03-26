#include<stdio.h>
#include "../modeMonitor.h"
#include <sys/ipc.h>
#include <sys/shm.h>

#define buff_size 1024
#define SHM_SIZE sizeof(struct mode_monitor)

int main()
{
    char buffer[buff_size];

    if(fgets(buffer, buff_size, stdin) !=NULL)
    {
        int mode;
        sscanf(buffer, "%d", &mode);

        key_t key = ftok("/tmp", 'M');
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

        struct mode_monitor* mm = shmat(shmid, NULL, 0);


        //need to make sure we have monitor lock here before changing value
        mm->mode = mode;

        if(shmdt(mm) == -1)
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