#include<stdio.h>
#include "../test_struct.h" //change to Refgen struct later
#include <sys/ipc.h>
#include <sys/shm.h>

#define buff_size 1024
#define SHM_SIZE sizeof(struct temp_struct)

int main()
{
    char buffer[buff_size];

    if(fgets(buffer, buff_size, stdin) !=NULL)
    {
        double d;
        sscanf(buffer, "%lf", &d);

        key_t key = ftok("/tmp", "RefGen");

        int shmid = shmget(key, SHM_SIZE, 0666);

        struct temp_struct* ts = shmat(shmid, NULL, 0);


        //need to make sure we have monitor lock here before changing value
        ts->ref = d;

        shmdt(ts);
    }
}