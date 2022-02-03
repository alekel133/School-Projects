/*
    Author: Alexander Kellough
    MSU NetID: atk133
    Compiler: GCC
    Description:
        Node class decleration. 
 */
#include <string>
#pragma once

class Node{
public:
    std::string data;
    Node* prev; 
    Node* next;

    Node(): data(0), prev(nullptr), next(nullptr){}

    Node(std::string data): data(data), prev(nullptr), next(nullptr){}

    Node(std::string data, Node* prev, Node* next): data(data), prev(prev), next(next){}
};
