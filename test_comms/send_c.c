#include <stdio.h>

int main()
{
    FILE *pipe = popen("python3 recieve_py.py", "w");
    
    fputs("test1\n", pipe);
    fputs("test2\n", pipe);

    pclose(pipe);
}