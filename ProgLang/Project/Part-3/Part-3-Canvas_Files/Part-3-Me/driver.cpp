//*****************************************************************************
// purpose: Driver for Project Part 3 (parser for TIPS)
// version: Fall 2022
//*****************************************************************************
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <iostream>
#include "lexer.h"
#include "parser.h"
#include <vector>
#include <bits/stdc++.h>
#include "parse_tree_nodes.h"

using namespace std;

extern "C" {
    // Instantiate global variables
    extern FILE *yyin;     // input stream
    extern FILE *yyout;    // output stream
    extern int   yyleng;   // length of current lexeme
    extern char *yytext;   // text of current lexeme
    extern int   yylineno; // line number for current lexeme
    extern int   yylex();  // the generated lexical analyzer
}

// Needed global variables
extern int nextToken; // token returned from yylex

//*****************************************************************************
// The main processing loop
//
int main(int argc, char* argv[]) {

    // Set the input stream
    if (argc > 1) {
        printf("INFO: Using the %s file for input\n", argv[1]);
        yyin = fopen(argv[1], "r"); 
    }
    else {
        printf("INFO: Using the sample.pas file for input\n");
        yyin = fopen("sample.pas", "r");
    }
  
    if (!yyin) {
        printf("ERROR: input file not found\n");
        return EXIT_FAILURE;
    }

    // Set the output stream
    yyout = stdout;

    // Get the first token
    nextToken = yylex();

    ProgramNode* root;

    // Fire up the parser!
    try {
        // Process the expected production
        root = program();  
        //factor(); // Build bottom-up to <expression>

        if (nextToken != TOK_EOF)
            throw "end of file expected, but there is more here!";
        
    } catch (char const *errmsg) {
        cout << endl << "***ERROR:" << endl;
        cout << "On line number " << yylineno << ", near |" << yytext << "|, error type ";
        cout << errmsg << endl;
        return EXIT_FAILURE;
    }

    // Tell the world about our success!!
    cout << endl << "=== GO BULLDOGS! Your parse was successful! ===" << endl;

    // Print the Tree
    cout << endl << "*** Print the Tree ***" << endl << endl;
    cout << *root; 

    // Print out the symbol table
    sort(ids.begin(), ids.end());
    cout << endl << endl << "*** User Defined Symbols ***" << endl;
    vector<string>::iterator it;
    for (it = ids.begin(); it != ids.end(); ++it) {
        cout << *it << endl;
    }

    // Delete the tree
    cout << endl << "*** Delete the Tree ***" << endl;
    delete root;
    root = nullptr;

    return EXIT_SUCCESS;
}

