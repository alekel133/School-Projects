//*****************************************************************************
// purpose:  DataNode Class and declerations
//
//  author: Alexander Kellough (atk133)
//*****************************************************************************

#ifndef NODES_H
#define NODES_H

#include <iostream>
#include <string>

using namespace std;

// Abstract class. Base class for StringNode, IntegerNode, and FloatNode
//
// Do NOT change this class
class DataNode {
public:
    virtual void printTo(ostream &os) = 0; // pure virtual method, makes the class Abstract
    virtual ~DataNode();                   // labeling the destructor as virtual allows 
	                                       // the subclass destructors to be called
};
ostream& operator<<(ostream&, DataNode&);  // print method
// ---------------------------------------------------------------------
class StringNode : public DataNode {
public:
    string* mystring = nullptr;

    // Add constructor, destructor, and printTo methods

    StringNode(string data);

    ~StringNode();

    void printTo(ostream& os);
};
// ---------------------------------------------------------------------
class IntegerNode : public DataNode {
public:
    int myinteger = 0;

    // Add constructor, destructor, and printTo methods
    
    IntegerNode(int data);

    ~IntegerNode();

    void printTo(ostream& os);
};
// ---------------------------------------------------------------------
class FloatNode : public DataNode {
public:
    float myfloat = 0.0;

    // Add constructor, destructor, and printTo methods
    FloatNode(float data);

    ~FloatNode();

    void printTo(ostream& os);
};

#endif /* NODES_H */
