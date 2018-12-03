%{
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "inter.tab.h"
void yyerror(const char*);
%}

%%

-?[1-9][0-9]* { yylval.iValue = atoi(yytext); yylloc.first_column = yylloc.last_column;          yylloc.last_column += strlen (yytext); return NUM;}
[1-9][0-9]*u|0u { yylval.iValue = atoi(yytext);         yylloc.first_column = yylloc.last_column;          yylloc.last_column += strlen (yytext); return UNUM; }
"signed" {             yylloc.first_column = yylloc.last_column;               yylloc.last_column += strlen (yytext);   return SIGNED;}
"unsigned" {              yylloc.first_column = yylloc.last_column;               yylloc.last_column += strlen (yytext);return UNSIGNED;}
"const" { yylloc.first_column = yylloc.last_column;               yylloc.last_column += strlen (yytext);        return CONST;}
"cell" { yylloc.first_column = yylloc.last_column;               yylloc.last_column += strlen (yytext);        return CELL;}
"->" { yylloc.first_column = yylloc.last_column;               yylloc.last_column += strlen (yytext);        return CONV;}
"<-" { yylloc.first_column = yylloc.last_column;               yylloc.last_column += strlen (yytext);        return ASSIG;}
"testrep" { yylloc.first_column = yylloc.last_column;               yylloc.last_column += strlen (yytext);        return WHILE;}
"testonce" { yylloc.first_column = yylloc.last_column;               yylloc.last_column += strlen (yytext);        return IF;}
"func" { yylloc.first_column = yylloc.last_column;               yylloc.last_column += strlen (yytext);        return FUNC;}
"call" { yylloc.first_column = yylloc.last_column;               yylloc.last_column += strlen (yytext);        return CALL;}
"xray" { yylloc.first_column = yylloc.last_column;               yylloc.last_column += strlen (yytext);        return XRAY;}
"top" { yylloc.first_column = yylloc.last_column;               yylloc.last_column += strlen (yytext);        return TOP;}
"bottom" { yylloc.first_column = yylloc.last_column;               yylloc.last_column += strlen (yytext);        return BOT;}
"left" { yylloc.first_column = yylloc.last_column;               yylloc.last_column += strlen (yytext);        return LEFT;}
"right" { yylloc.first_column = yylloc.last_column;               yylloc.last_column += strlen (yytext);        return RIGHT;}
"ntop" { yylloc.first_column = yylloc.last_column;               yylloc.last_column += strlen (yytext);        return NTOP;}
"nbottom" { yylloc.first_column = yylloc.last_column;               yylloc.last_column += strlen (yytext);        return NBOT;}
"nleft" { yylloc.first_column = yylloc.last_column;               yylloc.last_column += strlen (yytext);        return NLEFT;}
"nright" { yylloc.first_column = yylloc.last_column;               yylloc.last_column += strlen (yytext);        return NRIGHT;}
"matrix" { yylloc.first_column = yylloc.last_column;               yylloc.last_column += strlen (yytext);        return ARR;}
[a-zA-Z][0-9a-zA-Z_]* {
	yylval.iStr = (char *) malloc(sizeof(char)*strlen(yytext)+1);
     yylloc.first_column = yylloc.last_column + 1;
     yylloc.last_column += strlen (yytext);
	strcpy(yylval.iStr, yytext); 
	return NAME;
}
[\(\)=><\+\-\*\/%#\,\;] {yylloc.first_column = ++yylloc.last_column; return *yytext;}

[ \t]       {               yylloc.first_column = ++yylloc.last_column; }
\n          {yylloc.last_line=++yylloc.first_line;               yylloc.last_column = yylloc.first_column = 0;              }
 . yyerror("Unknown character"); 

%%

int yywrap(void) {return 1;}
