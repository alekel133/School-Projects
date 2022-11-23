/******************************************************************* 
Name: Alexander Kellough        NetID: atk133
Class: CSE 4714                 Assignment: Part 3
Programming Enviornment: Vim, Flex, Make, G++
Purpose of File: Contains rule definitions for Flex.
*******************************************************************/
%option yylineno
%option noyywrap
%{
#include "lexer.h"

%}

INTEGER   [[:digit:]]+
LETTER    [[:upper:]]
CHARACTER [[:upper:][:digit:]]
STRING    [.{0,80}]

%%
[{].*[}]
[\n\t\r ]+
{INTEGER}[.]{INTEGER}   { return TOK_FLOATLIT; }
{INTEGER}   { return TOK_INTLIT; }
"'"[^'\n]*"'"     { if(yyleng > 81){return TOK_UNKNOWN;} return TOK_STRINGLIT; }
"BEGIN"    { return TOK_BEGIN; }
"BREAK"    { return TOK_BREAK; }
"CONTINUE"    { return TOK_CONTINUE; }
"DOWNTO"    { return TOK_DOWNTO; }
"ELSE"    { return TOK_ELSE; }
"END"    { return TOK_END; }
"FOR"    { return TOK_FOR; }
"IF"    { return TOK_IF; }
"LET"    { return TOK_LET; }
"PROGRAM"    { return TOK_PROGRAM; }
"READ"    { return TOK_READ; }
"THEN"    { return TOK_THEN; }
"TO"    { return TOK_TO; }
"VAR"    { return TOK_VAR; }
"WHILE"    { return TOK_WHILE; }
"WRITE"    { return TOK_WRITE; }
"INTEGER"    { return TOK_INTEGER; }
"REAL"    { return TOK_REAL; }
";"    { return TOK_SEMICOLON; }
":"    { return TOK_COLON; }
"("    { return TOK_OPENPAREN; }
")"    { return TOK_CLOSEPAREN; }
"+"    { return TOK_PLUS; }
"-"    { return TOK_MINUS; }
"*"    { return TOK_MULTIPLY; }
"/"    { return TOK_DIVIDE; }
":="    { return TOK_ASSIGN; }
"="    { return TOK_EQUALTO; }
"<"    { return TOK_LESSTHAN; }
">"    { return TOK_GREATERTHAN; }
"<>"    { return TOK_NOTEQUALTO; }
"MOD"    { return TOK_MOD; }
"NOT"    { return TOK_NOT; }
"AND"    { return TOK_AND; }
"OR"    { return TOK_OR; }
{LETTER}{CHARACTER}*   { if(yyleng > 8){return TOK_UNKNOWN;}return TOK_IDENT; }

 /* Found an unknown character */

.         { return TOK_UNKNOWN; }

 /* Recognize end of file */

<<EOF>>   { return TOK_EOF; }

