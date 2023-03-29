/***************************************************************************
 
Name: Alexander Kellough            NetID: atk133

Program: Class-Project-Part-4      Class: CSE 4714

Description:
    Header file declaring useful parsing constants and functions.

***************************************************************************/

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "lexer.h"
#include "parse_tree_nodes.h"

using namespace std;

extern bool printParse;

extern int nextToken;        // next token returned by lexer

extern "C" {
	// Instantiate global variables used by flex
	extern int   yylex();      // the generated lexical analyzer
	extern char* yytext;       // text of current lexeme
}

// Function declarations
ProgramNode* program();


#endif /* PARSER_H */
