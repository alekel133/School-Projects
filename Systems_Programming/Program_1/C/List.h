#ifndef LIST_H
#define LIST_H

#include <malloc.h>
#include <string.h>

struct Node
{
    char *data;

    struct Node *prev;
    struct Node * next;
};

struct List
{
    struct Node *head;
    struct Node *tail;
};

struct List *makelist();
int push_back(struct List *list, char *new_data);
int pop_back(struct List *list, char *ret_data);

#endif
