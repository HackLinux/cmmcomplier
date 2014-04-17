/*function definitions for semantic analysis, which	*
 *will be carrried out after building the syntax tree*/


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "tree.h"		
#include "table.h"
#include "semantic.h"
#include "tokenset.h"

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

	/*
	switch	node_type

		case external def

			switch
				case fundef

				case global var def
				
				case struct def

		case local def

		case exp

	*/
	switch(n -> unit_code){
		case ExtDef:{	//external definitions

			switch(n -> child -> sibling -> unit_code){
				case FunDec:{	//func def
					struct tree_node* specifier_node = n -> child -> child;
					struct tree_node* fundec_node = n -> child -> sibling;
					if(specifier_node -> unit_code == StructSpecifier){
						printf("Error type 100 at line %d: Function return a struct type\n", specifier_node -> lineno);
				 		return;
					}
					assert(specifier_node -> unit_code == TYPE);
					char *func_name = fundec_node -> child -> unit_value;

					/*check func name repeat. this makes func name cannot overload*/
					if(find_func(func_table_head, func_name) != NULL){
						printf("Error type 4 at line %d: Function redifinition\n", fundec_node -> lineno);
						return ;
					}
					int return_type = (strcmp(specifier_node -> unit_value, "int") == 0) ? TYPE_INT : TYPE_FLOAT;
					int param_num = 0;
					if(fundec_node -> child -> sibling -> sibling -> unit_code == VarList){
						struct tree_node* varlist_node = fundec_node -> child -> sibling -> sibling;
						param_num ++ ;
						while(varlist_node -> child -> sibling != NULL){
							varlist_node = varlist_node -> child -> sibling -> sibling;
							param_num ++;
							//todo : treate params as global vars 
						}
					}
					struct func_descriptor* new_func_descriptor = create_func_descriptor(func_name, return_type, param_num);
					add_func(func_table_head, new_func_descriptor);
					break;
				}
				case ExtDecList:{	//global var def
					break;
				}
				case SEMI:{		// struct def
					break;
				}
				default: assert(0);break;
			}
		}
		case Def:{
			break;
		}
		case Exp :{
			break;
		}
		default: break;
	}
}