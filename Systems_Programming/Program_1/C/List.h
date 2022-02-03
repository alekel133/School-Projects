/*
    Author:Alexander Kellough
    MSU NetID: atk133
    Compiler: GCC
    Description:
       Definition for Linked List. 

 */
#ifndef LIST_H
#define LIST_H

#include <malloc.h>
#include <string.h>

// Node definition.
struct Node
{
    char *data;

    struct Node *prev;
    struct Node * next;
};

// List Definition.
struct List
{
    struct Node *head;
    struct Node *tail;
};

// List function declerations.
struct List *makelist();
int push_back(struct List *list, char *new_data);
int pop_back(struct List *list, char *ret_data);

#endif
