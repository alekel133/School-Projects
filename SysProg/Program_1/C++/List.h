#include "Node.h"
#include <iostream>
#pragma once

class List{
private:
    Node* head;
    Node* tail;

public:
    List(): head(nullptr), tail(nullptr){}

    ~List();

    void insert(std::string data);

    bool reverse(std::ostream &out);
};
