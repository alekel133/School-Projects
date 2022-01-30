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

int pop_back(struct List *list, char **ret_data)
{
    struct Node *tmp = NULL;
    if(list == NULL) return 0;
    if(list->head == NULL) return 0;

    tmp = list->tail;
    list->tail = tmp->prev;
    tmp->prev = NULL;
    *ret_data = tmp->data;
    free(tmp);
    
    return 1;
}
 
