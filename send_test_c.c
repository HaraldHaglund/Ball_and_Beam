#include <stdio.h>

int main()
{
    FILE *pipe = popen("./recieve_test.out", "w");
    
    fputs("test1\n", pipe);
    fputs("test2\n", pipe);

    pclose(pipe);
}