/********************************************************************************
Author: Alexander Kellough       NetID:atk133
Title: Project Part 4
Purpose:
   Class and Function Definitions for Parse Tree Generation
 ********************************************************************************/
#include "parse_tree_nodes.h"
#include <cstdarg>
#include <cstdlib>
#include <cmath>
#include <type_traits>

#define EPSILON 0.001

bool printDelete = false;

static bool truth(float F){
  return !((EPSILON > F) && (F > -EPSILON));
}

static void indent(int level) {
  for (int i = 0; i < level - 1; i++)
    cout << ("|  ");
}

static bool equalTo(float F, float G)
{
  if(abs(F-G) <= EPSILON)
    return true;
  
  else
    return false;
}

// FactorNode
FactorNode::~FactorNode() {}

ostream& operator<<(ostream& os, FactorNode& fn) {
  os << endl; indent(fn._level); os << "(factor ";
  fn.printTo(os);
  os << endl; indent(fn._level); os << "factor) ";
	return os;
}

float FactorNode::interpret() {
  return this->interpret();
}
// IntLitNode
IntLitNode::IntLitNode(int level, int value){
    _level = level;
    int_literal = value;
}

IntLitNode::~IntLitNode(){

  if(printDelete)
    cout << "Deleting FactorNode:IntLitNode" << endl;
}

void IntLitNode::printTo(ostream &os){
    os << "( INTLIT: " << int_literal << " ) ";
}

float IntLitNode::interpret() {
  return int_literal;
}

// FloatLitNode
FloatLitNode::FloatLitNode(int level, float value){
    _level = level;
    float_literal = value;
}

FloatLitNode::~FloatLitNode(){
  if(printDelete)
    cout << "Deleting FactorNode:FloatLitNode" << endl;
}

void FloatLitNode::printTo(ostream &os){
    os << "( FLOATLIT: " << float_literal << " ) ";
}

float FloatLitNode::interpret() {
  return float_literal;
}

//IdentNode
IdentNode::IdentNode(int level, string name){
    _level = level;
    id = new string(name);
}

IdentNode::~IdentNode(){
  if(printDelete)
    cout << "Deleting FactorNode:IdNode" << endl;

  delete id;
  id = nullptr;
}

void IdentNode::printTo(ostream &os){
    os << "( IDENT: " << *id << " ) ";
}

float IdentNode::interpret() {
  map<string, float>::iterator it = ids.find(*id);
  return it->second;
}

//NestedExpNode
NestedExprNode::NestedExprNode(int level, ExprNode *en){
    _level = level;
    exprPtr = en;
}

NestedExprNode::~NestedExprNode(){
  if(printDelete)
    cout << "Deleting FactorNode:NestedExprNode" << endl;

  delete exprPtr;
  exprPtr = nullptr;
}

void NestedExprNode::printTo(ostream &os){
    os << "( "<< endl << *exprPtr <<")";
}

float NestedExprNode::interpret() {
  return exprPtr->interpret();
}
//NotNode
NotNode::NotNode(int level, FactorNode *factor){
    _level = level;
    this->factor = factor;
}

NotNode::~NotNode(){
  if(printDelete)
    cout << "Deleting FactorNode:NotNode" << endl;

  delete factor;
  factor = nullptr;
}

void NotNode::printTo(ostream &os){
    os << "(NOT ";
    os << *factor;
    os << ") ";
}

float NotNode::interpret() {
  return !factor->interpret(); // This needs logic for ids and such
}
//MinusNode
MinusNode::MinusNode(int level, FactorNode* factor){
    _level = level;
    this->factor = factor;
}

MinusNode::~MinusNode(){
  if(printDelete)
    cout << "Deleting FactorNode:MinusNode" << endl;

  delete factor;
  factor = nullptr;
}

void MinusNode::printTo(ostream &os){
    os << "(- ";
    os << *factor;
    os << ") ";
}

float MinusNode::interpret(){
  return -factor->interpret();
}
//TermNode
TermNode::TermNode(int level) {
  _level = level; //Requires Logic for Id's and such
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
  if(printDelete)
    cout << "Deleting TermNode" << endl;

	delete firstFactor;
	firstFactor = nullptr;

	int length = restFactorOps.size();
	for (int i = 0; i < length; ++i) {
		delete restFactors[i];
		restFactors[i] = nullptr;
	}
}

float TermNode::interpret()
{
  // a Term is a Factor followed by 0 or more Factors separated by * or / operations

  // get the value of the first Factor
  float returnValue = firstFactor->interpret();

  int length = restFactorOps.size();
  for (int i = 0; i < length; ++i) {
    // get the value of the next Factor
    float nextValue = restFactors[i]->interpret();
    
    // perform the operation (* or /) that separates the Factors
    switch(restFactorOps[i]) {
      case TOK_MULTIPLY:
        returnValue = returnValue * nextValue;
      break;
      case TOK_DIVIDE:
        returnValue = returnValue / nextValue;
      break;
      case TOK_AND:
        returnValue = returnValue && nextValue;
    }
  }
  return returnValue;
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
  if(printDelete)
    cout << "Deleting SimpleExpNode" << endl;

	delete firstTerm;
	firstTerm = nullptr;

	int length = restTermOps.size();
	for (int i = 0; i < length; ++i) {
		delete restTerms[i];
		restTerms[i] = nullptr;
	}
}

float SimpleExprNode::interpret()
{
  // an Expr is a Term followed by 0 or more Terms separated by + or - operations

  // get the value of the first Term
  float returnValue = firstTerm->interpret();

  int length = restTermOps.size();
  for (int i = 0; i < length; ++i) {
    // get the value of the next Term
    float nextValue = restTerms[i]->interpret();

    // perform the operation (+ or -) that separates the Terms
    switch(restTermOps[i]) {
      case TOK_PLUS:
        returnValue = returnValue + nextValue; 
      break;
      case TOK_MINUS:
        returnValue = returnValue - nextValue; 
      break;
      case TOK_OR:
        returnValue = returnValue || nextValue;
    }
  }
  return returnValue;
}

// Expr Node
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
  if(printDelete)
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

float ExprNode::interpret() {
   float returnValue = firstSimpleExpr->interpret();

  int length = restSimpleExprOps.size();
  for (int i = 0; i < length; ++i) {
    // get the value of the next Term
    float nextValue = restSimpleExpr[i]->interpret();

    // perform the operation (+ or -) that separates the Terms
    switch(restSimpleExprOps[i]) {
      case TOK_EQUALTO:
        returnValue = equalTo(returnValue, nextValue); 
      break;
      case TOK_LESSTHAN:
        returnValue = returnValue < nextValue; 
      break;
      case TOK_GREATERTHAN:
        returnValue = returnValue > nextValue;
      break;
      case TOK_NOTEQUALTO:
        returnValue = equalTo(returnValue , nextValue);
      break;
    }
  }
  return returnValue;
}

// Statement Node
StatementNode::~StatementNode() {}
ostream& operator<<(ostream& os, StatementNode& fn) {
  fn.printTo(os);
	return os;
}

// Assignemnt Node
AssignmentNode::AssignmentNode(int level){
  _level = level;
}

AssignmentNode::~AssignmentNode() {
  if(printDelete)
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

void AssignmentNode::interpret() {
  map<string, float>::iterator it = ids.find(*ident);
  it->second = exp->interpret();
}

// Compound Node
CompoundNode::CompoundNode(int level){
  _level = level;
}

CompoundNode::~CompoundNode() {
  if(printDelete)
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

void CompoundNode::interpret(){
  for(int i = 0; i < statementList.size(); ++i)
  {
    statementList[i]->interpret();
  }
}

// If Node
IfNode::IfNode(int level) {
  _level = level;
}

IfNode::~IfNode()
{
  if(printDelete)
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

void IfNode::interpret() {
  if(truth(ifExpression->interpret()))
  {
    thenStatement->interpret();
  }
  else{
    if(elf)
    {
      elseStatement->interpret();
    }
  }
}

// While Node
WhileNode::WhileNode(int level) {
  _level = level;
}

WhileNode::~WhileNode(){
  if(printDelete)
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

void WhileNode::interpret(){
  while(truth(cond->interpret())){
    result->interpret();
  }
}
// Read Node
ReadNode::ReadNode(int level, string* value){
  _level = level;
  ident = value;
}

ReadNode::~ReadNode() {
  if(printDelete)
    cout << "Deleting StatementNode:ReadStmtNode" << endl;

  delete ident;
  ident = nullptr;
}

void ReadNode::printTo(ostream &os){
  indent(_level);os << "(read_stmt ( " << *ident << " )" << endl;
  indent(_level);os << "read_stmt)" << endl;
}

void ReadNode::interpret(){
  map<string, float>::iterator it = ids.find(*ident);
  cin >> it->second;
}

// Write Node
WriteNode::WriteNode(int level, string* value){
  _level = level;
  _value = value;
}

WriteNode::~WriteNode() {
  if(printDelete)
    cout << "Deleting StatementNode:WriteStmtNode" << endl;

  delete _value;
  _value = nullptr;
}

void WriteNode::printTo(ostream &os){
  indent(_level);os << "(write_stmt ( " << *_value << " )" << endl;
  indent(_level);os << "write_stmt)" << endl;
}

void WriteNode::interpret(){
  map<string, float>::iterator it = ids.find(*_value);
  if(it != ids.end())
  {
    cout << it->second << endl;
  }
  else {
    cout << *_value << endl;
  }
}
// Block Node
BlockNode::BlockNode(int level, CompoundNode* value) {
  _level = level;
  cmpd = value;
}

BlockNode::~BlockNode(){
  if(printDelete)
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

void BlockNode::interpret(){
  cmpd->interpret();
}
// Program Node
ProgramNode::ProgramNode(int level, BlockNode* value) {
  _level = level;
  block = value;
}

ProgramNode::~ProgramNode() {
  if(printDelete)
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

void ProgramNode::interpret(){
  block->interpret();
}