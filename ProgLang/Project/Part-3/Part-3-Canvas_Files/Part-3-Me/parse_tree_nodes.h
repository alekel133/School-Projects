/********************************************************************************
Author: Alexander Kellough       NetID:atk133
Title: Project Part 3 
Purpose:
   Class and Function Definitions for Parse Tree Generation
 ********************************************************************************/

#ifndef PARSE_TREE_NODES_H
#define PARSE_TREE_NODES_H

#include <iostream>
#include <vector>
#include <string>
#include "lexer.h"

using namespace std;

extern bool printDelete;

class PrgoramNode;
class BlockkNode;
class StatementNode;
class ExprNode;
class SimpleExprNode;
class TermNode;
class FactorNode;

class ExprNode {
public:
   int _level = 0;
   SimpleExprNode* firstSimpleExpr = nullptr;
   vector<int> restSimpleExprOps;
   vector<SimpleExprNode*> restSimpleExpr;

   ExprNode(int level);
   ~ExprNode();
};
ostream& operator<<(ostream&, ExprNode&);

class SimpleExprNode {
public:
   int _level = 0;
   TermNode* firstTerm = nullptr;
   vector<int> restTermOps;
   vector<TermNode*> restTerms;

   SimpleExprNode(int _level);
   ~SimpleExprNode();
};
ostream& operator<<(ostream&, SimpleExprNode&);

class TermNode {
public:
   int _level;
   FactorNode* firstFactor = nullptr;
   vector<int> restFactorOps;
   vector<FactorNode*> restFactors;

   TermNode(int level);
   ~TermNode();
};
ostream& operator<<(ostream&, TermNode&);

class FactorNode {
 public:
    int _level = 0;                        // recursion level of this node

    virtual void printTo(ostream &os) = 0; // pure virtual method, makes the class Abstract
    virtual ~FactorNode();                 // labeling the destructor as virtual allows
                                           // the subclass destructors to be called
};                            
ostream& operator<<(ostream&, FactorNode&); // Node print operator
                                        
class IntLitNode : public FactorNode {
public:
   int int_literal = 0;

   IntLitNode(int level, int value);
   ~IntLitNode();
   void printTo(ostream & os);
};

class FloatLitNode : public FactorNode{
public:
   float float_literal = 0;

   FloatLitNode(int level, float value);
   ~FloatLitNode();
   void printTo(ostream & os);
};

class IdentNode : public FactorNode{
public:
   string* id = nullptr;

   IdentNode(int level, string name);
   ~IdentNode();
   void printTo(ostream & os);
};

class NestedExprNode : public FactorNode{
public:
   ExprNode* exprPtr = nullptr;

   NestedExprNode(int level, ExprNode* en);
   ~NestedExprNode();
   void printTo(ostream & os);
};

class NotNode : public FactorNode{
public:
      FactorNode *factor = nullptr;

      NotNode(int level, FactorNode* factor);
      ~NotNode();
      void printTo(ostream &os);
};

class MinusNode : public FactorNode{
public:
   FactorNode *factor = nullptr;

   MinusNode(int level, FactorNode *factor);
   ~MinusNode();
   void printTo(ostream &os);
};
                                            
#endif
