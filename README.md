# README

## Video of the result
- GUI: https://www.youtube.com/watch?v=hf45A2fWMyA&lc=UgwT_Cos2LUpyU3EbNJ4AaABAg
- Ball: https://www.youtube.com/watch?v=Gw0yF0R8S4k&lc=UgxZ6DAxJitDFiHi4_R4AaABAg

## Starting the program

**Setup:**
1. Run `make all` from the makefile located in `/src`. This compiles all necessary libraries and executables in the `/bin` folder.
2. Ensure `receiver.so` and `Main.out` exist in the `/bin` folder.
3. Start the I/O box; otherwise, the program will not initialize properly.
4. Execute `Main.out` to start the regulator and GUI.

**Stopping:**
- To terminate regulation and all associated threads, simply click the stop button in the GUI. This action closes the GUI and stops the regulation.

---

## Using the program

**GUI:**
1. Inner and outer parameters can be set and applied on the left hand side of the UI.You can also choose if you want to control the beam or the ball. 
2. In the middle you can see the plots consisting of reference (black line), output signal (red line) and control signal (blue line). The reference can be adjusted on the right hand side. 
3. There is a slider for the manual setting, along with an amplitude, period, and max ctrl for the square/time optimal reference. 

---

## Troubleshooting

**Issue: `shmget` Invalid Argument**
- If encountered, this error indicates a problem with the keys for the shared memory. Restarting the machine resolves this issue.

---

## Comms

**Usage:**
- To enable communication between C and Python, import `comms.h` or `comms.py` from `comms` and use the functions as needed.

---

## Monitor Creation and Handling

**Creation:**
1. Declare a struct in a `.h` file, for example:

    ```c
    struct example_struct
    {
        volatile double example;
    };
    ```

2. In the place where the monitor is to be used, create it using the following:

    ```c
    #include <sys/ipc.h>
    #include <sys/shm.h>
    #define SHM_SIZE sizeof(example_struct)

    key_t key = ftok("/tmp", 'E'); // Generate key for shared memory segment
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666); // Create shared memory with appropriate permission
    struct example_struct* ex_st = shmat(shmid, NULL, 0);
    ```

**Usage:**
- `ex_st` can now be used as a normal struct: `ex_st->example = ...;`

**Key Generation:**
- Use the following characters for generating keys:
  - `'M'` for ModeMonitor
  - `'I'` for PI-monitor (inner controller)
  - `'D'` for PID-monitor (outer controller)
  - `'R'` for monitor in RefGen
  - `'E'` for data monitor
  - `'G'` for regulator

