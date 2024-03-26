
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
    char* path = "python3 ./comms/putMeassurementData.py";
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
    char* path = "python3 ./comms/putControlData.py";
    double data[] = {t, u};
    return write(path, data, 2);
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
}

/**
 * @brief Starts Regul
 * @return 0 if no error was encountered, otherwise non-0
 */
int startRegul()
{
    char* path = "python3 ./comms/startRegul.py";
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