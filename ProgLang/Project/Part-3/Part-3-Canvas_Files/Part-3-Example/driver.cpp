//*****************************************************************************
// (part 3 example)
// purpose: driver file for parser example - flex generated lexer
//          builds parse tree while parsing the input
// version: Spring 2022
// author: Joe Crumpton / Ed Swan
//*****************************************************************************
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <iostream>
#include "lexer.h"
#include "parser.h"
#include "parse_tree_nodes.h"

using namespace std;

extern "C"
{
  // Instantiate global variables
  extern FILE *yyin;        // input stream
  extern FILE *yyout;       // output stream
  extern int   yyleng;      // length of current lexeme
  extern char *yytext;      // text of current lexeme
  extern int   yylineno;    // line number for current lexeme
  extern int   yylex();     // the generated lexical analyzer
  extern int   yylex_destroy(); // deletes memory allocated by yylex
}

//*****************************************************************************
// The main processing loop
//
int main(int argc, char* argv[])
{
  bool printTree = true; // whether to print the parse tree

  // Process any command-line switches
  for(int i = 1; i < argc; i++) {
    // -p flag: if requested, print while parsing
    if(strcmp(argv[i], "-p") == 0) {
      printParse = true;
    }
    // -t flag: if requested, print parse tree
    if(strcmp(argv[i], "-t") == 0) {
      printTree = true;
    }
    // -d flag: if requested, print while deleting parse tree
    if(strcmp(argv[i], "-d") == 0) {
      printDelete = true;
    }
  }
  // Set the input stream
  if (argc > 1) {
    printf("INFO: Using the %s file for input\n", argv[argc-1]);
    yyin = fopen(argv[argc-1], "r"); 
  }
  // Otherwise, flex sets yyin to be stdin, the standard input
  
  // There are three ways to execute the program.  All of these can make use of
  // the command-line switches shown above.

  // (1) The file name can be given on the command line:
  //     $ parse.exe front.in
  //     $ parse.exe -d -p front.in
  
  // (2) After starting the program, an expression can be typed interactively.
  //     If so, to end must type "Ctrl-D" (generates EOF character).  
  //     $ parse.exe -d
  //     INFO: Using the -d file for input
  //     2 + sum
  //     ^D

  // (3) Files or strings can be sent into the parse with the pipe operator, like
  //     this:
  //     echo "5 + 3" | parse.exe
  //     echo "5 + 3" | parse.exe -p -d
  //     cat front.in | parse.exe
  //     cat front.in | parse.exe -p -d

  // Create the root of the parse tree
  ExprNode* root = nullptr;

  lex();  // prime the pump (get first token)
  do {
    // Start symbol is <expr>
    root = expr(); 
  } while(nextToken != TOK_EOF);

  if (yyin)
    fclose(yyin);

  yylex_destroy();

  // Printing and Deleting the tree all result in 
  // the same in-order traversal of the tree as parsing.  All
  // use the call stack.

  if(printTree) {
    cout << endl << "*** Print the Tree ***" << endl;
    cout << *root << endl << endl;
  }

  cout << "*** Delete the Tree ***" << endl;
  delete root;
  root = nullptr;

  return(EXIT_SUCCESS);
}
