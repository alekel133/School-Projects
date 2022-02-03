/*
    Author:Alexander Kellough
    MSU NetID: atk133
    Compiler: GCC
    Description:
        Takes input and stores it in linked list.

 */
#include <stdlib.h>
#include <stdio.h>
#include "List.h"

int main()
{
    char *string = malloc(sizeof(char *));
    int size = 1;
    int i;
    struct List *list = makelist();

    printf("Enter strings to be reversed. Enter [STOP] when finished:\n");
   
   // Input Loop 
    while(strcmp( string, "STOP") != 0)
    {
        scanf("%s", string);
        push_back(list, string);
    }
    
    printf("Reversed strings:\n");

    // Output Loop
    while(pop_back(list, string)!=0)
    {
        printf("%s\n", string);
    
    }

    exit(EXIT_SUCCESS);
}
