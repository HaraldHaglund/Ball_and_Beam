
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Starts Opcom
 * @return 0 if no error was encountered, otherwise non-0
 */
int startOpcom()
{
    char* path = "python3 ./../python/GUI.py"; //change to path of strting the opcom
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