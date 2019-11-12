#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int x = strlen(argv[1]);

// TODO: fix this overflow please
    if(argc + x < argc )
        printf("smaller");
    else
        printf("bigger");

    return 0;
}