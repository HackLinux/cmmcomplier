
%{
	#include <string.h>
	#include "syntax.h"
	#include "tree.h"
	#include "bool.h"

	int yycolumn = 1;

	#define YY_USER_ACTION	yylloc.first_line = yylloc.last_line = yylineno; \
		yylloc.first_column = yycolumn; yylloc.last_column = yycolumn + yyleng - 1; \
		yycolumn += yyleng;

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

{INT}		{ 	yylval.pnode = create_int_node(yylineno, "INT", atoi(yytext));
				return INT;
			}
{FLOAT}		{ 	yylval.pnode = create_float_node(yylineno, "FLOAT", atof(yytext));
				return FLOAT; 
			}
{SEMI}		{ 	yylval.pnode = create_node(true, yylineno, "SEMI");
				return SEMI; 
			}
{COMMA}		{ 	yylval.pnode = create_node(true, yylineno, "COMMA");
				return COMMA; 
			}								
{ASSIGNOP}	{ 	yylval.pnode = create_node(true, yylineno, "ASSIGNOP");
				return ASSIGNOP; 
			
			}
{RELOP}		{ 	yylval.pnode = create_node(true, yylineno, "RELOP");
				return RELOP; 
			}
{PLUS}		{ 	yylval.pnode = create_node(true, yylineno, "PLUS");
				return PLUS; 
			}
{MINUS}		{ 	yylval.pnode = create_node(true, yylineno, "MINUS");
				return MINUS; 
			}
{STAR}		{ 	yylval.pnode = create_node(true, yylineno, "STAR");
				return STAR; 
			}
{DIV}		{ 	yylval.pnode = create_node(true, yylineno, "DIV");
				return DIV; 
			}
{AND}		{ 	yylval.pnode = create_node(true, yylineno, "AND");
				return AND; 
			}
{OR}		{ 	yylval.pnode = create_node(true, yylineno, "OR");
				return OR; 
			}
{DOT}		{ 	yylval.pnode = create_node(true, yylineno, "DOT");
				return DOT; 
			}
{NOT}		{ 	yylval.pnode = create_node(true, yylineno, "NOT");
				return NOT; 
			}
{TYPE}		{ 	yylval.pnode = create_type_node(yylineno, "TYPE", yytext);
				return TYPE; 
			}
{LP}		{ 	yylval.pnode = create_node(true, yylineno, "LP");
				return LP; 
			}
{RP}		{ 	yylval.pnode = create_node(true, yylineno, "RP");
				return RP; 
			}
{LB}		{ 	yylval.pnode = create_node(true, yylineno, "LB");
				return LB; 
			}
{RB}		{ 	yylval.pnode = create_node(true, yylineno, "RB");
				return RB; 
			}
{LC}		{ 	yylval.pnode = create_node(true, yylineno, "LC");
				return LC; 
			}
{RC}		{ 	yylval.pnode = create_node(true, yylineno, "RC");
				return RC; 
			}
{STRUCT}	{ 	yylval.pnode = create_node(true, yylineno, "STRUCT");
				return STRUCT; 
			}
{RETURN}	{ 	yylval.pnode = create_node(true, yylineno, "RETURN");
				return RETURN; 
			}
{IF}		{ 	yylval.pnode = create_node(true, yylineno, "IF");
				return IF; 
			}
{ELSE}		{ 	yylval.pnode = create_node(true, yylineno, "ELSE");
				return ELSE; 
			}
{WHILE}		{ 	yylval.pnode = create_node(true, yylineno, "WHILE");
				return WHILE; 
			}
{ID}		{ 	if(strlen(yytext) >= 20)
					 printf("Error type 1 at line %d column %d: ID '%s' too long.\n", yylineno, yycolumn, yytext); 
				yylval.pnode = create_id_node(yylineno, "ID", yytext);
				return ID; 
			}
{ws}		{ ; }
{enter}		{ yycolumn = 1; }

.			{ printf("Error type 1 at line %d column %d: Mysterious character '%s'\n", yylineno, yycolumn, yytext); }

%%

