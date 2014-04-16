%{
	#include <string.h>
	#include "syntax.h"
	#include "tree.h"
	#include "bool.h"
	#include "math.h"

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
OCT			0[0-7]+
HEX			(0x|0X)[0-9a-fA-F]+
FLOAT 		{digit}*\.{digit}+|{digit}+\.
SCI			({FLOAT}|{digit}+)[Ee][+-]?{digit}+	
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
{OCT}		{	yylval.pnode = create_int_node(yylineno, "INT", atoi_oct(yytext));
				return INT;
			}
{HEX}		{	yylval.pnode = create_int_node(yylineno, "INT", atoi_hex(yytext));
				return INT;
			}
{FLOAT}		{ 	yylval.pnode = create_float_node(yylineno, "FLOAT", atof(yytext));
				return FLOAT; 
			}
{SCI}		{	yylval.pnode = create_float_node(yylineno, "FLOAT", atof_sci(yytext));
				return FLOAT; 
			}
{SEMI}		{ 	yylval.pnode = create_node(yylineno, "SEMI");
				return SEMI; 
			}
{COMMA}		{ 	yylval.pnode = create_node(yylineno, "COMMA");
				return COMMA; 
			}								
{ASSIGNOP}	{ 	yylval.pnode = create_node(yylineno, "ASSIGNOP");
				return ASSIGNOP; 
			}
{RELOP}		{ 	yylval.pnode = create_node(yylineno, "RELOP");
				return RELOP; 
			}
{PLUS}		{ 	yylval.pnode = create_node(yylineno, "PLUS");
				return PLUS; 
			}
{MINUS}		{ 	yylval.pnode = create_node(yylineno, "MINUS");
				return MINUS; 
			}
{STAR}		{ 	yylval.pnode = create_node(yylineno, "STAR");
				return STAR; 
			}
{DIV}		{ 	yylval.pnode = create_node(yylineno, "DIV");
				return DIV; 
			}
{AND}		{ 	yylval.pnode = create_node(yylineno, "AND");
				return AND; 
			}
{OR}		{ 	yylval.pnode = create_node(yylineno, "OR");
				return OR; 
			}
{DOT}		{ 	yylval.pnode = create_node(yylineno, "DOT");
				return DOT; 
			}
{NOT}		{ 	yylval.pnode = create_node(yylineno, "NOT");
				return NOT; 
			}
{TYPE}		{ 	yylval.pnode = create_type_node(yylineno, "TYPE", yytext);
				return TYPE; 
			}
{LP}		{ 	yylval.pnode = create_node(yylineno, "LP");
				return LP; 
			}
{RP}		{ 	yylval.pnode = create_node(yylineno, "RP");
				return RP; 
			}
{LB}		{ 	yylval.pnode = create_node(yylineno, "LB");
				return LB; 
			}
{RB}		{ 	yylval.pnode = create_node(yylineno, "RB");
				return RB; 
			}
{LC}		{ 	yylval.pnode = create_node(yylineno, "LC");
				return LC; 
			}
{RC}		{ 	yylval.pnode = create_node(yylineno, "RC");
				return RC; 
			}
{STRUCT}	{ 	yylval.pnode = create_node(yylineno, "STRUCT");
				return STRUCT; 
			}
{RETURN}	{ 	yylval.pnode = create_node(yylineno, "RETURN");
				return RETURN; 
			}
{IF}		{ 	yylval.pnode = create_node(yylineno, "IF");
				return IF; 
			}
{ELSE}		{ 	yylval.pnode = create_node(yylineno, "ELSE");
				return ELSE; 
			}
{WHILE}		{ 	yylval.pnode = create_node(yylineno, "WHILE");
				return WHILE; 
			}
{ID}		{ 	if(strlen(yytext) >= 20){
					 printf("Error type 1 at line %d: ID '%s' too long.\n", yylineno, yytext);
					 error_flag = true;
				} 
				yylval.pnode = create_id_node(yylineno, "ID", yytext);
				return ID;
			}
{ws}		{ ; }
{enter}		{ yycolumn = 1; }

"//"		{	char c = input();
				while(c != '\n')
					c = input();

			}
"/*"		{	char prev;
				char curr = input();
				while(true){
					prev = curr;
					curr = input();
					if( prev == '*' && curr == '/')
						break;
					if( prev == '/' && curr == '*'){
						 printf("Error type 1 at line %d: Nested comment\n", yylineno);
						error_flag = true;
					}
					if( curr == EOF ){
						 printf("Error type 1 at line %d: Unclosed comment\n", yylineno);
						 error_flag = true;
						 break;
					}
				}
			}
"*/"		{	printf("Error type 1 at line %d: Mismatch comment\n", yylineno); 
				error_flag = true;
			}
.			{ 	printf("Error type 1 at line %d: Mysterious character '%s'\n", yylineno, yytext); 
				error_flag = true;
			}

%%

