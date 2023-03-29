/*
     Author: Alexander Kellough
     NetID: atk133
     Compoiler: gcc (GCC) 12.2.1 20230201
     Description:
          Function declerations for Command_Vector.
*/
#pragma once
#ifndef COMMAND_VECTOR_H
#define COMMAND_VECTOR_H

typedef struct commandVector
{
    int init;
    int size;
    int fill;
    char ** arr;
} commandVecotr;

void init(struct commandVector *cVec);

int resize(struct commandVector *cVec);

int append(struct commandVector *target, char *val);

int depend(struct commandVector *target);

int clear(struct commandVector *target);

int find(struct commandVector *target, char *val);

void tokenize(struct commandVector* target, char *source, char *delim);

void display(struct commandVector *target);

void deconstruct(struct commandVector *target);

#endif 