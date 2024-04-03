
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Writes measurement data from the c regulator to the python GUI
 * @param t: t
 * @param yRef: yRef
 * @param y: y
 * @return 0 if no error was encountered, otherwise non-0
 */
int putMeassurementData(double t, double yRef, double y)
{
    //need to make a streuct and put data in there
}

/**
 * @brief Writes control data from the c regulator to the python GUI
 * @param t: t
 * @param u: u
 * @return 0 if no error was encountered, otherwise non-0
 */
int putControlData(double t, double u)
{
    //Need to make a struct and put datat in there
}

/**
 * @brief Starts Opcom
 * @return 0 if no error was encountered, otherwise non-0
 */
int startOpcom()
{
    char* path = "python3 ./comms/startOpcom.py";
    double* data;
    return write(path, data, 0);
    //start the opcom, integrate later
}


/**
 * @brief *Do not call*
 * @param path: path to pipe data to
 * @param data: array of data to send
 * @param nbr_data: number of data pieces to send
 * @return 0 if no error was encountered, otherwise non-0
 */
int write(char* path, double data[], int nbr_data)
{
    FILE *pipe = popen(path, "w");
    if(pipe == NULL)
    {
        perror("Error, popen: ");
        return -1;
    }

    for(int i = 0; i < nbr_data; ++i)
    {
        fprintf(pipe, "%lf", data[i]);
    }

    if(pclose(pipe) == -1)
    {
        perror("Error, pclose: ");
        return -1;
    }

    return 0;
}