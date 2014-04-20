/*functions for semantic analysis, which    *
 *will be carrried out after building the syntax tree*/


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "semantic.h"
#include "tree.h"       
#include "table.h"
#include "type.h"
#include "common/tokenset.h"

/*preorder transeverse the syntax tree to do semantic analysis*/
void
semantic_analyze(struct tree_node *root){
	assert(root != NULL);
	analyze_node(root);
	struct tree_node *child;
	for (child = root -> child; child != NULL; child = child -> sibling){
		semantic_analyze(child);
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

		case local def (in compst)

		case stmt

	*/
	switch(n -> unit_code){
		case ExtDef:{   //external definitions

			switch(n -> child -> sibling -> unit_code){
				case FunDec:{  //func def
					
					//ExtDef -> Specifier FunDec CompSt
					struct tree_node* specifier_node = n -> child;
					struct tree_node* fundec_node = n -> child -> sibling;
					struct tree_node* compst_node = fundec_node -> sibling;
					create_function(specifier_node, fundec_node, compst_node);

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
						if(extdeclist_node -> child -> sibling != NULL)
							extdeclist_node = extdeclist_node -> child -> sibling -> sibling;
						else
							break;
					}
					break;
				}
				case SEMI:{     // struct def

					//Extdef -> Specifier SEMI;
					//Specifier -> StructSpecifier
					struct tree_node* specifier_node = n -> child;
					struct tree_node* structspecifier_node = specifier_node -> child;
					if(specifier_node -> child -> unit_code == TYPE){	//Specifier -> TYPE
						printf("Error type 100 at line %d: Empty variable definition\n", specifier_node -> lineno);
						return;
					}
					if(structspecifier_node -> child -> sibling -> unit_code == Tag){	//StructSpecifier -> STRUCT Tag
						printf("Error type 100 at line %d: Empty structure definition\n", structspecifier_node -> lineno);
						return ;  
					}
					if(structspecifier_node -> child -> sibling -> child == NULL){	//OptTag -> empty
						printf("Error type 100 at line %d: Anonymous structure definition without variables\n", structspecifier_node -> lineno);
						return ;
					}
					create_structure(structspecifier_node);
					break;
				}
				default: assert(0);break;
			}
			break;	//break of case ExtDef
		}
		case CompSt:{	

			//CompSt -> LC DefList StmtList RC
			struct tree_node* deflist_node = n -> child -> sibling;
			
			//handle each local def
			while(deflist_node -> child != NULL){
		
				struct tree_node* specifier_node = deflist_node -> child -> child;
				struct tree_node* declist_node = specifier_node -> sibling;
				while(true){
					struct tree_node* vardec_node = declist_node -> child -> child;
					if(vardec_node -> sibling != NULL){
						//todo: ASSIGNOP
					}
					create_variable(specifier_node, vardec_node, var_table_head);
					if(declist_node -> child -> sibling != NULL)
						declist_node = declist_node -> child -> sibling -> sibling;	
					else
						break;
				}
				deflist_node = deflist_node -> child -> sibling;
			}
			break;
		}
		case Stmt:{
			//find expressions in statements and check their validation
			check_stmt_valid(n);

		}
		default: break;
	}
}


/*create a function and add it into function list
 *the function is defined by an extdef node in syntax tree*/
void
create_function(struct tree_node* specifier_node, struct tree_node* fundec_node, struct tree_node* compst_node){
				
	char* func_name = fundec_node -> child -> unit_value;

	if(find_func(func_table_head, func_name) != NULL){
		printf("Error type 4 at line %d: Function \'%s\'redifinition\n", fundec_node -> lineno, func_name);
		return ;
	}
	if(is_struct_type(specifier_node) && is_struct_definition(specifier_node -> child)){
		printf("Warnning: struct defined inside return field of function \'%s\'\n", func_name);
	}

	/*handle return type*/
	struct type_descriptor* return_type = create_type_descriptor_by_specifier(specifier_node);
	/*struct tree_node* stmtlist_node = compst_node -> child -> sibling -> sibling;
	struct tree_node* return_stmt_node = NULL;
	while(stmtlist_node -> child != NULL){
		return_stmt_node = stmtlist_node -> child;
		stmtlist_node = stmtlist_node -> child -> sibling;
	}
	if(return_stmt_node == NULL){
		printf("Warnning: control reaches end of non-void function \'%s\'\n", func_name);
	} else{
		assert(return_stmt_node -> child -> unit_code == RETURN);
		struct tree_node* return_exp_node = return_stmt_node -> child -> sibling;
		//todo : determine return type consistent
	}*/

	/*create func descriptor and add into list*/
	struct func_descriptor* new_func_descriptor = create_func_descriptor(func_name, return_type, 0);
	add_func(func_table_head, new_func_descriptor);
	
	/*handle params*/
	if(fundec_node -> child -> sibling -> sibling -> unit_code == VarList){
		struct tree_node* varlist_node = fundec_node -> child -> sibling -> sibling;
		new_func_descriptor -> param_num = init_param_list(new_func_descriptor -> param_list_head, varlist_node);
		
	}
}


/*create a variable and add it into a var list
 *var defined by the specifier and vardec nodes in syntax tree*/
struct var_descriptor*
create_variable(struct tree_node* specifier_node, struct tree_node* vardec_node, struct var_descriptor* head){
	struct type_descriptor* var_type = create_type_descriptor_by_specifier(specifier_node);
	char* var_name = find_id_node_in_vardec(vardec_node) -> unit_value;
	struct array_descriptor* var_array = create_array_descriptor_by_vardec(vardec_node);
	struct var_descriptor* new_var =  create_var_descriptor(var_name, var_type, var_array);
	if(find_var(head, var_name) == NULL){
		add_var(head, new_var);
		return new_var;
	}
	else{
		//todo maybe error type 15 when create struct member
		printf("Error type 3 at line %d: variable \'%s\'redefine\n", vardec_node -> lineno, var_name);
		return NULL;
	}
}


/*create a structure and add it into the struct list
 *structure defined by structspecifier node in syntax tree*/
struct struct_descriptor*
create_structure(struct tree_node* structspecifier_node){

	assert(structspecifier_node -> unit_code == StructSpecifier);
	struct tree_node* opttag_node = structspecifier_node -> child -> sibling;
	char* struct_name;
	if(structspecifier_node -> child -> sibling -> child != NULL){	//OptTag !-> empty
		struct_name = opttag_node -> child -> unit_value; //ID node has a value
		/*check struct name repeat*/
		if((find_struct(struct_table_head, struct_name) != NULL) ||
			(find_var(var_table_head, struct_name) != NULL)){
			printf("Error type 15 at line %d: Structure redefine\n", structspecifier_node -> lineno);
			return NULL;
		}
	}
	else
		struct_name = "";	//empty struct name
	
	struct tree_node* deflist_node = opttag_node -> sibling -> sibling;
		
	struct struct_descriptor* new_struct_descriptor = create_struct_descriptor(struct_name);
	new_struct_descriptor -> member_num = init_member_list(new_struct_descriptor -> member_list_head, deflist_node);
	add_struct(struct_table_head, new_struct_descriptor);
	return new_struct_descriptor;
}


/*add vars defined in the deflist of a structure into the structure's member list
 *return member var number*/
int
init_member_list(struct var_descriptor* member_list_head, struct tree_node* deflist_node){
	
	assert(deflist_node -> unit_code == DefList);
	assert(member_list_head != NULL);
	
	int num = 0;

	while(deflist_node -> child != NULL){
		
		struct tree_node* specifier_node = deflist_node -> child -> child;
		struct tree_node* declist_node = specifier_node -> sibling;
		
		while(true){
			struct tree_node* vardec_node = declist_node -> child -> child;
			if(vardec_node -> sibling != NULL){
				printf("Error type 15 at line %d: struct member initialized\n", vardec_node -> lineno);
				break;
			}
			if(create_variable(specifier_node, vardec_node, member_list_head) != NULL){
				num ++;
			}
			if(declist_node -> child -> sibling != NULL)
				declist_node = declist_node -> child -> sibling -> sibling;
			else
				break;
		}
		deflist_node = deflist_node -> child -> sibling;
	}
	//print_var_table(member_list_head);
	return num;
}

/*add vars defined in a func param list into var table
 *return param numbers*/
int
init_param_list(struct var_descriptor* param_list_head, struct tree_node* varlist_node){
	
	assert(varlist_node -> unit_code == VarList);
	assert(param_list_head != NULL);

	int num = 0;
	struct var_descriptor* prev_var = param_list_head;

	while(true){
		
		//treate all params as global vars	
		struct tree_node* specifier_node = varlist_node -> child -> child;
		struct tree_node* vardec_node = specifier_node -> sibling;
		struct var_descriptor* new_var = create_variable(specifier_node, vardec_node, var_table_head);

		if(new_var != NULL){
			//insert param into param list in order(not insert after head)
			num ++;
			struct var_descriptor* new_var_copy = create_var_descriptor(new_var -> var_name, new_var -> var_type, new_var -> var_array);
			prev_var -> next = new_var_copy;
			prev_var = new_var_copy;
		}
		if(varlist_node -> child -> sibling != NULL)
			varlist_node = varlist_node -> child -> sibling -> sibling;
		else
			break;
	}
	//print_var_table(param_list_head);
	return num;

}



void
check_stmt_valid(struct tree_node* stmt_node){


	assert(stmt_node -> unit_code == Stmt);
	struct tree_node* first_child = stmt_node -> child;
	if(first_child -> unit_code == Exp){
		check_exp_valid(first_child);
	} 
	else if(first_child -> unit_code == RETURN){
		check_exp_valid(first_child -> sibling);
		//todo return type consistent
	}
	else if(first_child -> unit_code == IF){
		check_exp_valid(first_child -> sibling -> sibling);
		//todo exp type in if
	}
	else if(first_child -> unit_code == WHILE){
		check_exp_valid(first_child -> sibling -> sibling);
		//todo exp type in while
	}
}

/*treate an expression as a variable to do nested check*/
struct var_descriptor*
check_exp_valid(struct tree_node* exp_node){
	struct tree_node* first_child = exp_node -> child;
}