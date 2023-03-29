/********************************************************************************
Author: Alexander Kellough       NetID:atk133
Title: Project Part 4 
Purpose:
   Class and Function Declerations for Parse Tree Generation
 ********************************************************************************/

#ifndef PARSE_TREE_NODES_H
#define PARSE_TREE_NODES_H

#include <iostream>
#include <vector>
#include <string>
#include "lexer.h"
#include <map>

using namespace std;

extern map<string, float> ids;

extern bool printDelete;

class ProgramNode;
class BlockNode;
class StatementNode;
class CompoundNode;
class ExprNode;
class SimpleExprNode;
class TermNode;
class FactorNode;

class ProgramNode {
public:
    int _level;
    BlockNode* block = nullptr;

    ProgramNode(int level, BlockNode* value);
    ~ProgramNode();
    void interpret();
};
ostream& operator << (ostream&, ProgramNode&);

class BlockNode {
public:
    int _level;
    CompoundNode* cmpd = nullptr;

    BlockNode(int level, CompoundNode* value);
    ~BlockNode();
    void interpret();
};
ostream& operator<<(ostream&, BlockNode&);

class StatementNode {
public:
    int _level = 0;

    virtual void printTo(ostream &os) = 0;
    virtual ~StatementNode();
    virtual void interpret() = 0;
};
ostream& operator<<(ostream&, StatementNode&);

class AssignmentNode : public StatementNode { 
public:
    string* ident = nullptr;
    ExprNode* exp = nullptr;

    AssignmentNode(int level);
    ~AssignmentNode();
    void printTo(ostream &os);
    void interpret();
};

class CompoundNode : public StatementNode {
public:
    vector<StatementNode*> statementList;

    CompoundNode(int level);
    ~CompoundNode();
    void printTo(ostream &os);
    void interpret();
};

class IfNode : public StatementNode {
public:
    bool elf = false;
    ExprNode* ifExpression = nullptr;
    StatementNode* thenStatement = nullptr;
    StatementNode* elseStatement = nullptr;

    IfNode(int level);
    ~IfNode();
    void printTo(ostream &os);
    void interpret();
};

class WhileNode : public StatementNode {
public:
    ExprNode* cond = nullptr;
    StatementNode* result = nullptr;

    WhileNode(int level);
    ~WhileNode();
    void printTo(ostream &os);
    void interpret();
};

class ReadNode : public StatementNode {
public:
    string* ident = nullptr;

    ReadNode(int level, string* value);
    ~ReadNode();
    void printTo(ostream &os);
    void interpret();
};

class WriteNode : public StatementNode {
public:
    string* _value = nullptr;

    WriteNode(int level, string* value);
    ~WriteNode();
    void printTo(ostream &os);
    void interpret();
};

class ExprNode {
public:
   int _level = 0;
   SimpleExprNode* firstSimpleExpr = nullptr;
   vector<int> restSimpleExprOps;
   vector<SimpleExprNode*> restSimpleExpr;

   ExprNode(int level);
   ~ExprNode();
   float interpret();
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
   float interpret();
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
   float interpret();
};
ostream& operator<<(ostream&, TermNode&);

class FactorNode {
 public:
    int _level = 0;                        // recursion level of this node

    virtual void printTo(ostream &os) = 0; // pure virtual method, makes the class Abstract
    virtual ~FactorNode(); 
    virtual float interpret();                // labeling the destructor as virtual allows
                                           // the subclass destructors to be called
};                            
ostream& operator<<(ostream&, FactorNode&); // Node print operator
                                        
class IntLitNode : public FactorNode {
public:
   int int_literal = 0;

   IntLitNode(int level, int value);
   ~IntLitNode();
   void printTo(ostream & os);
   float interpret();
};

class FloatLitNode : public FactorNode{
public:
   float float_literal = 0;

   FloatLitNode(int level, float value);
   ~FloatLitNode();
   void printTo(ostream & os);
   float interpret();
};

class IdentNode : public FactorNode{
public:
   string* id = nullptr;

   IdentNode(int level, string name);
   ~IdentNode();
   void printTo(ostream & os);
   float interpret();
};

class NestedExprNode : public FactorNode{
public:
   ExprNode* exprPtr = nullptr;

   NestedExprNode(int level, ExprNode* en);
   ~NestedExprNode();
   void printTo(ostream & os);
   float interpret();
};

class NotNode : public FactorNode{
public:
      FactorNode *factor = nullptr;

      NotNode(int level, FactorNode* factor);
      ~NotNode();
      void printTo(ostream &os);
      float interpret();
};

class MinusNode : public FactorNode{
public:
   FactorNode *factor = nullptr;

   MinusNode(int level, FactorNode *factor);
   ~MinusNode();
   void printTo(ostream &os);
   float interpret();
};
                                            
#endif
