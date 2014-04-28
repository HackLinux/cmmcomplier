#include <stdio.h>
#include <stdlib.h>
#include "lexical.c"

#include "tree.h"
#include "table.h"

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

	if(!error_flag){

		//print_syntax_tree($$, 0);	
		printf("no syntax error\n");
		
		semantic_analyze(program_node);
		
		print_func_table(func_table_head);
		//print_struct_table(struct_table_head);
		//print_var_table(var_table_head);
		
		destroy_tree(program_node);
	}

	return 0;
}

yyerror(char *msg){
	
	error_flag = true;
	fprintf(stderr, "Error type 2 at line %d: %s \n", yylineno ,msg);
}
