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

/*find all extdef node in tree to do semantic analysis*/
void
semantic_analyze(struct tree_node *root){
	
	assert(root -> unit_code == Program);

	struct tree_node* extdeflist_node = root -> child;

	while(extdeflist_node -> child != NULL){
		analyze_extdef_node(extdeflist_node -> child);
		extdeflist_node = extdeflist_node -> child -> sibling;
	}

}

/*analyze an extdef node*/
void
analyze_extdef_node(struct tree_node* extdef_node){

	assert( extdef_node -> unit_code == ExtDef );
	
	/*switch

		case fundef

		case global var def
				
		case struct def
	*/
	struct tree_node* specifier_node = extdef_node -> child;
	struct tree_node* second_child = extdef_node -> child -> sibling;
	switch(second_child -> unit_code){
		case ExtDecList: {	//ExtDef -> Specifier ExtDecList SEMI
			struct tree_node* extdeclist_node  = second_child;
			while(true){
				struct tree_node* vardec_node = extdeclist_node -> child;
				create_variable(specifier_node, vardec_node, var_table_head, false);
				if(extdeclist_node -> child -> sibling != NULL)
					extdeclist_node = extdeclist_node -> child -> sibling -> sibling;
				else
					break;
			}
			break;
		}
		case FunDec: {		//ExtDef -> Specifier FunDec CompSt
			struct tree_node* fundec_node = second_child;
			struct tree_node* compst_node = fundec_node -> sibling;
			analyze_function_node(specifier_node, fundec_node, compst_node);
			break;
		}
		case SEMI : {		//Extdef -> Specifier SEMI;
			
			if(specifier_node -> child -> unit_code == TYPE){	//Specifier -> TYPE
				printf("Error type 100 at line %d: Empty variable definition\n", specifier_node -> lineno);
				return;
			}
			//Specifier -> StructSpecifier
			struct tree_node* structspecifier_node = specifier_node -> child;
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
		default: assert(0);
	}

}

/*create func by func head and analyze func body*/
void 
analyze_function_node(struct tree_node* specifier_node, struct tree_node* fundec_node, struct tree_node* compst_node){
	struct func_descriptor* new_func = create_function(specifier_node, fundec_node);
	
	if(new_func!= NULL){	//create success
		analyze_compst_node(new_func, compst_node);
	}
}

/*analyze the func body*/
void
analyze_compst_node(struct func_descriptor* belongs_func, struct tree_node* compst_node){

	assert(compst_node -> unit_code == CompSt);

	struct tree_node* deflist_node = compst_node -> child -> sibling;
	struct tree_node* stmtlist_node = deflist_node -> sibling;


	//firstly, deal with local defs
	while(deflist_node -> child != NULL){
		
		struct tree_node* specifier_node = deflist_node -> child -> child;
		struct tree_node* declist_node = specifier_node -> sibling;
		while(true){
			struct tree_node* vardec_node = declist_node -> child -> child;
			struct var_descriptor* new_var = create_variable(specifier_node, vardec_node, var_table_head, false);
			if(vardec_node -> sibling != NULL){
				struct var_descriptor* init_exp_var = check_exp_valid(vardec_node -> sibling -> sibling);
				if(new_var != NULL && init_exp_var != NULL)
					if(!var_type_equal(new_var , init_exp_var))
						printf("Error type 5 at line %d: Type mismatch between assignment operator\n", vardec_node -> lineno);
			}
			if(declist_node -> child -> sibling != NULL)
				declist_node = declist_node -> child -> sibling -> sibling;	
			else
				break;
		}
		deflist_node = deflist_node -> child -> sibling;
	}

	//then stmts
	while(stmtlist_node -> child  != NULL){
		check_stmt_valid(belongs_func, stmtlist_node -> child);
		stmtlist_node = stmtlist_node -> child -> sibling;
	}
}


/*create a function and add it into function list
 *the function is defined by an extdef node in syntax tree*/
struct func_descriptor*
create_function(struct tree_node* specifier_node, struct tree_node* fundec_node){
				
	char* func_name = fundec_node -> child -> unit_value;

	if(find_func(func_table_head, func_name) != NULL){
		printf("Error type 4 at line %d: Function \'%s\'redifine\n", fundec_node -> lineno, func_name);
		return NULL;
	}
	if(is_struct_type(specifier_node) && is_struct_definition(specifier_node -> child)){
		printf("Warnning: struct defined inside return field of function \'%s\'\n", func_name);
	}

	/*handle return type*/
	struct type_descriptor* return_type = create_type(specifier_node);

	/*create func descriptor and add into list*/
	struct func_descriptor* new_func_descriptor = create_func_descriptor(func_name, return_type, 0);
	add_func(func_table_head, new_func_descriptor);
	
	/*handle params*/
	if(fundec_node -> child -> sibling -> sibling -> unit_code == VarList){
		struct tree_node* varlist_node = fundec_node -> child -> sibling -> sibling;
		new_func_descriptor -> param_num = init_param_list(new_func_descriptor -> param_list_head, varlist_node);
	}

	return new_func_descriptor;
}


/*create a variable and add it into a var list
 *var defined by the specifier and vardec nodes in syntax tree*/
struct var_descriptor*
create_variable(struct tree_node* specifier_node, struct tree_node* vardec_node, struct var_descriptor* head, bool is_member_var){
	

	struct type_descriptor* var_type = create_type(specifier_node);
	if(var_type == NULL)
		return NULL;
	char* var_name = find_id_node_in_vardec(vardec_node) -> unit_value;
	struct array_descriptor* var_array = create_array_descriptor_by_vardec(vardec_node);
	struct var_descriptor* new_var =  create_var_descriptor(var_name, var_type, var_array);
	if(find_var(head, var_name) == NULL){
		add_var(head, new_var);
		return new_var;
	}
	else{
		if(is_member_var)
			printf("Error type 15 at line %d: member variable \'%s\' redefine\n", vardec_node -> lineno, var_name);
		else
			printf("Error type 3 at line %d: variable \'%s\' redefine\n", vardec_node -> lineno, var_name);
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
			printf("Error type 16 at line %d: Struct name redefine\n", structspecifier_node -> lineno);
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


/*create type of a variable*/
struct type_descriptor*
create_type(struct tree_node* specifier_node){

	assert(specifier_node -> unit_code == Specifier);

	int type_code = get_type_code_from_specifier(specifier_node);
	struct struct_descriptor* sd;
	
	if(type_code == TYPE_INT || type_code == TYPE_FLOAT){
		sd = NULL;
	}
	if(type_code == TYPE_STRUCT){ 

		//Specifier -> StructSpecifier
		struct tree_node* structspecifier_node = specifier_node -> child;
		if(structspecifier_node -> child -> sibling -> unit_code == Tag){	//exist struct
			char *struct_name = structspecifier_node -> child -> sibling -> child -> unit_value;
			sd = find_struct(struct_table_head, struct_name);
			if(sd == NULL){
				printf("Error Type 17 at line %d : Undefined struct %s\n", specifier_node -> lineno, struct_name );
	 			return NULL;
			}
		}
		else{	//new struct
			sd = create_structure(structspecifier_node);
		}
	}
	return create_type_descriptor(type_code, sd);
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
			if(create_variable(specifier_node, vardec_node, member_list_head, true) != NULL){
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
		struct var_descriptor* new_var = create_variable(specifier_node, vardec_node, var_table_head, false);

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
check_stmt_valid(struct func_descriptor* belongs_func, struct tree_node* stmt_node){


	assert(stmt_node -> unit_code == Stmt);

	struct tree_node* first_child = stmt_node -> child;
	if(first_child -> unit_code == Exp){
		check_exp_valid(first_child);
	} 
	else if(first_child -> unit_code == CompSt){
		analyze_compst_node(belongs_func, first_child);
	}
	else if(first_child -> unit_code == RETURN){
		struct var_descriptor* return_var = check_exp_valid(first_child -> sibling);
		
		if(return_var != NULL && type_equal(return_var -> var_type, belongs_func -> return_type))
			;
		else
			printf("Error type 8 at line %d: Return type mismatch in function \'%s\'\n", first_child -> lineno, belongs_func -> func_name);
	}
	else if(first_child -> unit_code == IF){
		check_exp_valid(first_child -> sibling -> sibling);
		//todo exp type in if

		struct tree_node* stmt_node_in_if = first_child -> sibling -> sibling -> sibling -> sibling;
		check_stmt_valid(belongs_func, stmt_node_in_if);
		if(stmt_node_in_if -> sibling != NULL){
			check_stmt_valid(belongs_func, stmt_node_in_if -> sibling -> sibling);
		}
	}
	else if(first_child -> unit_code == WHILE){
		check_exp_valid(first_child -> sibling -> sibling);
		//todo exp type in while
		check_stmt_valid(belongs_func, first_child -> sibling -> sibling -> sibling -> sibling);
	}
}

/*treate an expression as a variable to do nested check*/
struct var_descriptor*
check_exp_valid(struct tree_node* exp_node){
	
	assert(exp_node -> unit_code == Exp);
	struct tree_node* first_child = exp_node -> child;
	struct tree_node* second_child = first_child -> sibling;

	//Exp -> ID
	if(first_child -> unit_code == ID && second_child == NULL){	
		char* var_name = first_child -> unit_value;
		struct var_descriptor* the_var = find_var(var_table_head, var_name);
		if(the_var == NULL)
			printf("Error type 1 at line %d: Undefined variable \'%s\'\n", first_child -> lineno, var_name);
		return the_var;
	}
	//Exp -> INT 
	else if(first_child -> unit_code == INT && second_child == NULL) {
		struct type_descriptor* int_type = create_type_descriptor(TYPE_INT, NULL);
		struct var_descriptor* temp_var = create_var_descriptor("", int_type, NULL);	//memory leak
		temp_var -> is_lvalue = false;
		return temp_var;
	}
	//Exp -> FLOAT
	else if(first_child -> unit_code == FLOAT && second_child == NULL) {
		struct type_descriptor* float_type = create_type_descriptor(TYPE_FLOAT, NULL);
		struct var_descriptor* temp_var = create_var_descriptor("", float_type, NULL); //memory leak
		temp_var -> is_lvalue = false;
		return temp_var;
	}
	//Exp -> Exp ASSIGNOP Exp
	else if(first_child -> unit_code == Exp && second_child -> unit_code == ASSIGNOP) {	// assignments
		struct var_descriptor* left_var = check_exp_valid(first_child);
		struct var_descriptor* right_var = check_exp_valid(second_child -> sibling);
		
		if(left_var == NULL || right_var == NULL)//errors happen in check sub expression
			return NULL;

		if(!left_var -> is_lvalue){
			printf("Error type 6 at line %d: lvalue required as left operand of assignment\n", first_child -> lineno);
			return NULL;
		}
		if(!var_type_equal(left_var, right_var)){
			printf("Error type 5 at line %d: Type mismatch between assignment operator\n", first_child -> lineno);
			return NULL;
		}
		return left_var;	//doubt it
	}
	//Exp -> Exp 2-operator Exp
	else if(first_child -> unit_code == Exp && (second_child -> unit_code == AND ||
												second_child -> unit_code == OR ||
												second_child -> unit_code == RELOP ||
												second_child -> unit_code == PLUS ||
												second_child -> unit_code == MINUS ||
												second_child -> unit_code == STAR ||
												second_child -> unit_code == DIV)) {	// 2-operand operator expressions
		struct var_descriptor* left_var = check_exp_valid(first_child);
		struct var_descriptor* right_var = check_exp_valid(second_child -> sibling);
		
		if(left_var == NULL || right_var == NULL)//errors happen in check sub expression
			return NULL;
		
		if(!var_type_equal(left_var, right_var)){
			printf("Error type 7 at line %d: Type mismatch between \'%s\' operator\n", first_child -> lineno, second_child -> unit_name);
			return NULL;
		}
		left_var = create_var_descriptor("", right_var -> var_type, right_var -> var_array);
		left_var -> is_lvalue = false;
		return left_var;	//doubt
	}
	//1-operand operators
	//including "LP RP" "MINUS" "NOT"
	else if(second_child -> unit_code == Exp) {
		struct var_descriptor* op_var = check_exp_valid(second_child);
		struct var_descriptor* temp_var = create_var_descriptor("", op_var -> var_type, op_var -> var_array);	//memory leak
		temp_var -> is_lvalue = false;
		return temp_var;
	}
	//func call
	else if(first_child -> unit_code == ID && second_child -> unit_code == LP) {
		
		char* func_name = first_child  -> unit_value;
		struct func_descriptor* the_func = find_func(func_table_head, func_name);
		if(the_func == NULL){
			if(find_var(var_table_head, func_name))
				printf("Error type 11 at line %d: Use variable \'%s\' as a function \n", first_child -> lineno, func_name);
			else
				printf("Error type 2 at line %d: Undefined function \'%s\'\n", first_child -> lineno, func_name);
			return NULL;
		}
		//params check
		struct tree_node* args_node = (second_child -> sibling -> unit_code == Args) ? second_child -> sibling : NULL;
		if(!check_params_valid(the_func -> param_list_head, args_node)){
			printf("Error type 9 at line %d: Argument mismatch in function \'%s\'\n", first_child -> lineno, func_name);
		}

		struct var_descriptor* temp_var = create_var_descriptor("", the_func -> return_type, NULL);	//memory leak
		temp_var -> is_lvalue = false;
		return temp_var;
	}
	//array call
	else if(first_child -> unit_code == Exp && second_child -> unit_code == LB) {
		
		struct var_descriptor* array_var = check_exp_valid(first_child);
		struct var_descriptor* sub_var = check_exp_valid(second_child -> sibling);
		if(array_var == NULL)
			return NULL;
		if(sub_var == NULL || sub_var -> var_type -> type_code != TYPE_INT){
			printf("Error type 12 at line %d: Array subscript is not an integer\n", first_child -> lineno);
		}
		if(array_var -> var_array == NULL){
			printf("Error type 10 at line %d: Use non-array variable as an array\n", first_child -> lineno);
			return NULL;
		}
		return create_var_descriptor(array_var -> var_name, array_var -> var_type, array_var -> var_array -> subarray);
	}
	//struct member call
	else if(first_child -> unit_code == Exp && second_child -> unit_code == DOT) {
		
		struct var_descriptor* struct_var = check_exp_valid(first_child);
		char* member_var_name = second_child -> sibling -> unit_value;
		if(struct_var == NULL)
			return NULL;
		struct struct_descriptor* the_struct = struct_var -> var_type -> sd;
		if(the_struct == NULL){
			printf("Error type 13 at line %d: Use member access operator \'.\' for non-struct variable \'%s\'\n", first_child -> lineno, struct_var -> var_name);
			return NULL;
		}
		struct var_descriptor* member_var = find_var(the_struct -> member_list_head, member_var_name);
		if(member_var == NULL){
			printf("Error type 14 at line %d: Undefined field \'%s\' in struct \'%s\'\n", first_child -> lineno, member_var_name, the_struct -> struct_name);
			return NULL;
		}
		return  member_var;
	}

	assert(0);//all situations should be checked and returned
}

bool
check_params_valid(struct var_descriptor* param_list_head, struct tree_node* args_node){
	struct var_descriptor* expect_param = param_list_head -> next;
	
	//no params need, and no real params
	if(args_node == NULL && expect_param == NULL)
		return true;
	
	//one NULL, one not
	if(args_node == NULL || expect_param == NULL)
		return false;
	
	//both not null
	while(true){

		struct var_descriptor* real_param = check_exp_valid(args_node -> child);
		
		if(real_param == NULL)	//errors happen in check sub expression
			return false;
		if(!var_type_equal(expect_param, real_param));
			return false;
		if(args_node -> child -> sibling != NULL && expect_param -> next != NULL){	//both have next
			args_node = args_node -> child -> sibling -> sibling;
			expect_param = expect_param -> next;
		}
		else
			break;
	}
	//left one null, one not
	if(args_node != NULL || expect_param != NULL)
		return false;
	
	return true;
}


/*type compare and array compare*/
bool
var_type_equal(struct var_descriptor* v1, struct var_descriptor* v2){
	assert(v1 != NULL && v2 != NULL);
	if(!type_equal(v1 -> var_type, v2 -> var_type))
		return false;
	if(!array_equal(v1 -> var_array, v2 -> var_array))
		return false;
	return true;
}