#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int *age = malloc(sizeof(int));
    *age = 30;

    return 0;
}