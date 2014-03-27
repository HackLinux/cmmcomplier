
%{
	#include "syntax.h"

	int yycolumn = 1;

	/*#define YY_USER_ACTION	yylloc.first_line = yylloc.last_line = yylineno; \
		yylloc.first_column = yycolumn; yylloc.last_column = yycolumn + yyleng - 1; \
		yycolumn += yyleng;*/

%}

%option yylineno

digit 		[0-9]
letter_ 	[_a-zA-Z]
ws 			[ \t]+
enter		[\n]+

INT 		0|[1-9]{digit}*
FLOAT 		{digit}+\.{digit}+	
ID 			{letter_}({letter_}|digit)*	
SEMI 		";"
COMMA 		","
ASSIGNOP 	"="
RELOP		>|<|>=|<=|==|!=
PLUS 		"+"
MINUS		"-"
STAR		"*"
DIV			"/"
AND			"&&"
OR			"||"
DOT 		"."
NOT 		"!"
TYPE		int|float
LP 			"("
RP 			")"
LB 			"["
RB 			"]"
LC 			"{"
RC			"}"
STRUCT 		struct
RETURN 		return
IF 			if
ELSE 		else
WHILE 		while

%%

{INT}		{ 
				//yylval = atoi(yytext);
				//yylloc.first_column = 1;
				return INT;
	 		}

{FLOAT}		{ return FLOAT; }

{SEMI}		{ return SEMI; }
{COMMA}		{ return COMMA; }								
{ASSIGNOP}	{ return ASSIGNOP; }
{RELOP}		{ return RELOP; }
{PLUS}		{ return PLUS; }
{MINUS}		{ return MINUS; }
{STAR}		{ return STAR; }
{DIV}		{ return DIV; }
{AND}		{ return AND; }
{OR}		{ return OR; }
{DOT}		{ return DOT; }
{NOT}		{ return NOT; }
{TYPE}		{ return TYPE; }
{LP}		{ return LP; }
{RP}		{ return RP; }
{LB}		{ return LB; }
{RB}		{ return RB; }
{LC}		{ return LC; }
{RC}		{ return RC; }
{STRUCT}	{ return STRUCT; }
{RETURN}	{ return RETURN; }
{IF}		{ return IF; }
{ELSE}		{ return ELSE; }
{WHILE}		{ return WHILE; }

{ID}		{ return ID; }

{ws}		{ ; }
{enter}		{ yycolumn = 1; }

.			{ printf("Error type 1 at line %d column %d: Mysterious character '%s'\n", yylineno, yycolumn, yytext); }

%%

