//*****************************************************************************
// (part 3)
// purpose: Recursive descent parsing of arithmetic expressions
//          Builds the parse tree while parsing the input
// version: Spring 2022
//  author: Joe Crumpton / Ed Swan
//*****************************************************************************

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "parse_tree_nodes.h"
#include <stdlib.h>
#include <iostream>

extern "C" {
  extern FILE *yyin;       // input stream
  extern int   yylex();    // the generated lexical analyzer
  extern char *yytext;     // text of current lexeme
}
extern int nextToken;        // next token returned by lexer
extern bool printParse;      // shall tree be printed while parsing?

/* Function declarations */
int lex();                   // return the next token

ExprNode* expr();
TermNode* term();
FactorNode* factor();

#endif /* PARSER_H */