%{
#include <stdio.h>
#include <stdlib.h>
#include "inter.h"
int yylex(void);
void yyerror(const char *s);
extern FILE *yyin;
%}
%union{
	int iValue;
	char *iStr;
};

%token NUM
%token UNUM
%token NAME
%token SIGNED
%token UNSIGNED
%token CONST
%token CELL
%right ASSIG
%token WHILE
%token FUNC
%token CALL
%token IF
%token XRAY
%token TOP
%token BOT
%token LEFT
%token RIGHT
%token NTOP
%token NBOT
%token NLEFT
%token NRIGHT
%token ARR
%left '>' '<' CONV '='
%left '+' '-' '%' 
%left '*' '/'

%%
program:
     func program 				{ }

	|
;

func:	FUNC NAME '(' decl_arg ')' '(' text expr';' ')' 
                                    {Funcc($<iStr>2);}
;

decl_arg:                           {Newbody(); }

	| type NAME add_arg 			{ FArg($<iStr>2);	}
;

add_arg:
    ',' type NAME add_arg           { FArg($<iStr>2);	}

    |                           	{ Newbody();}
;

is_const:                       	{NConst();}

    | CONST                     	{YConst();}
;

type_name:
    CELL                        	{Celll();	}

    | digit                         {	}
;

digit:	SIGNED                  	{Signedd();	}

    | UNSIGNED                      {Unsignedd();	}
;

type:	 is_const type_name			{	}	
;

text:   text line       			{	}

    |                               {	}
;

line:	';'         				
	
    | adress ASSIG expr ';'			{Assigg();	}

    | action ';'                    {   }

    | variable ';'		        	{ }

    | WHILE cycle_cond '('text')'   {Whilee();	}


    | IF '(' condition ')' '('text')'
                                    {  Iff();	}
	
;

cycle_cond:
    is_op_br condition is_clbr
                                    {           }
;

is_op_br:
    '('                             {           }
    |')'                            {fprintf(stderr, "\nUnexpected \")\" Eexpected \"(\"  l:%d c:%d - l:%d c: %d\n\n",@0.first_line, @0.first_column, @0.last_line, @0.last_column); execute_flag = 0;  }
    |';'                            {fprintf(stderr, "\nUnexpected \";\", Expected \"(\" l:%d c:%d - l:%d c: %d\n\n",@1.first_line, @1.first_column, @1.last_line, @1.last_column); execute_flag = 0;}
;

is_clbr:
    ')'                             {           }
    |';'                            {fprintf(stderr, "\nUnexpected \";\", Expected \")\" l:%d c:%d - l:%d c: %d\n\n",@1.first_line, @1.first_column, @1.last_line, @1.last_column); execute_flag = 0;}
;


action:	TOP                     	{Gdir(0);	}

    | BOT                       	{Gdir(2);	}

    | LEFT                      	{Gdir(1);	}

    | RIGHT                        	{Gdir(3);	}
;


variable:
	ARR type NAME'('UNUM','UNUM')' is_init	
                                    {vArr($<iStr>3, $<iValue>5, $<iValue>7, $<iValue>9);	}

    | type NAME ASSIG expr			{Var( $<iStr>2);}
	
;

is_init:
    ASSIG XRAY                  	{$<iValue>$ = 1;}

    |                               {$<iValue>$ = 0;}
;

condition:
    lexpr                       	{Cond();	}

    | expr                          {Cond();	}
;

lexpr:	query '('adress')'			{Queryy();	}

    | lexpr '/' lexpr               {Lop(8);}

    | lexpr '%' lexpr               {Lop(9);}
	
    | lexpr '-' lexpr               {Lop(10);}

    | '(' lexpr ')'                 {Lbr();}
;


query:	TOP                     	{Lwhere(0);	}

    | NTOP              			{Lwhere(1);	}

    | BOT                           {Lwhere(4); }

    | NBOT                  		{Lwhere(5);	}
	
    | LEFT          				{Lwhere(2);	}

    | NLEFT                     	{Lwhere(3);	}

    | RIGHT                     	{Lwhere(6);	}

    | NRIGHT            			{Lwhere(7);	}

;

adress:

    NAME                            {EName($<iStr>1);}

    | NAME '('index','index')'      { EArrr($<iStr>1);	}

;


index:

    NAME                            {IName($<iStr>1);}

    | UNUM                      	{INum($<iValue>1);}

;

cnv:	CONV type_name				{$<iValue>$ = 1;}

    |                               {$<iValue>$ = 0;}

;
expr:	adress 	cnv                 {Iscnv($<iValue>2);		}

    | XRAY                          {Xrayy();	}

    | CALL NAME '('call_arg')'		{Call($<iStr>2);}

    | NUM                       	{Num($<iValue>1);}

    | UNUM                          {Unum($<iValue>1);}

    | expr '+' expr         		{Pl();}

    | expr '-' expr         		{Min();		}

    | expr '/' expr         		{Dv();
                                     if(!execute_flag)
                                         fprintf(stderr, "\nDiv by zero in l:%d c:%d - l:%d c: %d\n\n",@3.first_line, @3.first_column, @3.last_line, @3.last_column);	}

    | expr '*' expr         		{Ml();}


    | '-' expr                  	{Imin();	}

    | expr '<' expr         		{Lss();}

    | expr '>' expr         		{Mr();	}

    | expr '=' expr                 {Eq();		}

    | '(' expr ')'          		{Br();}
;

call_arg:	

    arg add_call_arg                {       }

    |',' arg add_call_arg          	{ fprintf(stderr, "\nUnexpected \",\"  l:%d c:%d - l:%d c: %d\n\n",@1.first_line, @1.first_column, @1.last_line, @1.last_column);
                                      execute_flag = 0;}


    |                               {       }
;

add_call_arg:
    ',' arg add_call_arg            {       }
    |                               {       }

;

arg:
    NAME                            {AName($<iStr>1);}

    |NUM                            {ANum($<iValue>1);}

    |UNUM                           {AUNum($<iValue>1);}

;

%%
void yyerror(const char*s){
	fprintf(stderr,"%s\n",s);
}

int main(void) {
	yyin = fopen("program.txt","r");
    yyparse();
    cout<<"\n"<<execute_flag<<"\n";
    if(execute_flag)
        starts();
	return 0;
}


