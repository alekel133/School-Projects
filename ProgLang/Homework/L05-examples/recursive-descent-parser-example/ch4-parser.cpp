//*****************************************************************************
// purpose: CSE 4714 / 6714 example recursive descent parser
//          (Sebesta Section 4.4)
//  author: J. Edward Swan II
//   usage: 
//          $ cat sample.in | parse.exe
//          $ cat sample.in | parse.exe | grep -v found
//          $ echo "15 * 3 + 6" | parse.exe
//          $ echo "(15 + total) / 6" | parse.exe
//*****************************************************************************
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

#include "lexer.h"

extern "C"
{
// Instantiate global variables
extern FILE *yyin;         // input stream
extern FILE *yyout;        // output stream
extern int   yyleng;       // length of current lexeme
extern char *yytext;       // text of current lexeme
extern int   yylex();      // the generated lexical analyzer
}

// Needed global variables
static int token = 0;       // the current token
static int level = 0;       // the current parse tree depth

// Production functions
void E( void );
void T( void );
void F( void );

// Function declarations for checking whether the current token is
// in the first set of each production rule.
bool IsFirstOfE( void );
bool IsFirstOfT( void );
bool IsFirstOfF( void );

// Function to help print the tree nesting
string psp(void) { // Stands for p-space, but I want the name short
  string str("");
  for(int i = 0; i < level; i++)
    str += "| ";
  return str;
}
// Function to output lexemes
void found(string what) {
  cout << psp() << "found |" << yytext << "| " << what << endl;
}
//*****************************************************************************
// The main processing loop
int main( int argc, char* argv[] )
{
  // Set the input and output streams
  yyin = stdin;
  yyout = stdout;

  // Get the first token
  token = yylex();
  
  // Fire up the parser!
  try
  {
    if( !IsFirstOfE() ) // Check for ID | INTLIT | (
      throw "first token not 'ID' | 'INTLIT' | '('";

    // Process E Production
    E(); 

    if( token != TOK_EOF )
      throw "end of file expected, but there is more here!";
  }
  catch( char const *errmsg )    
  {
    cerr << endl << "***ERROR: " << errmsg << endl;
    return 1;
  }

  cout << endl << "=== GO BULLDOGS! Your parse was successful! ===" << endl;
  return 0;
}

//*****************************************************************************
// E --> T { ( + | - ) T }
void E( void )
{
  static int Ecnt = 1; // Count the number of E's
  int CurEcnt = Ecnt++;
  char const *Terr = 
    "term does not start with 'ID' | 'INTLIT' | '('";

  cout << psp() << "enter E " << CurEcnt << endl;
  ++level;

  // We next expect to see a T
  if( IsFirstOfT() ) 
    T();
  else
    throw Terr;

  // As long as the next token is + or -, keep parsing T's
  while( token == TOK_PLUS || token == TOK_MINUS )
  {
    found("+ or -");
    token = yylex();
    if( IsFirstOfT() )
      T();
    else
      throw Terr;
  }
  --level;
  cout << psp() << "exit E " << CurEcnt << endl;
}

//*****************************************************************************
// T --> F { ( * | / ) F }
void T( void )
{
  static int Tcnt = 1; // Count the number of T's
  int CurTcnt = Tcnt++;
  char const *Ferr = 
    "factor does not start with 'ID' | 'INTLIT' | '('";

  cout << psp() << "enter T " << CurTcnt << endl;
  ++level;

  // We next expect to see a F
  if( IsFirstOfF() )
    F();
  else
    throw Ferr;
  
  // As long as the next token is * or /, keep parsing F's
  while( token == TOK_MULTIPLY || token == TOK_DIVIDE )
  {
    found("* or /");
    token = yylex();
    if( IsFirstOfF() )
      F();
    else
      throw Ferr;
  }
  --level;
  cout << psp() << "exit T " << CurTcnt << endl;
}

//*****************************************************************************
// F --> ID | INTLIT | (
void F( void )
{
  static int Fcnt = 1; // Count the number of F's
  int CurFcnt = Fcnt++;

  cout << psp() << "enter F " << CurFcnt << endl;
  ++level;

  // Determine what token we have
  switch( token )
  {
  case TOK_IDENTIFIER:
    found("ID");
    token = yylex(); // Read past what we have found
    break;
    
  case TOK_INTLIT:
    found("INTLIT");
    token = yylex(); // Read past what we have found
    break;

  case TOK_OPENPAREN:
    // We expect (E); parse it
    found("OPENPAREN");
    token = yylex();
    if( !IsFirstOfE() ) // Check for ID | INTLIT | (
      throw "first token not 'ID' | 'INTLIT' | '('";
    E();
    if( token == TOK_CLOSEPAREN ) {
      found("CLOSEPAREN");
      token = yylex();
    }
    else
      throw "E does not end with )";
    break;

  default:
    // If we made it to here, syntax error
    throw "factor does not start with 'ID' | 'INTLIT' | '('";
  }
  --level;
  cout << psp() << "exit F " << CurFcnt << endl;
}

//*****************************************************************************
bool IsFirstOfE( void )
{
  return token == TOK_IDENTIFIER || token == TOK_INTLIT || token == TOK_OPENPAREN;
}
//*****************************************************************************
bool IsFirstOfT( void )
{
  return token == TOK_IDENTIFIER || token == TOK_INTLIT || token == TOK_OPENPAREN;
}
//*****************************************************************************
bool IsFirstOfF( void )
{
  return token == TOK_IDENTIFIER || token == TOK_INTLIT || token == TOK_OPENPAREN;
}

