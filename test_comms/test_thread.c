#include <stdio.h>
#include <unistd.h>
#include "test_struct.h"
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE sizeof(struct test_struct)

int main()
{
    key_t key = ftok("/tmp", 'A'); //Gen key for shared memeory segment
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666); //Create shared memory with approriate permission
    struct test_struct* ts = shmat(shmid, NULL, 0);

    ts->ref = 1;

    while(1)
    {
        printf("ref in struct : %lf\n", ts->ref);
        sleep(1);
    }
}