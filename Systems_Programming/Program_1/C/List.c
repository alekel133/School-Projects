#include "List.h"

struct List *makelist()
{
    struct List *list = (struct List *) malloc(sizeof(struct List));
    if(list == NULL) return NULL;

    list->head = NULL;
    list->tail = NULL;

    return list;
}

int push_back(struct List *list, char *new_data) 
{
    if(list == NULL) return 0;

    struct Node *n = (struct Node *) malloc(sizeof(struct Node *));
    n->data = new_data;
    if(list->head == NULL)
    {
        list->head = n;
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

int pop_back(struct List *list, char *ret_data)
{
    if(list == NULL) return 0;
    if(list->head == NULL) return 0;

    struct Node *n = list->tail;
    
    ret_data = n-> data;
    
    if(n == list->head)
    {
        list->head = NULL;    
        list->tail = NULL;
    }
    
    else
    {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
        n->prev = NULL;
    }

    free(n);

    return 1;
}
 
