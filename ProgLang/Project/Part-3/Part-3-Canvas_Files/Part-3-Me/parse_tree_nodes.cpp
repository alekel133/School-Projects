/********************************************************************************
Author: Alexander Kellough       NetID:atk133
Title: Project Part 3 
Purpose:
   Class and Function Definitions for Parse Tree Generation
 ********************************************************************************/
#include "parse_tree_nodes.h"
#include <cstdarg>
#include <cstdlib>
#include <type_traits>

static void indent(int level) {
  for (int i = 0; i < level - 1; i++)
    cout << ("|  ");
}

// FactorNode
FactorNode::~FactorNode() {}

ostream& operator<<(ostream& os, FactorNode& fn) {
  os << endl; indent(fn._level); os << "(factor ";
  fn.printTo(os);
  os << endl; indent(fn._level); os << "factor) ";
	return os;
}

// IntLitNode
IntLitNode::IntLitNode(int level, int value){
    _level = level;
    int_literal = value;
}

IntLitNode::~IntLitNode(){

    cout << "Deleting FactorNode:IntLitNode" << endl;
}

void IntLitNode::printTo(ostream &os){
    os << "( INTLIT: " << int_literal << " ) ";
}

// FloatLitNode
FloatLitNode::FloatLitNode(int level, float value){
    _level = level;
    float_literal = value;
}

FloatLitNode::~FloatLitNode(){
    cout << "Deleting FactorNode:FloatLitNode" << endl;
}

void FloatLitNode::printTo(ostream &os){
    os << "( FLOATLIT: " << float_literal << " ) ";
}

//IdentNode
IdentNode::IdentNode(int level, string name){
    _level = level;
    id = new string(name);
}

IdentNode::~IdentNode(){
    cout << "Deleting FactorNode:IdNode" << endl;
    delete id;
    id = nullptr;
}

void IdentNode::printTo(ostream &os){
    os << "( IDENT: " << *id << " ) ";
}

//NestedExpNode
NestedExprNode::NestedExprNode(int level, ExprNode *en){
    _level = level;
    exprPtr = en;
}

NestedExprNode::~NestedExprNode(){
    cout << "Deleting FactorNode:NestedExprNode" << endl;
    delete exprPtr;
    exprPtr = nullptr;
}

void NestedExprNode::printTo(ostream &os){
    os << "( "<< endl << *exprPtr <<")";
}

//NotNode
NotNode::NotNode(int level, FactorNode *factor){
    _level = level;
    this->factor = factor;
}

NotNode::~NotNode(){
    cout << "Deleting FactorNode:NotNode" << endl;
    delete factor;
    factor = nullptr;
}

void NotNode::printTo(ostream &os){
    os << "(NOT ";
    os << *factor;
    os << ") ";
}

//MinusNode
MinusNode::MinusNode(int level, FactorNode* factor){
    _level = level;
    this->factor = factor;
}

MinusNode::~MinusNode(){
    cout << "Deleting FactorNode:MinusNode" << endl;
    delete factor;
    factor = nullptr;
}

void MinusNode::printTo(ostream &os){
    os << "(- ";
    os << *factor;
    os << ") ";
}

//TermNode
TermNode::TermNode(int level) {
  _level = level;
}

ostream& operator<<(ostream& os, TermNode& tn) {
  os << endl; indent(tn._level); os << "(term ";
	os << *(tn.firstFactor);

	int length = tn.restFactorOps.size();
	for (int i = 0; i < length; ++i) {
		int op = tn.restFactorOps[i];
    if (op == TOK_MULTIPLY) {
      os << endl; indent(tn._level); os << "* ";
    } else {
      os << endl; indent(tn._level); os << "/ ";
    }
		os << *(tn.restFactors[i]);
	}
  os << endl; indent(tn._level); os << "term) ";
	return os;
}

TermNode::~TermNode() {
    cout << "Deleting TermNode" << endl;
	delete firstFactor;
	firstFactor = nullptr;

	int length = restFactorOps.size();
	for (int i = 0; i < length; ++i) {
		delete restFactors[i];
		restFactors[i] = nullptr;
	}
}

// Simple Expression
SimpleExprNode::SimpleExprNode(int level) {
  _level = level;
}
ostream& operator<<(ostream& os, SimpleExprNode& en) {
  os << endl; indent(en._level); os << "(simple_exp ";
	os << *(en.firstTerm);

	int length = en.restTermOps.size();
	for (int i = 0; i < length; ++i) {
    int op = en.restTermOps[i]; 

    if (op == TOK_PLUS) { 
      os << endl; indent(en._level); os << "+ "; 
    } 
    else { 
      os << endl; indent(en._level); os << "- ";
      }
      os << *(en.restTerms[i]);
	}
  os << endl; indent(en._level); os << "simple_exp) ";
	return os;
}

SimpleExprNode::~SimpleExprNode() {
    cout << "Deleting SimpleExpNode" << endl;
	delete firstTerm;
	firstTerm = nullptr;

	int length = restTermOps.size();
	for (int i = 0; i < length; ++i) {
		delete restTerms[i];
		restTerms[i] = nullptr;
	}
}

ExprNode::ExprNode(int level)
{
    _level = level;
}
ostream& operator<<(ostream& os, ExprNode& en) {
  indent(en._level); os << "(expression ";
	os << *(en.firstSimpleExpr);

	int length = en.restSimpleExprOps.size();
	for (int i = 0; i < length; ++i) {
		int op = en.restSimpleExprOps[i];
    if (op == TOK_EQUALTO) {
      os << endl; indent(en._level); os << "= ";
    }
    else if (op == TOK_GREATERTHAN) {
      os << endl; indent(en._level); os << "> ";
    } 
    else if(op == TOK_LESSTHAN) {
      os << endl; indent(en._level); os << "< ";
    }
    else{
      os << endl; indent(en._level); os << "<> ";
    }
		os << *(en.restSimpleExpr[i]);
  }
  os << endl; indent(en._level); os << "expression) ";
	return os;
}

ExprNode::~ExprNode()
{
    cout << "Deleting ExpressionNode" << endl;
    delete firstSimpleExpr;
    firstSimpleExpr = nullptr;

    int length = restSimpleExpr.size();
    for(int i = 0; i < length; ++i)
    {
        delete restSimpleExpr[i];
        restSimpleExpr[i] = nullptr;
    }
}

StatementNode::~StatementNode() {}
ostream& operator<<(ostream& os, StatementNode& fn) {
  fn.printTo(os);
	return os;
}

AssignmentNode::AssignmentNode(int level){
  _level = level;
}

AssignmentNode::~AssignmentNode() {
  cout << "Deleting StatementNode:AssignmentStmtNode" << endl;
  delete ident;
  ident = nullptr;
  delete exp;
  exp = nullptr;
}

void AssignmentNode::printTo(ostream &os)
{
  indent(_level); os << "(assignment_stmt ( " << *ident << " := )" << endl;
  os << *exp << endl;
  indent(_level); os << "assignment_stmt)" << endl;
}

CompoundNode::CompoundNode(int level){
  _level = level;
}

CompoundNode::~CompoundNode() {
  cout << "Deleting StatementNode:CompoundStmtNode" << endl;
  int length = statementList.size();
    for(int i = 0; i < length; ++i)
    {
        delete statementList[i];
        statementList[i] = nullptr;
    }
}

void CompoundNode::printTo(ostream &os){
  indent(_level); os << "(compound_stmt" << endl;
  for(int i = 0; i < statementList.size(); ++i)
  {
    os << *statementList[i];
  }
  indent(_level); os << "compound_stmt)" << endl;
}

IfNode::IfNode(int level) {
  _level = level;
}

IfNode::~IfNode()
{
  cout << "Deleting StatementNode:IfStmtNode" << endl;
  delete ifExpression;
  delete thenStatement;
  ifExpression = nullptr;
  thenStatement = nullptr;
  if(elf)
  {
    delete elseStatement;
    elseStatement = nullptr;
  }
}

void IfNode::printTo(ostream &os){
  indent(_level); os << "(if_stmt " << endl;
  os << *ifExpression << endl;
  indent(_level);os << "(then " << endl;
  os << *thenStatement;
  indent(_level);os << "then) " << endl;
  
  if(elf)
  {
    indent(_level);os << "(else " << endl;
    os << *elseStatement;
    indent(_level);os << "else) " << endl;
  }
  indent(_level);os << "if_stmt)" << endl;
}

WhileNode::WhileNode(int level) {
  _level = level;
}

WhileNode::~WhileNode(){
  cout << "Deleting StatementNode:WhileStmtNode" << endl;
  delete cond;
  delete result;
  cond = nullptr;
  result = nullptr;
}

void WhileNode::printTo(ostream &os){
  indent(_level);os << "(while_stmt " << endl;
  os << *cond << endl;
  os << *result;
  indent(_level);os << "while_stmt)" << endl;
}

ReadNode::ReadNode(int level, string* value){
  _level = level;
  ident = value;
}

ReadNode::~ReadNode() {
  cout << "Deleting StatementNode:ReadStmtNode" << endl;
  delete ident;
  ident = nullptr;
}

void ReadNode::printTo(ostream &os){
  indent(_level);os << "(read_stmt ( " << *ident << " )" << endl;
  indent(_level);os << "read_stmt)" << endl;
}

WriteNode::WriteNode(int level, string* value){
  _level = level;
  _value = value;
}

WriteNode::~WriteNode() {
  cout << "Deleting StatementNode:WriteStmtNode" << endl;
  delete _value;
  _value = nullptr;
}

void WriteNode::printTo(ostream &os){
  indent(_level);os << "(write_stmt ( " << *_value << " )" << endl;
  indent(_level);os << "write_stmt)" << endl;
}

BlockNode::BlockNode(int level, CompoundNode* value) {
  _level = level;
  cmpd = value;
}

BlockNode::~BlockNode(){
  cout << "Deleting BlockNode" << endl;
  delete cmpd;
  cmpd = nullptr;
}
ostream& operator<<(ostream &os, BlockNode &bl)
{
  indent(bl._level); os << "(block " << endl; ///indent(bl._level);
  os << *bl.cmpd;
  indent(bl._level); os << "block) "; 
  return os;
}

ProgramNode::ProgramNode(int level, BlockNode* value) {
  _level = level;
  block = value;
}

ProgramNode::~ProgramNode() {
  cout << "Deleting ProgramNode" << endl;
  delete block;
  block = nullptr;
}
ostream& operator<<(ostream &os, ProgramNode& pr) {
  indent(pr._level); os << "(program " << endl; 
  os << *pr.block << endl;;
  indent(pr._level); os << "program) " << endl; 
  return os;
}