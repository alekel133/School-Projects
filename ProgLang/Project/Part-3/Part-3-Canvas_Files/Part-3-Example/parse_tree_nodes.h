//*****************************************************************************
// (part 3)
// purpose: node classes used while building a parse tree for
//              the arithmetic expression
// version: Spring 2022
//  author: Joe Crumpton / Ed Swan
//*****************************************************************************

#ifndef PARSE_TREE_NODES_H
#define PARSE_TREE_NODES_H

#include <iostream>
#include <vector>
#include <string>
#include "lexer.h"

using namespace std;

extern bool printDelete;      // shall we print deleting the tree?

// ---------------------------------------------------------------------
// Forward declaration of node types
class ExprNode; 
class TermNode;
class FactorNode;

// ---------------------------------------------------------------------
// <expr> -> <term> {{ (( + || - )) <term> }}
class ExprNode {
public:
  int _level = 0;          // recursion level of this node
  TermNode* firstTerm = nullptr;
  vector<int> restTermOps; // TOK_ADD_OP or TOK_SUB_OP
  vector<TermNode*> restTerms;

  ExprNode(int level);
  ~ExprNode();
};
ostream& operator<<(ostream&, ExprNode&); // Node print operator
// ---------------------------------------------------------------------
// <term> -> <factor> {{ (( * || / )) <factor> }}
class TermNode {
public:
  int _level = 0;              // recursion level of this node
  FactorNode* firstFactor = nullptr;
  vector<int> restFactorOps;   // TOK_MULT_OP or TOK_DIV_OP
  vector<FactorNode*> restFactors;

  TermNode(int level);
  ~TermNode();
};
ostream& operator<<(ostream&, TermNode&); // Node print operator
// ---------------------------------------------------------------------
// Abstract class. Base class for IdNode, IntLitNode, NestedExprNode.
// <factor> -> ID || INTLIT || ( <expr> )
class FactorNode {
public:
  int _level = 0;                        // recursion level of this node

  virtual void printTo(ostream &os) = 0; // pure virtual method, makes the class Abstract
  virtual ~FactorNode();                 // labeling the destructor as virtual allows 
	                                       // the subclass destructors to be called
};
ostream& operator<<(ostream&, FactorNode&); // Node print operator
// ---------------------------------------------------------------------
// class IdNode (Identifier Node)
class IdNode : public FactorNode {
public:
    string* id = nullptr;

    IdNode(int level, string name);
    ~IdNode();
    void printTo(ostream & os);
};
// ---------------------------------------------------------------------
// class IntLitNode (Integer Literal Node)
class IntLitNode : public FactorNode {
public:
    int int_literal = 0;

    IntLitNode(int level, int value);
    ~IntLitNode();
    void printTo(ostream & os);
};
// ---------------------------------------------------------------------
// class NestedExprNode (Nested Expression Node)
class NestedExprNode : public FactorNode {
public:
    ExprNode* exprPtr = nullptr;

    NestedExprNode(int level, ExprNode* en);
    void printTo(ostream & os);
    ~NestedExprNode();
};

#endif /* PARSE_TREE_NODES_H */
