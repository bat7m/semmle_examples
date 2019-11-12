#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int x = strlen(argv[1]);

    if(argc + x < x )
        printf("smaller");
    else
        printf("bigger");

    return 0;
}