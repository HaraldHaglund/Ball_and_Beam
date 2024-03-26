*****Monitor creation and handling*****
When creating a monitor it should be done by declaring a struct in a .h file. for example:

struct example_struct
{
    volatile double example;
};

in the place where the monitor is to be used, it should be created with:

key_t key = ftok("/tmp", "example"); //Gen key for shared memeory segment, replace "example" with appropriate unique string, needs to be the same as in the place where we want to change it later
int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666); //Create shared memory with approriate permission
struct example_struct* ex_st = shmat(shmid, NULL, 0);

ex_st can now be used as a normal struct: ex_st->example = ...;

For generating keys use:
'M' for ModeMonitor
'I for PI-monitor(inner controller)
'D' for PID-monitor(outer controller)
'R for monitor in RefGen