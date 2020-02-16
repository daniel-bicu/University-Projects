%{
#include <stdio.h>
#include "y.tab.h"
void extern yyerror(char*);
void InvalidToken();

%}
%option noyywrap
numar -?[1-9][0-9]*|0
character [a-zA-z@#$%^&*_]
%%
"/*"(.|\n)+"*/" ;
"_"[a-zA-Z][a-zA-Z0-9\-]* {yylval.tipData = strdup(yytext); return ID;}
"bool"|"int"|"float"|"char"|"string"|"array[]"|"array["{numar}"]"|"[module]" {yylval.tipData =  strdup(yytext);return TIP;}
"<"|">"|"<="|">="|"==="|"!==" {return COMP;}
"Eval" {return EVAL;}
"module" {return MODULE;}
"constructor" {return CONSTRUCTOR;}
"state" {return STATE;}
"handlers" {return HANDLERS;}
"action" {return FUNC;}
"var" {return VAR;}
"if" {return IF;}
"else" {return ELSE;}
"for" {return FOR;}
"while" {return WHILE;}
"@var" {return CONST_VAR;}
"&&"|"||" {return BOOL_OPERATION;}
[+\-*/] {return NR_OPERATION;}
":=" {return ASSIGN;}
\'{character}\' {return CHAR;}
\"{character}+\" {return STRING;}
"true"|"false" {return BOOL;}
{numar} {yylval.integer = atoi(yytext);return INT;}
{numar}\.[0-9]+ {yylval.floatV = atof(yytext); return FLOAT;}
[ \t] ;
\n {yylineno++;}
. { return yytext[0];}

 
