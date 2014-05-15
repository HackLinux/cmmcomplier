/*translate syntax tree into intermediate representation*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "translate.h"
#include "../common/tree.h"
#include "../common/tokenset.h"

void 
intermediate_generate(struct tree_node* program_node){

	assert(program_node -> unit_code == Program);

	//find each fundec and translate into intermediate code
	struct tree_node* extdef_node = program_node -> child -> child;
	while( extdef_node != NULL){

		if(extdef_node -> child -> sibling -> unit_code == FunDec)
			translate_func(extdef_node);
		extdef_node = extdef_node -> sibling -> child;
	}
}

void
translate_func(struct tree_node* extdef_node){

	assert(extdef_node -> child -> sibling -> unit_code == FunDec);

	struct tree_node* fundec_node = extdef_node -> child -> sibling;
	
	printf("Function %s :\n", (char *)fundec_node -> child -> unit_value);
	
	if(fundec_node -> child -> sibling -> sibling -> unit_code == VarList){
		struct tree_node* varlist_node = fundec_node -> child -> sibling -> sibling;
		while(true){
			struct tree_node* vardec_node = varlist_node -> child -> child -> sibling;
			if(vardec_node -> child -> unit_code == ID){
				printf("Param %s\n", (char *)vardec_node -> child -> unit_value);
			}
			else{
				printf("error, use array as a parameter\n");
			}
			if(varlist_node -> child -> sibling != NULL)
				varlist_node = varlist_node -> child -> sibling -> sibling;
			else
				break;
		}
	}

	struct tree_node* stmtlist_node = fundec_node -> sibling -> child -> sibling -> sibling;
	while(stmtlist_node -> child != NULL){
		translate_stmt(stmtlist_node -> child);
		stmtlist_node = stmtlist_node -> child -> sibling;
	}
}

void
translate_stmt(struct tree_node* stmt_node){

	assert(stmt_node -> unit_code == Stmt);
	struct tree_node* first_child = stmt_node -> child;

	if(first_child -> unit_code == Exp){
		translate_exp(first_child);
	} 
	else if(first_child -> unit_code == CompSt){
		struct tree_node* inner_stmtlist_node = first_child -> child -> sibling -> sibling;
		while(inner_stmtlist_node -> child != NULL){
			translate_stmt(inner_stmtlist_node -> child);
			inner_stmtlist_node = inner_stmtlist_node -> child -> sibling;
		}
	}
	else if(first_child -> unit_code == RETURN){
		
	}
	else if(first_child -> unit_code == IF){
		
	}
	else if(first_child -> unit_code == WHILE){

	}
	else
		assert(0);
}

void
translate_exp(struct tree_node* exp_node){

	assert(exp_node -> unit_code == Exp);

	printf("hint exp\n");

}






