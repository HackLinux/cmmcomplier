/*function definitions for semantic analysis, which	*
 *will be carrried out after building the syntax tree*/


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "tree.h"		
#include "table.h"
#include "semantic.h"

/*preorder transeverse the syntax tree to do semantic analysis*/
void
preorder_analyze(struct tree_node *root){
	assert(root != NULL);
	analyze_node(root);
	struct tree_node *child;
	for (child = root -> child; child != NULL; child = child -> sibling){
		preorder_analyze(child);
	}
}

/*analyze a tree node*/
void
analyze_node(struct tree_node *n){

	if(strcmp(n -> unit_name, "ExtDef") == 0){
		if(strcmp(n -> child -> sibling -> unit_name, "FunDec") == 0){

			struct tree_node* specifier_node = n -> child -> child;
			struct tree_node* fundec_node = n -> child -> sibling;
			if(strcmp(specifier_node -> unit_name, "StructSpecifier") == 0){
				 printf("Error type 100 at line %d: the function return a struct type\n", specifier_node -> lineno);
				 return;
			}
			assert(strcmp(specifier_node -> unit_name, "TYPE") == 0);
			char *func_name = fundec_node -> child -> unit_value;
			int return_type = (strcmp(specifier_node -> unit_value, "int") == 0) ? TYPE_INT : TYPE_FLOAT;
			int param_num = 0;
			if(strcmp(fundec_node -> child -> sibling -> sibling -> unit_name, "VarList") == 0){
				struct tree_node* varlist_node = fundec_node -> child -> sibling -> sibling;
				param_num ++ ;
				while(varlist_node -> child -> sibling != NULL){
					varlist_node = varlist_node -> child -> sibling -> sibling;
					param_num ++;
				}
			}
			struct func_symbol* new_func_symbol = create_func_symbol(func_name, return_type, param_num);
			insert_into_func_list(new_func_symbol);
		}
		
	}

}