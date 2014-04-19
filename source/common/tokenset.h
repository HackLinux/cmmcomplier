#ifndef __TOKEN_H__
#define __TOKEN_H__


//terminals
#define INT 258
#define FLOAT 259
#define ID 260
#define SEMI 261
#define COMMA 262
#define ASSIGNOP 263
#define RELOP 264
#define PLUS 265
#define MINUS 266
#define STAR 267
#define DIV 268
#define AND 269
#define OR 270
#define DOT 271
#define NOT 272
#define TYPE 273
#define LP 274
#define RP 275
#define LB 276
#define RB 277
#define LC 278
#define RC 279
#define STRUCT 280
#define RETURN 281
#define IF 282
#define ELSE 283
#define WHILE 284

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