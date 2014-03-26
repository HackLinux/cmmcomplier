
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

{INT}		{ printf("Integer %d ", atoi(yytext)); }

{FLOAT}		{ printf("Float %f ", atof(yytext)); }

{SEMI}		{ printf("SEMI \"%s\" ", yytext);	}
{COMMA}		{ printf("COMMA \"%s\" ", yytext); }								
{ASSIGNOP}	{ printf("ASSIGNOP \"%s\" ", yytext);	}
{RELOP}		{ printf("RELOP \"%s\" ", yytext); }
{PLUS}		{ printf("PLUS \"%s\" ", yytext); }
{MINUS}		{ printf("MINUS \"%s\" ", yytext); }
{STAR}		{ printf("STAR \"%s\" ", yytext); }
{DIV}		{ printf("DIV \"%s\" ", yytext); }
{AND}		{ printf("AND \"%s\" ", yytext); }
{OR}		{ printf("OR \"%s\" ", yytext); }
{DOT}		{ printf("DOT \"%s\" ", yytext); }
{NOT}		{ printf("NOT \"%s\" ", yytext); }
{TYPE}		{ printf("TYPE \"%s\" ", yytext); }
{LP}		{ printf("LP \"%s\" ", yytext); }
{RP}		{ printf("RP \"%s\" ", yytext); }
{LB}		{ printf("LB \"%s\" ", yytext); }
{RB}		{ printf("RB \"%s\" ", yytext); }
{LC}		{ printf("LC \"%s\" ", yytext); }
{RC}		{ printf("RC \"%s\" ", yytext); }
{STRUCT}	{ printf("STRUCT \"%s\" ", yytext); }
{RETURN}	{ printf("RETURN \"%s\" ", yytext); }
{IF}		{ printf("IF \"%s\" ", yytext); }
{ELSE}		{ printf("ELSE \"%s\" ", yytext); }
{WHILE}		{ printf("WHILE \"%s\" ", yytext); }

{ID}		{ printf("ID \"%s\" ", yytext); }

{ws}		{ ; }
{enter}		{ printf("End of line %d\n", yylineno - 1); }


.			{ printf("Error Type 1 at line %d: Undefined Characters \"%s\"", yylineno, yytext); }

%%
	/*copy to tail of lexical.c*/

int main(int argc, char** argv){

	if( argc > 1){
		if(!(yyin = fopen(argv[1], "r"))){
			perror(argv[1]);
			return 1;
		}
	}

	yylex();

	printf("Scanner finished!");

	return 0;
}
