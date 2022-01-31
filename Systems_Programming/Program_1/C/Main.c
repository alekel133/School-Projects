#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "List.h"

int main()
{
    char string[256] = {0};
    struct List *list = makelist();

    printf("Enter strings to be reversed. Enter [STOP] when finished:\n");
    
    while(strcmp( string, "STOP") != 0)
    {
        scanf("%s", string);
        push_back(list, string);
    }

    printf("Reversed strings:\n");

    while(pop_back(list, string) != 0)
    {
        printf("%s\n", string);
    }

    exit(EXIT_SUCCESS);
}
