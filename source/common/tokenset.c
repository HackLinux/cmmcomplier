#include <string.h>

#include "bool.h"
#include "tokenset.h"

/*tokens and terminals related functions*/

/*a terminal or a non-terminal*/
bool is_terminal(int token_code){
	if(token_code > 1000)
		return false;
	return true;
}

/*token string to code*/
int token_stoc(char *token_string){
	if(strcmp(token_string, "INT") == 0) 		return INT;
	if(strcmp(token_string, "FLOAT") == 0) 		return FLOAT;
	if(strcmp(token_string, "ID") == 0) 		return ID;
	if(strcmp(token_string, "SEMI") == 0) 		return SEMI;
	if(strcmp(token_string, "COMMA") == 0) 		return COMMA;
	if(strcmp(token_string, "ASSIGNOP") == 0) 	return ASSIGNOP;
	if(strcmp(token_string, "RELOP") == 0) 		return RELOP;
	if(strcmp(token_string, "PLUS") == 0) 		return PLUS;
	if(strcmp(token_string, "MINUS") == 0) 		return MINUS;
	if(strcmp(token_string, "STAR") == 0) 		return STAR;
	if(strcmp(token_string, "DIV") == 0) 		return DIV;
	if(strcmp(token_string, "AND") == 0) 		return AND;
	if(strcmp(token_string, "OR") == 0) 		return OR;
	if(strcmp(token_string, "DOT") == 0) 		return DOT;
	if(strcmp(token_string, "NOT") == 0) 		return NOT;
	if(strcmp(token_string, "TYPE") == 0) 		return TYPE;
	if(strcmp(token_string, "LP") == 0) 		return LP;
	if(strcmp(token_string, "RP") == 0) 		return RP;
	if(strcmp(token_string, "LB") == 0) 		return LB;
	if(strcmp(token_string, "RB") == 0) 		return RB;
	if(strcmp(token_string, "LC") == 0) 		return LC;
	if(strcmp(token_string, "RC") == 0) 		return RC;
	if(strcmp(token_string, "STRUCT") == 0) 	return STRUCT;
	if(strcmp(token_string, "RETURN") == 0) 	return RETURN;
	if(strcmp(token_string, "IF") == 0) 		return IF;
	if(strcmp(token_string, "ELSE") == 0)		return ELSE;
	if(strcmp(token_string, "WHILE") == 0) 		return WHILE;

	if(strcmp(token_string, "Program") == 0) 	return Program;
	if(strcmp(token_string, "ExtDefList") == 0) return ExtDefList;
	if(strcmp(token_string, "ExtDef") == 0) 	return ExtDef;
	if(strcmp(token_string, "ExtDecList") == 0) return ExtDecList;
	if(strcmp(token_string, "Specifier") == 0) 	return Specifier;
	if(strcmp(token_string, "StructSpecifier") == 0) return StructSpecifier;
	if(strcmp(token_string, "OptTag") == 0) 	return OptTag;
	if(strcmp(token_string, "Tag") == 0) 		return Tag;
	if(strcmp(token_string, "VarDec") == 0) 	return VarDec;
	if(strcmp(token_string, "FunDec") == 0) 	return FunDec;
	if(strcmp(token_string, "VarList") == 0)	return VarList;
	if(strcmp(token_string, "ParamDec") == 0) 	return ParamDec;
	if(strcmp(token_string, "CompSt") == 0) 	return CompSt;
	if(strcmp(token_string, "StmtList") == 0) 	return StmtList;
	if(strcmp(token_string, "Stmt") == 0) 		return Stmt;
	if(strcmp(token_string, "DefList") == 0) 	return DefList;
	if(strcmp(token_string, "Def") == 0) 		return Def;
	if(strcmp(token_string, "DecList") == 0) 	return DecList;
	if(strcmp(token_string, "Dec") == 0) 		return Dec;
	if(strcmp(token_string, "Exp") == 0) 		return Exp;
	if(strcmp(token_string, "Args") == 0) 		return Args;

	return -1;
}


char* token_ctos(int token_code){
	switch(token_code){
		case INT:		return "INT";
		case FLOAT:		return "FLOAT";
		case ID:		return "ID";
		case SEMI:		return "SEMI";
		case COMMA:		return "COMMA";
		case ASSIGNOP:	return "ASSIGNOP";
		case RELOP:		return "RELOP";
		case PLUS:		return "PLUS";
		case MINUS:		return "MINUS";
		case STAR:		return "STAR";
		case DIV:		return "DIV";
		case AND:		return "AND";
		case OR:		return "OR";
		case DOT:		return "DOT";
		case NOT:		return "NOT";
		case TYPE:		return "TYPE";
		case LP:		return "LP";
		case RP:		return "RP";
		case LB:		return "LB";
		case RB:		return "RB";
		case LC:		return "LC";
		case RC:		return "RC";
		case STRUCT:	return "STRUCT";
		case RETURN:	return "RETURN";
		case IF:		return "IF";
		case ELSE:		return "ELSE";
		case WHILE:		return "WHILE";

		case Program:	return "Program";
		case ExtDefList:return "ExtDefList";
		case Specifier:	return "Specifier";
		case StructSpecifier:	return "StructSpecifier";
		case OptTag:	return "OptTag";
		case Tag:		return "Tag";
		case VarDec:	return "VarDec";
		case FunDec:	return "FunDec";
		case VarList:	return "VarList";
		case ParamDec:	return "ParamDec";
		case CompSt:	return "CompSt";
		case StmtList:	return "StmtList";
		case Stmt:		return "Stmt";
		case DefList:	return "DefList";
		case Def:		return "Def";
		case DecList:	return "DecList";
		case Dec:		return "Dec";
		case Exp:		return "Exp";
		case Args:		return "Args";
		default: 		return NULL;
	}
}