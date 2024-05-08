*****Starting Regulation*****
When setting up regulation run make all from the makefile located in /src. This will compile all necessary libraries and executables in the /bin folder. From the /bin folder then make sure receiver.so and Main.out exists. Make sure that the I/O box is started, otherwise the program will not be able to initialize. To start regulator and GUI, execute the Main.out exec.

When you wish to stop regulating and terminate all threads, simply click the stop button in the GUI.This will close GUI and terminate regulation.
*****

*****Troubleshooting*****
shmget invalid argument - If this error is encountered something has gone wrong with the keys for the shared memory. Restarting the machine fixes this issue.
*****

*****Comms*****
When you want to use communication between c and python import comms.h or comms.py from comms and use functions like you normally would.
*****

*****Monitor creation and handling*****
When creating a monitor it should be done by declaring a struct in a .h file. for example:

struct example_struct
{
    volatile double example;
};

in the place where the monitor is to be used, it should be created with:

#include <sys/ipc.h>
#include <sys/shm.h>
#define SHM_SIZE sizeof(example_strucr)

key_t key = ftok("/tmp", 'E'); //Gen key for shared memeory segment, replace "example" with appropriate unique string, needs to be the same as in the place where we want to change it later
int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666); //Create shared memory with appropriate permission
struct example_struct* ex_st = shmat(shmid, NULL, 0);

ex_st can now be used as a normal struct: ex_st->example = ...;

For generating keys use:
'M' for ModeMonitor
'I for PI-monitor(inner controller)
'D' for PID-monitor(outer controller)
'R for monitor in RefGen
'E' for data monitor
'G' for regulator
*****
