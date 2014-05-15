#include <stdio.h>
#include <stdlib.h>
#include "common/tree.h"
#include "common/table.h"
#include "syntax/syntax.h"
#include "semantic/semantic.h"
#include "translate/translate.h"

#include "syntax/lexical.c"

int main(int argc, char** argv){

	if(argc != 2){
		printf("too few or too many call parameters\n");
		return 1;
	}
	FILE *f = fopen(argv[1], "r");
	if(!f){
		perror(argv[1]);
		return 1;
	}
	
	yyrestart(f);
	yyparse();

	if(error_flag)
		return 1;//lex or syntax error	

	if(semantic_analyze(program_node))
		return 1;	//semantic error

	intermediate_generate(program_node);
	
	destroy_tree(program_node);
	
	return 0;
}

yyerror(char *msg){

	error_flag = true;
	fprintf(stderr, "%s at line%d\n",msg, yylineno);
}
