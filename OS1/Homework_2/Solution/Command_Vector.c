/*
     Author: Alexander Kellough
     NetID: atk133
     Compoiler: gcc (GCC) 12.2.1 20230201
     Description:
          Function definitions for Command_Vector.h
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Command_Vector.h"

// Initialize Command Vector
void init(struct commandVector *cVec)
{
    cVec->init = 1;
    cVec->size = 64;
    cVec->fill = 0;    ;
    cVec->arr = (char **) malloc(cVec->size*sizeof(char*)); 
}

// Dynamically allocat more space if needed
int resize(struct commandVector *cVec)
{
    int newSize = 2*cVec->size;

    char ** tmp = cVec->arr;

    if(!(cVec->arr = (char **) malloc(2 * newSize * sizeof(char*))))
        return 0;

    cVec->size = newSize;

    for(int i = 0; i < cVec->fill; ++ i)
        cVec->arr[i] = tmp[i];

    return 1;
}

// Check if initialized, if so add new value. If value can't fit, resize.
int append(struct commandVector *target, char * val)
{
    if(target->init != 1)
        init(target);
    
    if(target->fill < target->size)
    {
        ++target->fill;
        target->arr[target->fill-1] = val;
        target->arr[target->fill] = (char *) NULL;
        return 1;
    }

    if(!(resize(target)))
        return 0;

    return append(target, val);
}

// Remove last value.
int depend(struct commandVector *target)
{
    if(target->fill == 0)
        return 0;

    --target->fill;
    target->arr[target->fill] = (char *) NULL; 
    return 1;
}

// Clear array, deallocate, and reinitialize.
int clear(struct commandVector *target)
{
    free(target->arr);
    target->arr = NULL;
    init(target);
    return 1;
}

// Find a string in the array.
int find(struct commandVector *target, char *val)
{
    for(int i = 0; i < target->fill; ++i)
    {
        if(strcmp(target->arr[i], val) == 0)
            return i;
    }
    return 0;
}

// Tokenize source string based on delim, and place it in target.`
void tokenize(struct commandVector* target, char *source, char *delim) 
{
    char *token;
    token = strtok(source, delim);

    while(token != NULL)
    {
        append(target, token);
        token = strtok(NULL, delim);
    }
}

// Display string contents as a command to stderr.
void display(struct commandVector *target)
{
    for(int i = 0; i < target->fill; ++i)
    {
        fprintf(stderr,"%s ",target->arr[i]);   
    }
    fprintf(stderr,"\n");
}

// Free Allocated memory.
void deconstruct(struct commandVector *target)
{
    free(target->arr);
}