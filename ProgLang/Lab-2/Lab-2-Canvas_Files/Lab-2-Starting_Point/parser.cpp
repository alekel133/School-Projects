/**************************************************************************
  Name: Alexander Kellough                NetID:atk133
  Course: CSE 4714                    
  Lab 02 - Parsing
  Purpose of a file: Contains code for a parser of a subset of the English
  Language: <sentence> -> <noun phrase> <verb phrase> <noun phrase>
**************************************************************************/

#include <stdio.h>
#include <iostream>
#include "lexer.h"
#include "parser.h"

using namespace std;

extern "C"
{
  extern int   yylex();      // the generated lexical analyzer
  extern char *yytext;       // text of current lexeme
}

int nextToken = 0;
int count = 0;

// Production functions
bool firstOf_sentence();
bool firstOf_adjectivePhrase();
bool firstOf_nounPhrase();
void adjectivePhrase();
void nounPhrase();
bool verbPhrase();
void printTab(int rec);


//*****************************************************************************
// Get the next lexeme (word in sentence)
int lex() {
  nextToken = yylex();
  if( nextToken == TOK_EOF ) {
    yytext[0] = 'E'; yytext[1] = 'O'; yytext[2] = 'F'; yytext[3] = 0;   
  }

  printf("Next token: %d, lexeme: |%s|\n", nextToken, yytext);
  return nextToken;
}
//*****************************************************************************
// Report what we found
void output( string what ) {
  cout << "===> Accepted " << what << ": |" << yytext << "| <===" << endl;
}
//*****************************************************************************
// <sentence> -> <noun phrase> <verb phrase> <noun phrase>
void sentence() 
{
  if( firstOf_sentence() == false )
    throw( "<sentence> did not start with an article or possessive." );

  cout << "Enter <sentence>" << endl;

  nounPhrase();
  verbPhrase();
  nounPhrase();

  cout << "Exit <sentence>" << endl;
} 
//*****************************************************************************
bool firstOf_sentence() {
    int tok = nextToken;
    if(tok != ARTICLE && tok != POSSESSIVE)
        return false;

    return true;
}


bool firstOf_adjectivePhrase()
{
    if(nextToken != ARTICLE && nextToken != POSSESSIVE)
        return false;

    return true;
}

bool firstOf_nounPhrase()
{
    if(!firstOf_adjectivePhrase())
        return false;

    return true;
}


void adjectivePhrase() 
{
    if(!firstOf_adjectivePhrase())
    {
        throw("<adjective phrase> did not start with an article or possessive");
    }

    printTab(count);
    cout << "Enter <adjective phrase> " << count << endl;

    if(nextToken == ARTICLE)
        output("ARTICLE");

    if(nextToken == POSSESSIVE)
        output("POSSESSIVE");

    lex();
    int tok = nextToken;

    if(tok != ADJECTIVE)
    {
        throw( "<adjective phrase> did not have and adjective>." );
    }

    output("ADJECTIVE");
    lex();

    printTab(count);
    cout << "Exit <adjective phrase> " << count << endl;
}

void nounPhrase()
{
    if(!firstOf_nounPhrase())
    {
        throw("<noun phrase> did not start with an article or possessive." );
    }

    ++count;
    printTab(count);
    cout << "Enter <noun phrase> "<< count << endl;

    adjectivePhrase();

    int tok = nextToken;

    if(tok != NOUN)
    {
        throw( "<noun phrase> did not have a noun.");
        cout << "Exit <noun phrase> "<< count << endl;
    }

    output("NOUN"); 

    lex();
    printTab(count);
    cout << "Exit <noun phrase> " << count << endl;
}

bool verbPhrase()
{
    printTab(count);
    cout << "Enter <verb phrase> " << count << endl;

    if(nextToken != VERB && nextToken != ADVERB)
    {
        return false;
    }

    if(nextToken == VERB)
    {
        output("VERB");
        lex();
        printTab(count);
        cout << "Exit <verb phrase> " << count << endl;
        return true;
    }

    if(nextToken == ADVERB)
    {
       output("ADVERB"); 
       lex();
       ++count;
       verbPhrase();
       --count;
       printTab(count);
       cout << "Exit <verb phrase> " << count << endl;
       return true;
    }
    return false;
}
void printTab(int rec)
{
    for(int i = 0; i < rec; ++i)
    {
        cout << "  ";
    }
}

