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
				case FunDec:{  //func def
					
					//ExtDef -> Specifier FunDec CompSt
					//Specifier -> TYPE
					struct tree_node* specifier_node = n -> child;
					struct tree_node* fundec_node = n -> child -> sibling;
					char* func_name = fundec_node -> child -> unit_value;

					if(find_func(func_table_head, func_name) != NULL){
						printf("Error type 4 at line %d: Function redifinition\n", fundec_node -> lineno);
						return ;
					}
					if(is_struct_type(specifier_node) && is_struct_definition(specifier_node -> child)){
						printf("Warnning: struct defined inside function return field\n");
					}

					struct type_descriptor* return_type = create_type_descriptor_by_specifier(specifier_node);
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
					struct tree_node* vardec_node = extdeclist_node -> child;

					struct type_descriptor* var_type = create_type_descriptor_by_specifier(specifier_node);
					char* var_name = find_id_node_in_vardec(vardec_node) -> unit_value;
					struct array_descriptor* var_array = create_array_descriptor_by_vardec(vardec_node);
					struct var_descriptor* new_var_descriptor = create_var_descriptor(var_name, var_type, var_array);
					add_var(var_table_head, new_var_descriptor);
					while(extdeclist_node -> child -> sibling != NULL){
						
						extdeclist_node = extdeclist_node -> child -> sibling -> sibling;
						vardec_node = extdeclist_node -> child;
						var_name = find_id_node_in_vardec(vardec_node) -> unit_value;
						var_array = create_array_descriptor_by_vardec(vardec_node);
						new_var_descriptor = create_var_descriptor(var_name, var_type, var_array);
						add_var(var_table_head, new_var_descriptor);
					}
					//todo : array var ...
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
					//todo: member variables


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