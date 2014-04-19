
%{
	#include <stdio.h>
	#include "tree.h"
	#include "table.h"

%}

/* declared types */
%union{
	struct tree_node* pnode;
}

/* declared tokens */
%token <pnode> INT
%token <pnode> FLOAT
%token <pnode> ID
%token <pnode> SEMI COMMA 
%token <pnode> ASSIGNOP RELOP
%token <pnode> PLUS MINUS STAR DIV AND OR DOT NOT
%token <pnode> TYPE
%token <pnode> LP RP LB RB LC RC
%token <pnode> STRUCT RETURN IF ELSE WHILE

/* declared non-terminals */
%type <pnode> Program ExtDefList ExtDef ExtDecList
%type <pnode> Specifier StructSpecifier OptTag Tag
%type <pnode> VarDec FunDec VarList ParamDec
%type <pnode> CompSt StmtList Stmt
%type <pnode> DefList Def DecList Dec
%type <pnode> Exp Args

/* priority and associativity*/

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%left COMMA
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left DOT LP RP LB RB

%nonassoc LOWER_THAN_ERROR
%nonassoc error

%% 

Program		:	ExtDefList			{	$$ = build_a_production(@$.first_line, "Program", 1, $1);
										if(!error_flag){
											//print_syntax_tree($$, 0);
											printf("no syntax error\n");
											preorder_analyze($$);
											print_func_table(func_table_head);
											print_struct_table(struct_table_head);
											print_var_table(var_table_head);
											destroy_tree($$);
										}
									}
ExtDefList	:	ExtDef ExtDefList	{	$$ = build_a_production(@$.first_line, "ExtDefList", 2, $1, $2);
									}
			|	/*empty*/			{	$$ = build_a_production(@$.first_line, "ExtDefList", 0);
									}
			;	
ExtDef 		:	Specifier ExtDecList SEMI	{	$$ = build_a_production(@$.first_line, "ExtDef", 3, $1, $2, $3);
											}
			|	Specifier SEMI				{	$$ = build_a_production(@$.first_line, "ExtDef", 2, $1, $2);
											}
			|	Specifier FunDec CompSt		{	$$ = build_a_production(@$.first_line, "ExtDef", 3, $1, $2, $3);
											}
			| 	error SEMI 					{ 	//printf("get error ExtDef at %d\n", @1.first_line);
												error_flag = true;}
			;
ExtDecList	:	VarDec						{	$$ = build_a_production(@$.first_line, "ExtDecList", 1, $1);
											}
			|	VarDec COMMA ExtDecList		{	$$ = build_a_production(@$.first_line, "ExtDecList", 3, $1, $2, $3);
											}
			;


Specifier 		:	TYPE 							{	$$ = build_a_production(@$.first_line, "Specifier", 1, $1);
													}
				|	StructSpecifier					{	$$ = build_a_production(@$.first_line, "Specifier", 1, $1);
													}
				;
StructSpecifier	:	STRUCT OptTag LC DefList RC		{	$$ = build_a_production(@$.first_line, "StructSpecifier", 5, $1, $2, $3, $4, $5);
													}
				|	STRUCT Tag 						{	$$ = build_a_production(@$.first_line, "StructSpecifier", 2, $1, $2);
													}
				;
OptTag 			:	ID 								{	$$ = build_a_production(@$.first_line, "OptTag", 1, $1);
													}
				|	/*empty*/						{	$$ = build_a_production(@$.first_line, "OptTag", 0);
													}
				;
Tag 			: 	ID 								{	$$ = build_a_production(@$.first_line, "Tag", 1, $1);
													}
				;


VarDec		:	ID 							{	$$ = build_a_production(@$.first_line, "VarDec", 1, $1);
											}
			|	VarDec LB INT RB			{	$$ = build_a_production(@$.first_line, "VarDec", 4, $1, $2, $3, $4);
											}
			;
FunDec		:	ID LP VarList RP 			{	$$ = build_a_production(@$.first_line, "FunDec", 4, $1, $2, $3, $4);
											}
			| 	ID LP RP 					{	$$ = build_a_production(@$.first_line, "FunDec", 3, $1,$2, $3);
											}
			| 	error RP 					{ 	//printf("get error FunDec at %d\n", @1.first_line);
												error_flag = true;
											}
			;
VarList		:	ParamDec COMMA VarList 		{	$$ = build_a_production(@$.first_line, "VarList", 3, $1,$2, $3);
											}
			| 	ParamDec 					{	$$ = build_a_production(@$.first_line, "VarList", 1, $1);
											}
			;
ParamDec 	:	Specifier VarDec			{	$$ = build_a_production(@$.first_line, "ParamDec",2, $1, $2);
											}
			;


CompSt		:	LC DefList StmtList RC 		{	$$ = build_a_production(@$.first_line, "CompSt",4, $1, $2, $3, $4);
											}
			|	error RC					{	//printf("get error CompSt at %d\n", @1.first_line);
												error_flag = true;
											}
			;
StmtList	:	Stmt StmtList 				{	$$ = build_a_production(@$.first_line, "StmtList", 2, $1, $2);
											}
			|	/*empty*/					{	$$ = build_a_production(@$.first_line, "StmtList", 0);
											}
			;
Stmt 		:	Exp SEMI 					{	$$ = build_a_production(@$.first_line, "Stmt", 2, $1, $2);
											}
			|	CompSt						{	$$ = build_a_production(@$.first_line, "Stmt", 1, $1);
											}
			|	RETURN Exp SEMI				{	$$ = build_a_production(@$.first_line, "Stmt", 3, $1, $2, $3);
											}
			|	IF LP Exp RP Stmt 			%prec LOWER_THAN_ELSE
											{	$$ = build_a_production(@$.first_line, "Stmt", 5, $1, $2, $3, $4, $5);
											}
			|	IF LP Exp RP Stmt ELSE Stmt {	$$ = build_a_production(@$.first_line, "Stmt", 7, $1, $2, $3, $4, $5, $6, $7);
											}
			|	WHILE LP Exp RP Stmt 		{	$$ = build_a_production(@$.first_line, "Stmt", 5, $1, $2, $3, $4, $5);
											}
			|	error SEMI					{	//printf("get error Stmt at %d\n", @1.first_line);
												error_flag = true;
											}
			;


DefList :	Def DefList 			{	$$ = build_a_production(@$.first_line, "DefList", 2, $1, $2);
									}
		|	/*empty*/				%prec LOWER_THAN_ERROR
									{	$$ = build_a_production(@$.first_line, "DefList", 0);
									}
		;
Def 	:	Specifier DecList SEMI 	{	$$ = build_a_production(@$.first_line, "Def", 3, $1, $2, $3);
									}
		| 	error SEMI 				{ 	//printf("get error Def at %d\n", @1.first_line);
										error_flag = true;
									}
		;
DecList :	Dec 					{	$$ = build_a_production(@$.first_line, "DecList", 1, $1);
									}
		|	Dec COMMA DecList 		{	$$ = build_a_production(@$.first_line, "DecList", 3, $1, $2, $3);
									}
		;
Dec 	:	VarDec 					{	$$ = build_a_production(@$.first_line, "Dec", 1, $1);
									}
		|	VarDec ASSIGNOP Exp 	{	$$ = build_a_production(@$.first_line, "Dec", 3, $1, $2, $3);
									}
		;


Exp 	:	Exp ASSIGNOP Exp 		{	$$ = build_a_production(@$.first_line, "Exp", 3, $1, $2, $3);
									}
		|	Exp AND Exp 			{	$$ = build_a_production(@$.first_line, "Exp", 3, $1, $2, $3);
									}
		|	Exp OR Exp 				{	$$ = build_a_production(@$.first_line, "Exp", 3, $1, $2, $3);
									}
		|	Exp RELOP Exp 			{	$$ = build_a_production(@$.first_line, "Exp", 3, $1, $2, $3);
									}
		|	Exp PLUS Exp 			{	$$ = build_a_production(@$.first_line, "Exp", 3, $1, $2, $3);
									}
		|	Exp MINUS Exp 			{	$$ = build_a_production(@$.first_line, "Exp", 3, $1, $2, $3);
									}
		|	Exp STAR Exp 			{	$$ = build_a_production(@$.first_line, "Exp", 3, $1, $2, $3);
									}
		|	Exp DIV Exp 			{	$$ = build_a_production(@$.first_line, "Exp", 3, $1, $2, $3);
									}
		|	LP Exp RP  				{	$$ = build_a_production(@$.first_line, "Exp", 3, $1, $2, $3);
									}
		|	MINUS Exp 				{	$$ = build_a_production(@$.first_line, "Exp", 2, $1, $2);
									}
		|	NOT Exp 				{	$$ = build_a_production(@$.first_line, "Exp", 2, $1, $2);
									}
		|	ID LP Args RP 			{	$$ = build_a_production(@$.first_line, "Exp", 4, $1, $2, $3, $4);
									}
		|	ID LP RP 				{	$$ = build_a_production(@$.first_line, "Exp", 3, $1, $2, $3);
									}
		|	Exp LB Exp RB 			{	$$ = build_a_production(@$.first_line, "Exp", 4, $1, $2, $3, $4);
									}
		|	Exp DOT ID 				{	$$ = build_a_production(@$.first_line, "Exp", 3, $1, $2, $3);
									}
		|	ID 						{	$$ = build_a_production(@$.first_line, "Exp", 1, $1);
									}
		|	INT 					{	$$ = build_a_production(@$.first_line, "Exp", 1, $1);
									}
		|	FLOAT					{	$$ = build_a_production(@$.first_line, "Exp", 1, $1);
									}
		;
Args 	: 	Exp COMMA Args 			{	$$ = build_a_production(@$.first_line, "Args", 3, $1, $2, $3);
									}
		|	Exp 					{	$$ = build_a_production(@$.first_line, "Args", 1, $1);
									}
%%
