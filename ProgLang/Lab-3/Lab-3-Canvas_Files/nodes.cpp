//*****************************************************************************
// purpose: DataNode Functions Definitions
//
//  author: Alexander Kellough (atk133)
//*****************************************************************************

#include "nodes.h"

// ---------------------------------------------------------------------
// Do NOT change this method
DataNode::~DataNode() {}
// Uses double dispatch to call the overloaded method printTo in the 
// DataNodes: StringNode, IntegerNode, and FloatNode
//
// Do NOT change this method
ostream& operator<<(ostream& os, DataNode& fn) {
	os << "Data: ";
	fn.printTo(os);
	return os;
}

////////// --- StringNode Functions --- ///////////
StringNode::StringNode(string data)
{
    mystring = new string;
    *mystring = data;
}

StringNode::~StringNode()
{
    cout << "Deleting DataNode:StringNode:" << *mystring << endl;
    mystring = nullptr;
    delete mystring;
}

void StringNode::printTo(ostream& os)
{
    cout << "(string: " << *mystring << ") ";
}

////////// --- OmtegerNode Functions --- ///////////
IntegerNode::IntegerNode(int data)
{
    myinteger = data;
}

IntegerNode::~IntegerNode()
{
    cout << "Deleting DataNode:IntegerNode:" << myinteger << endl;
    myinteger = (int) NULL;
}

void IntegerNode::printTo(ostream&os)
{
    cout << "(integer: " << myinteger << ") ";
}

////////// --- FloatNode Functions --- ///////////
FloatNode::FloatNode(float data)
{
    myfloat = data;
}

FloatNode::~FloatNode()
{
    cout << "Deleting DataNode:FloatNode:" << myfloat << endl;
    myfloat = (float) NULL;
}

void FloatNode::printTo(ostream& os)
{
    cout << "(float: " << myfloat << ") ";
}
