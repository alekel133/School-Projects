/*******************************************************************

*******************************************************************/
#include "parse_tree_nodes.h"
#include <cstdarg>

static void indent(int level) {
  for (int i = 0; i < level; i++)
    cout << ("| ");
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

    cout << "Deleteing FactorNode:IntLitNode" << endl;
}

void IntLitNode::printTo(ostream &os){
    os << "( INTLIT: " << int_literal << " )";
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
    os << "( FloatLit: " << float_literal << " )";
}

//IdentNode
IdentNode::IdentNode(int level, string name){
    _level = level;
    id = new string(name);
}

IdentNode::~IdentNode(){
    cout << "Deleting FactorNode:IdentNode" << endl;
    delete id;
    id = nullptr;
}

void IdentNode::printTo(ostream &os){
    os << "( Identifier: " << *id << " )";
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
    cout << exprPtr;
}

//NotNode
NotNode::NotNode(int level, FactorNode *factor){
    _level = level;
    this->factor = factor;
}

NotNode::~NotNode(){
    cout << "Deleting FactorNode: NotNode" << endl;
    delete factor;
    factor = nullptr;
}

void NotNode::printTo(ostream &os){
    os << "(NOT";
    os << factor;
    os << " )";
}

//MinusNode
MinusNode::MinusNode(int level, FactorNode* factor){
    _level = level;
    this->factor = factor;
}

MinusNode::~MinusNode(){
    cout << "Deleteing FactorNode:MinusNode" << endl;
    delete factor;
    factor = nullptr;
}

void MinusNode::printTo(ostream &os){
    os << "(-";
    os << factor;
    os << " )";
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
  if(printDelete)
    cout << "Deleting TermNode " << endl;
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
  os << endl; indent(en._level); os << "(simple_expr ";
	os << *(en.firstTerm);

	int length = en.restTermOps.size();
	for (int i = 0; i < length; ++i) {
		int op = en.restTermOps[i];
    if (op == TOK_PLUS) {
      os << endl; indent(en._level); os << "+ ";
    } else {
      os << endl; indent(en._level); os << "- ";
    }
		os << *(en.restTerms[i]);
	}
  os << endl; indent(en._level); os << "simple_expr) ";
	return os;
}

SimpleExprNode::~SimpleExprNode() {
  if(printDelete)
    cout << "Deleting SimpleExprNode " << endl;
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

ExprNode::~ExprNode()
{
    cout << "Deleting ExprNode" << endl;
    delete firstSimpleExpr;
    firstSimpleExpr = nullptr;

    int length = restSimpleExpr.size();
    for(int i = 0; i < length; ++i)
    {
        delete restSimpleExpr[i];
        restSimpleExpr[i] = nullptr;
    }
}