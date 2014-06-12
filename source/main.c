#include <stdio.h>
#include <stdlib.h>
#include "common/tree.h"
#include "common/table.h"
#include "syntax/syntax.h"
#include "semantic/semantic.h"
#include "translate/translate.h"
#include "translate/intercode.h"

#include "syntax/lexical.c"

int main(int argc, char** argv){

	FILE* fin = NULL;	//cmm source file
	FILE* fout = NULL;	//intercode output file
	if(argc == 2){
		fin = fopen(argv[1], "r");
		fout = stdout;
	}
	else if (argc == 3){
		fin = fopen(argv[1], "r");
		fout = fopen(argv[2], "w");	
		if(!fout){
			perror(argv[2]);
			return 1;
		}
	}
	else{
		printf("too few or too many call parameters\n");
		return 1;
	}
	if(!fin){
		perror(argv[1]);
		return 1;
	}
	
	yyrestart(fin);
	yyparse();	

	/*	a syntax tree has been generated until here.
	 *	the tree is rooted in "program_node" declared in common/tree.h
	 *	semantic analyze, ir-code gengrate and assemble-code generate
	 *	will be done on the syntax tree*/

	if(error_flag)
		return 1;	//lex or syntax error	

	if(semantic_analyze(program_node))
		return 1;	//semantic error

	struct intercode* ic_head = intermediate_generate(program_node);

	print_intercode_list(ic_head, fout);

	printf("\n\n");

	assemble_intercode_list(ic_head);
	
	destroy_tree(program_node);
	
	return 0;
}

yyerror(char *msg){
	error_flag = true;
	fprintf(stderr, "%s at line%d\n",msg, yylineno);
}
