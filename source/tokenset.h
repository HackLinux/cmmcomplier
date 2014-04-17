#ifndef __TOKEN_H__
#define __TOKEN_H__

#include "syntax.h"	//terminals

//non-terminals
#define Program 1001
#define ExtDefList 1002
#define ExtDef 1003
#define ExtDecList 1004

#define Specifier 1005
#define StructSpecifier 1006
#define OptTag 1007
#define Tag 1008

#define VarDec 1009
#define FunDec 1010
#define VarList 1011
#define ParamDec 1012

#define CompSt 1013
#define StmtList 1014
#define Stmt 1015

#define DefList 1016
#define Def 1017
#define DecList 1018
#define Dec 1019

#define Exp 1020
#define Args 1021

bool is_terminal(int token_code);
int token_stoc(char *token_string);
char* token_ctos(int token_code);

#endif