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
					struct tree_node* specifier_node = n -> child;
					struct tree_node* fundec_node = n -> child -> sibling;
					create_function(specifier_node, fundec_node);

					break;
				}
				case ExtDecList:{   //global var def

					//ExtDef -> Specifier ExtDecList SEMI
					struct tree_node* specifier_node = n -> child;
					struct tree_node* extdeclist_node  = n -> child -> sibling;
					while(true){
						//ExtDecList -> VarDec
						//ExtDecList -> VarDec COMMA ExtDecList
						struct tree_node* vardec_node = extdeclist_node -> child;
						create_variable(specifier_node, vardec_node, var_table_head);
						if(extdeclist_node -> child -> sibling == NULL){
							break;
						}
						extdeclist_node = extdeclist_node -> child -> sibling -> sibling;
					}
					break;
				}
				case SEMI:{     // struct def

					//Extdef -> Specifier SEMI;
					//Specifier -> StructSpecifier
					struct tree_node* structspecifier_node = n -> child -> child;
					create_structure(structspecifier_node);
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


/*create a function and add it into function list
 *the function is defined by an extdef node in syntax tree*/
void
create_function(struct tree_node* specifier_node, struct tree_node* fundec_node){
				
	char* func_name = fundec_node -> child -> unit_value;

	if(find_func(func_table_head, func_name) != NULL){
		printf("Error type 4 at line %d: Function redifinition\n", fundec_node -> lineno);
		return ;
	}
	if(is_struct_type(specifier_node) && is_struct_definition(specifier_node -> child)){
		printf("Warnning: struct defined inside function return field\n");
	}

	int param_num = 0;
	if(fundec_node -> child -> sibling -> sibling -> unit_code == VarList){
		struct tree_node* varlist_node = fundec_node -> child -> sibling -> sibling;
		param_num = get_var_num(varlist_node);
		//todo : treate params as global vars 
	}
	struct type_descriptor* return_type = create_type_descriptor_by_specifier(specifier_node);
	struct func_descriptor* new_func_descriptor = create_func_descriptor(func_name, return_type, param_num);
	add_func(func_table_head, new_func_descriptor);
}


/*create a variable and add it into a var list
 *var defined by the specifier and vardec nodes in syntax tree*/
void
create_variable(struct tree_node* specifier_node, struct tree_node* vardec_node, struct var_descriptor* head){
	struct type_descriptor* var_type = create_type_descriptor_by_specifier(specifier_node);
	char* var_name = find_id_node_in_vardec(vardec_node) -> unit_value;
	struct array_descriptor* var_array = create_array_descriptor_by_vardec(vardec_node);
	struct var_descriptor* new_var =  create_var_descriptor(var_name, var_type, var_array);
	if(find_var(head, var_name) == NULL)
		add_var(head, new_var);
	else
		printf("Error type 3 at line %d: variable redefine\n", vardec_node -> lineno);
}


/*create a structure and add it into the struct list
 *structure defined by structspecifier node in syntax tree*/
void
create_structure(struct tree_node* structspecifier_node){
	assert(structspecifier_node -> unit_code == StructSpecifier);

	if(structspecifier_node -> unit_code == TYPE){	//Specifier -> TYPE
		printf("Error type 100 at line %d: Empty variable definition\n", structspecifier_node -> lineno);
		return;
	}
	
	struct tree_node* opttag_node = structspecifier_node -> child -> sibling;
	if(opttag_node -> unit_code == Tag){	//StructSpecifier -> STRUCT Tag
		printf("Error type 100 at line %d: Empty structure definition\n", structspecifier_node -> lineno);
		return;  
	}
	if(opttag_node -> child == NULL){	//OptTag -> empty
		printf("Error type 100 at line %d: Anonymous structure definition without variables\n", structspecifier_node -> lineno);
		return;
	}

	char* struct_name = opttag_node -> child -> unit_value; //ID node has a value
	/*check struct name repeat*/
	if((find_struct(struct_table_head, struct_name) != NULL) ||
		(find_var(var_table_head, struct_name) != NULL)){
		printf("Error type 15 at line %d: Structure redefine\n", structspecifier_node -> lineno);
		return ;
	}
	struct tree_node* deflist_node = opttag_node -> sibling -> sibling;
		
	struct struct_descriptor* new_struct_descriptor = create_struct_descriptor(struct_name);
	init_member_var_list(new_struct_descriptor -> member_list_head, deflist_node);
	new_struct_descriptor -> member_num =  get_var_num(deflist_node);				
	add_struct(struct_table_head, new_struct_descriptor);
}


/*add vars defined in the deflist of a structure into the structure's member list*/
void
init_member_var_list(struct var_descriptor* head, struct tree_node* deflist_node){
	
	assert(deflist_node -> unit_code == DefList);
	
	while(deflist_node -> child != NULL){
		
		struct tree_node* specifier_node = deflist_node -> child -> child;
		struct tree_node* declist_node = specifier_node -> sibling;
		
		while(true){
			struct tree_node* vardec_node = declist_node -> child -> child;
			if(vardec_node -> sibling != NULL){
				printf("Error type 15 at line %d: struct member initialized\n", vardec_node -> lineno);
			}
			create_variable(specifier_node, vardec_node, head);
			if(declist_node -> child -> sibling == NULL){
				break;	
			}
			declist_node = declist_node -> child -> sibling -> sibling;
		}
		deflist_node = deflist_node -> child -> sibling;
	}
}



/*get variable number from an extdeflist/deflist/varlist*/
int
get_var_num(struct tree_node* node){
	int num = 0;
	switch(node -> unit_code){
		case ExtDefList:	//same with deflist
		case DefList:{
			struct tree_node* deflist_node = node;
			while(deflist_node -> child != NULL){
				struct tree_node* declist_node = deflist_node -> child -> child -> sibling;
				num++;
				while(declist_node -> child -> sibling != NULL){
					declist_node = declist_node -> child -> sibling -> sibling;
					num++;
				}
				deflist_node = deflist_node -> child -> sibling;
			}
		} break;
		case VarList:{
			struct tree_node* varlist_node = node;
			num ++;
			while(varlist_node -> child -> sibling != NULL){
				num ++;
				varlist_node = varlist_node -> child -> sibling -> sibling;
			}
		} break;
		default: assert(0);
	}
	return num;
}