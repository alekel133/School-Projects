/***************************************************************************
 
Name: Alexander Kellough            NetID: atk133

Program: Class-Project-Part-2       Class: CSE 4714

Description:
    Header file declaring useful parsing constants and functions.

***************************************************************************/

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

// Holds the symbols in the interpreted program
extern vector<string> ids;

extern int nextToken;        // next token returned by lexer

extern "C" {
	// Instantiate global variables used by flex
	extern int   yylex();      // the generated lexical analyzer
	extern char* yytext;       // text of current lexeme
}

// Function declarations
void program();

#endif /* PARSER_H */
