/*functions for semantic analysis, which    *
 *will be carrried out after building the syntax tree*/


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "tree.h"       
#include "table.h"
#include "semantic.h"
#include "type.h"
#include "common/tokenset.h"


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
	switch  node_type

		case external def

			switch
				case fundef

				case global var def
				
				case struct def

		case local def

		case exp

	*/
	switch(n -> unit_code){
		case ExtDef:{   //external definitions

			switch(n -> child -> sibling -> unit_code){
				case FunDec:{   //func def
					
					//ExtDef -> Specifier FunDec CompSt
					//Specifier -> TYPE
					struct tree_node* specifier_node = n -> child;
					struct tree_node* fundec_node = n -> child -> sibling;
					assert(specifier_node -> unit_code == Specifier);
					assert(fundec_node -> unit_code == FunDec);
					char *func_name = fundec_node -> child -> unit_value;

					//check func name repeat. this makes func name cannot overload
					if(find_func(func_table_head, func_name) != NULL){
						printf("Error type 4 at line %d: Function redifinition\n", fundec_node -> lineno);
						return ;
					}
					int return_type = get_type_code_from_specifier(specifier_node);

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
				case ExtDecList:{   //global var def

					//ExtDef -> Specifier ExtDecList SEMI
					struct tree_node* specifier_node = n -> child;
					struct tree_node* extdeclist_node  = n -> child -> sibling;

					//int type_code = get_type_code_from_specifier(specifier_node);

					//assert(type_code == TYPE_INT);
					struct type_descriptor* var_type = create_type_descriptor_by_specifier(specifier_node);
					char* var_name = extdeclist_node -> child -> child ->unit_value;
					struct var_descriptor* new_var_descriptor = create_var_descriptor(var_name, var_type, NULL);
					add_var(var_table_head, new_var_descriptor);
					while(extdeclist_node -> child -> sibling != NULL){
						extdeclist_node = extdeclist_node -> child -> sibling -> sibling;
						var_name = extdeclist_node -> child -> child ->unit_value;
						new_var_descriptor = create_var_descriptor(var_name, var_type, NULL);
						add_var(var_table_head, new_var_descriptor);
					}

					//todo : struct var array var ...

					//todo : anonymous structure definition
					break;
				}
				case SEMI:{     // struct def

					//Extdef -> Specifier SEMI;
					//Specifier -> StructSpecifier
					//StructSpecifer -> STRUCT OptTag LC DefList RC
					struct tree_node* struct_specifier_node = n -> child -> child;
					if(struct_specifier_node -> unit_code == TYPE){
						//(global)int ;
						printf("Error type 100 at line %d: Empty global variable definition\n", struct_specifier_node -> lineno);
						return;
					}
					struct tree_node* opttag_node = struct_specifier_node -> child -> sibling;
					if(opttag_node -> unit_code == Tag){
						printf("Error type 100 at line %d: Empty structure definition\n", struct_specifier_node -> lineno);
						return;  
					}
					struct tree_node* deflist_node = opttag_node -> sibling -> sibling;

					assert(struct_specifier_node -> unit_code == StructSpecifier);
					assert(opttag_node -> unit_code == OptTag);
					assert(deflist_node -> unit_code == DefList);
					if(opttag_node -> child == NULL){
						//anonymous structure def
						printf("Error type 100 at line %d: Anonymous structure definition without variables\n", struct_specifier_node -> lineno);
						return;
					}

					char* struct_name = opttag_node -> child -> unit_value; //ID node has a value

					/*check struct name repeat*/
					if(find_struct(struct_table_head, struct_name) != NULL){
						printf("Error type 4 at line %d: Structure redifinition\n", struct_specifier_node -> lineno);
						return ;
					}
					//todo: check struct name repeat with variables

					struct struct_descriptor* new_struct_descriptor = create_struct_descriptor(struct_name, deflist_node);
					if(new_struct_descriptor != NULL){
						add_struct(struct_table_head, new_struct_descriptor);
					}
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