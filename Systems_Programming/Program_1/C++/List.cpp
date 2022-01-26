#include "List.h"

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

