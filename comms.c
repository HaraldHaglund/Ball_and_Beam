
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
    char* path = "./comms/script_to_be_run";
    double data[] = {t, yRef, y};
    return write(path, data, 3);
}

/**
 * @brief Writes control data from the c regulator to the python GUI
 * @param t: t
 * @param u: u
 * @return 0 if no error was encountered, otherwise non-0
 */
int putControlData(double t, double u)
{
    char* path = "./comms/script_to_be_run";
    double data[] = {t, u};
    return write(path, data, 2);
}

//idk if this should be implemented here
int startOpcom()
{
    char* path = "./comms/script_to_be_run";
    double* data;
    return write(path, data, 0);
}

//idk if this should be implemented here or not
int startRegul()
{
    char* path = "./comms/script_to_be_run";
    double* data;
    return write(path, data, 0);
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