#include <stdio.h>
#include <stdlib.h>

#define buff_size 1024

int main ()
{
    char buffer[buff_size];

    while(fgets(buffer, buff_size, stdin) != NULL)
        printf("recieved: %s", buffer);
    return 0;
}