/*********************************************************************
  Name: Alexander Kellough           NetID: atk133
  Course: CSE 4714              
  Lab: 01 - Lexical Analyzer 
  Purpose of File: Contains the rules/regular expressions used
  by flex to generate a lexical anlyzer..

**********************************************************************/

%option noyywrap
%{
#include "lexer.h"

// global variable to hold current line number being read
int line_number = 1;
%}

DIGIT   [[:digit:]?]
YEAR    {DIGIT}{DIGIT}{DIGIT}{DIGIT}
DATE    {DIGIT}{DIGIT}

%%


 /* Eat any whitespace, except newlines*/

[ \t\r]+             /* nop */

 /* When a newline is found, increment line number */

[\n]    { ++line_number;} 

 /* Regulear Expression for yyyy/mm/dd date. */

{YEAR}[/]{DATE}[/]{DATE}    { return DATE; }

 /* Comma returns Seperator */

","     { return SEPARATOR; }

 /* Yes and no regular expressions */

[yY]+"es"   { return YES; }

[nN]+"o"    { return NO; }


 /* Lab result status regular expressions */

[uU]+"nknown"   { return UNKNOWN_VALUE; }

[mM]+"issing"   { return MISSING; }

[lL]+"aboratory-confirmed "+[cC]+"ase"  { return LABORATORY; }

[pP]+"robable "+[cC]+"ase"      { return PROBABLE; }

 /* Gender Regular expressions */

[mM]+"ale"      { return MALE; }

[fF]+"emale"    { return FEMALE; }

[oO]+"ther"     { return OTHER; }

 /* Age group regular expression */

"0 - 9 "+[yY]+"ears"     { return AGE_0X; }

"10 - 19 "+[yY]+"ears"     { return AGE_1X; }

"20 - 39 "+[yY]+"ears"     { return AGE_2X; }

"40 - 49 "+[yY]+"ears"    { return AGE_4X; }

"50 - 59 "+[yY]+"ears"     { return AGE_5X; }

"60 - 69 "+[yY]+"ears"     { return AGE_6X; }

"70 - 79 "+[yY]+"ears"     { return AGE_7X; }

"80+ "+[yY]+"ears"       { return AGE_8X; }

 /* Ethnicity / Race Regular Expressions */

["]+[hH]+"ispanic/Latino"+["]   { return HISPANIC; }

["]+[aA]+"merican Indian / Alaska Native, Non-Hispanic"+["]      { return NATIVE_AMERICAN; }

["]+[aA]+"sian, Non-Hispanic"+["]      { return ASIAN; }

["]+[bB]+"lack, Non-Hispanic"+["]      { return BLACK; }

["]+[nN]+"ative Hawaiian / Other Pacific Islander, Non-Hispanic"+["]      { return PACIFIC_ISLANDER; }

["]+[wW]+"hite, Non-Hispanic"+["]      { return WHITE; }

["]+[mM]+"ultiple/Other, Non-Hispanic"+["]      { return MULTIPLE_OTHER; }

 /* Found an unknown character */

.         { return UNKNOWN_TOKEN; }

 /* Recognize end of file */

<<EOF>>   { return EOF_TOKEN; }


%%
