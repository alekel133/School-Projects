/*
    Author:Alexander Kellough
    MSU NetID: atk133
    Compiler: GCC
    Description:
       Definition for Linked List functions. 

 */
#include "List.h"

// makelist() decleration. Initializes data members of list struct.
struct List *makelist()
{
    struct List *list = (struct List *) malloc(sizeof(struct List));
    if(list == NULL) return NULL;

    list->head = NULL;
    list->tail = NULL;

    return list;
}

// Push_back decleration. Adds new node to list and stores data passed in.
int push_back(struct List *list, char *new_data) 
{
    if(list == NULL) return 0;

    struct Node *n = (struct Node *) malloc(sizeof(struct Node *));
    n->data = malloc(sizeof(char *));
    strcpy(n->data, new_data);
    if(list->head == NULL)
    {
        list->head = n;
        list->head->next = list->tail;
        list->tail = n;
    }
    else
    {
        n->prev = list->tail;
        list->tail->next = n;
        list->tail = n;
    }
    return 1;
}

// Pop_back decleration. Rremoves node and passes data back out through ret_data parameter.
int pop_back(struct List *list, char *ret_data)
{
    if(list == NULL) return 0;
    if(list->head == NULL) return 0;

    
    ret_data = list->tail->data;
    
    if(list->tail == list->head)
    {
        free(list->head);    
        list->head = NULL;
        list->tail = NULL;
    }
    
    else
    {
        list->tail = list->tail->prev;
        list->tail->next->prev = NULL;
        free(list->tail->next);
        list->tail->next = NULL;
    }


    return 1;
}
 
