/*
    Author: Alexander Kellough
    MSU NetID: atk133
    Compiler: GCC
    Description:
        List function definitions.

 */
#include "List.h"

// List destructor
List::~List()
{
    Node* tmp = tail;

    while(tmp->prev != nullptr)
    {
        tmp = tmp->prev;
        tmp->next->prev = nullptr;
        tmp->next = nullptr;
    }
}

// Insert fucntion, adds new node with provided data.
void List::insert(std::string data) 
{
    Node* tmp = new Node(data);

    tmp->prev = tail;

    if(head == nullptr)
    {
       head = tmp;
    }
    else
    {
        tail->next = tmp;
    }
    tail = tmp;
}

// Prints list in reverse
bool List::reverse(std::ostream &os)
{
    if(head == nullptr)
        return false;

    Node* tmp = tail;

    while(tmp != nullptr)
    {
        os << tmp->data << std::endl;
        tmp = tmp->prev;
    }
    return true;
}

