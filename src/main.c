#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    if (argc != 2) return 1;

    if (strcmp(argv[1], "remind") == 0)
    {
        printf("reminder set\n");
    }

    return 0;
}
