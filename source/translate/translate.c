/*translate syntax tree into intermediate representation*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "translate.h"
#include "intercode.h"
#include "operand.h"
#include "../common/table.h"
#include "../common/tree.h"
#include "../common/type.h"
#include "../common/tokenset.h"

int used_temp_num = 1;
int used_label_num = 1;

struct intercode ic_head_code;
struct intercode* ic_head = &ic_head_code;	//empty intercode head

/*generate all intercodes into list heading "ic_head"*/
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

	print_intercode_list(ic_head);
}

void
translate_func(struct tree_node* extdef_node){

	assert(extdef_node -> child -> sibling -> unit_code == FunDec);

	struct tree_node* fundec_node = extdef_node -> child -> sibling;

	//function intercode
	struct intercode* new_ic = create_func_intercode((char *)fundec_node -> child -> unit_value);
	add_code_to_tail(ic_head, new_ic);
	

	//parameters intercodes
	if(fundec_node -> child -> sibling -> sibling -> unit_code == VarList){
		struct tree_node* varlist_node = fundec_node -> child -> sibling -> sibling;
		while(true){
			struct tree_node* vardec_node = varlist_node -> child -> child -> sibling;
			if(vardec_node -> child -> unit_code == ID){
				char* var_name = (char *)vardec_node -> child -> unit_value;
				struct operand* new_op = create_operand(OP_VAR, find_var_seq(var_table_head, var_name));
				struct intercode* new_ic = create_intercode(IC_PARAM, new_op, NULL, NULL);
				add_code_to_tail(ic_head, new_ic);
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
	//def intercodes
	struct tree_node* deflist_node = fundec_node -> sibling -> child -> sibling;
	while(deflist_node -> child != NULL){
		translate_def(deflist_node -> child);
		deflist_node = deflist_node -> child -> sibling;
	} 

	//statement intercodes
	struct tree_node* stmtlist_node = fundec_node -> sibling -> child -> sibling -> sibling;
	while(stmtlist_node -> child != NULL){
		translate_stmt(stmtlist_node -> child);
		stmtlist_node = stmtlist_node -> child -> sibling;
	}

	//todo: optimize function (func_start_ic, func_stop_ic)

	//add return code if need
	struct intercode* ic_tail =  ic_head;
	while(ic_tail -> next != NULL)
		ic_tail = ic_tail -> next;
	if(ic_tail -> type != IC_RETURN){
		struct operand *op = create_operand(OP_CONST, 0);
		struct intercode* new_ic = create_intercode(IC_RETURN, op, NULL, NULL);
		add_code_to_tail(ic_head, new_ic);
	}
}

void
translate_def(struct tree_node* def_node){

	assert(def_node -> unit_code == Def);

	struct tree_node* declist_node = def_node -> child -> sibling;
	struct intercode* new_ic = NULL;
	
	while(true){
		//find each vardec and translate it.
		struct tree_node* vardec_node = declist_node -> child -> child;
		char* var_name = (char *)(find_id_node_in_vardec(vardec_node) -> unit_value);
		struct var_descriptor* vd = find_var(var_table_head, var_name);
		struct operand* var_op = create_operand(OP_VAR, find_var_seq(var_table_head, var_name));

		assert(vd != NULL);

		// struct or array variable need dec intercode
		if(vd -> var_type -> type_code == TYPE_STRUCT || vd -> var_array != NULL){
			int var_size = calculate_var_size(vd);
			
			new_ic = create_dec_intercode(var_op, var_size);
			add_code_to_tail(ic_head, new_ic);
		}

		// initialized variable need assign intercode
		if(vardec_node -> sibling != NULL){
			//todo: deal with struct or array initial
			struct operand* assign_op = translate_exp(vardec_node -> sibling -> sibling);
			new_ic = create_intercode(IC_ASSIGN, var_op, assign_op, NULL);
			add_code_to_tail(ic_head, new_ic);
		}

		if(declist_node -> child -> sibling != NULL)
			declist_node = declist_node -> child -> sibling -> sibling;
		else
			break;
	}
}


void
translate_stmt(struct tree_node* stmt_node){

	assert(stmt_node -> unit_code == Stmt);
	
	struct tree_node* first_child = stmt_node -> child;
	struct intercode* new_ic = NULL;

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
		struct operand *op = translate_exp(first_child -> sibling);
		new_ic = create_intercode(IC_RETURN, op, NULL, NULL);
		add_code_to_tail(ic_head, new_ic);
	}
	else if(first_child -> unit_code == IF){

		struct tree_node* exp_node = first_child -> sibling;
		struct tree_node* true_stmt_node = first_child -> sibling -> sibling -> sibling -> sibling;
		struct tree_node* false_stmt_node = NULL;

		struct operand *label_true = create_operand(OP_LABEL, used_label_num++);
		struct operand *label_false = create_operand(OP_LABEL, used_label_num++);
		struct operand *label_next = NULL;
		
		//cond exp code
		translate_cond(first_child -> sibling -> sibling, label_true, label_false);
		
		//label true 
		new_ic = create_intercode(IC_LABEL, label_true, NULL, NULL);
		add_code_to_tail(ic_head, new_ic);

		//true code
		translate_stmt(true_stmt_node);

		//goto next (if need)
		if(true_stmt_node -> sibling != NULL){
			label_next = create_operand(OP_LABEL, used_label_num++);
			new_ic = create_intercode(IC_GOTO, label_next, NULL, NULL);
			add_code_to_tail(ic_head, new_ic);
		}

		//label false
		new_ic = create_intercode(IC_LABEL, label_false, NULL, NULL);
		add_code_to_tail(ic_head, new_ic);

		//false code and label next(if need)
		if(true_stmt_node -> sibling != NULL){
			false_stmt_node = true_stmt_node -> sibling -> sibling;
			translate_stmt(false_stmt_node);
			new_ic = create_intercode(IC_LABEL, label_next, NULL, NULL);
			add_code_to_tail(ic_head, new_ic);
		}
		
	}
	else if(first_child -> unit_code == WHILE){
		struct operand* label_start = create_operand(OP_LABEL, used_label_num++);
		struct operand* label_true = create_operand(OP_LABEL, used_label_num++);
		struct operand* label_false = create_operand(OP_LABEL, used_label_num++);

		//label start intercode
		new_ic = create_intercode(IC_LABEL, label_start, NULL, NULL);
		add_code_to_tail(ic_head, new_ic);

		//cond exp intercode
		translate_cond(first_child -> sibling -> sibling, label_true, label_false);

		//label true
		new_ic = create_intercode(IC_LABEL, label_true, NULL, NULL);
		add_code_to_tail(ic_head, new_ic);

		//true code
		translate_stmt(first_child -> sibling -> sibling -> sibling -> sibling);

		//goto label start
		new_ic = create_intercode(IC_GOTO, label_start, NULL, NULL);
		add_code_to_tail(ic_head, new_ic);

		//label false
		new_ic = create_intercode(IC_LABEL, label_false, NULL, NULL);
		add_code_to_tail(ic_head, new_ic);

	}
	else
		assert(0);
}

struct operand*
translate_exp(struct tree_node* exp_node){

	assert(exp_node -> unit_code == Exp);
	struct tree_node* first_child = exp_node -> child;
	struct tree_node* second_child = first_child -> sibling;

	struct intercode* new_ic = NULL;
	struct operand* op1 = NULL;
	struct operand* op2 = NULL;
	struct operand* op3 = NULL;

	//Exp -> ID
	if(first_child -> unit_code == ID && second_child == NULL){	
		char* var_name = first_child -> unit_value;
		op1 = create_operand(OP_VAR, find_var_seq(var_table_head, var_name));
		return op1;
	}
	//Exp -> INT 
	if(first_child -> unit_code == INT && second_child == NULL) {
		op1 = create_operand(OP_CONST, *((int *)first_child -> unit_value));
		return op1;
	}
	//Exp -> FLOAT
	if(first_child -> unit_code == FLOAT && second_child == NULL) {
		
		printf("error, float deteced\n");
		return NULL;

	}
	//Exp -> Exp ASSIGNOP Exp
	if(first_child -> unit_code == Exp && second_child -> unit_code == ASSIGNOP) {	// assignments
		op1 = translate_exp(first_child);
		op2 = translate_exp(second_child -> sibling);

		if(op1 == NULL || op2 == NULL)
			return NULL;

		new_ic = create_intercode(IC_ASSIGN, op1, op2, NULL);
		add_code_to_tail(ic_head, new_ic);

		return op1;

	}
	//Exp -> Exp plus/minus/star/div Exp
	if(first_child -> unit_code == Exp && (second_child -> unit_code == PLUS ||
												second_child -> unit_code == MINUS ||
												second_child -> unit_code == STAR ||
												second_child -> unit_code == DIV)) {

		op1 = create_operand(OP_TEMP, used_temp_num++);
		op2 = translate_exp(first_child);
		op3 = translate_exp(second_child -> sibling);
		
		int ic_type = -1;
		if(second_child -> unit_code == PLUS)
			ic_type = IC_ADD;
		else if(second_child -> unit_code == MINUS)
			ic_type = IC_SUB;
		else if(second_child -> unit_code == STAR)
			ic_type = IC_MUL;
		else if(second_child -> unit_code == DIV)
			ic_type = IC_DIV;

		new_ic = create_intercode(ic_type, op1, op2, op3);
		add_code_to_tail(ic_head, new_ic);

		return op1;
	}
	// Exp -> MINUS Exp
	if(first_child -> unit_code == MINUS){
		op1 = create_operand(OP_TEMP, used_temp_num++);
		op2 = create_operand(OP_CONST, 0);
		op3 = translate_exp(second_child);
		new_ic = create_intercode(IC_SUB, op1, op2, op3);
		add_code_to_tail(ic_head, new_ic);
		return op1;
	}
	
	//conditions
	if(first_child -> unit_code == NOT || (first_child -> unit_code == Exp && 
											(second_child -> unit_code == AND ||
											second_child -> unit_code == OR ||
											second_child -> unit_code == RELOP))){

		struct operand* label_true = create_operand(OP_LABEL, used_label_num++);
		struct operand* label_false = create_operand(OP_LABEL, used_label_num++);

		op1 = create_operand(OP_TEMP, used_temp_num++);
		op2 = create_operand(OP_CONST, 0);
		op3 = create_operand(OP_CONST, 1);
		
		// result = 0 code
		new_ic = create_intercode(IC_ASSIGN, op1, op2, NULL);
		add_code_to_tail(ic_head, new_ic);

		translate_cond(exp_node, label_true, label_false);

		//label true
		new_ic = create_intercode(IC_LABEL, label_true, NULL, NULL);
		add_code_to_tail(ic_head, new_ic);

		//result = 1 code
		new_ic = create_intercode(IC_ASSIGN, op1, op3, NULL);
		add_code_to_tail(ic_head, new_ic);

		//label false
		new_ic = create_intercode(IC_LABEL, label_false, NULL, NULL);
		add_code_to_tail(ic_head, new_ic);

		return op1;

	}
	//Exp -> LP Exp RP
	if(first_child -> unit_code == LP && second_child -> unit_code == Exp) {
		return translate_exp(second_child);
	}
	//func call
	if(first_child -> unit_code == ID && second_child -> unit_code == LP) {
		
		char* func_name = (char *)first_child -> unit_value;

		//special func write
		if(strcmp(func_name, "write") == 0){
			op1 = translate_exp(second_child -> sibling -> child);
			new_ic = create_intercode(IC_WRITE, op1, NULL, NULL);
			add_code_to_tail(ic_head, new_ic);

			return NULL;
		}

		//args
		if(second_child -> sibling -> unit_code == Args){
			struct tree_node* args_node = second_child -> sibling;
			struct intercode* prev_arg_ic = NULL;
			while(true){
				struct tree_node* exp_node = args_node -> child;
				op1 = translate_exp(exp_node);

				//todo reverse order
				new_ic = create_intercode(IC_ARG, op1, NULL, NULL);

				if(prev_arg_ic == NULL)
					add_code_to_tail(ic_head, new_ic);
				else
					add_code_before(prev_arg_ic, new_ic);
				prev_arg_ic = new_ic;

				

				if(args_node -> child -> sibling != NULL)
					args_node = args_node -> child -> sibling -> sibling;
				else
					break;
			}
		}
		//return operator
		op1 = create_operand(OP_TEMP, used_temp_num++);
		
		if(strcmp(func_name, "read") == 0)
			new_ic = create_intercode(IC_READ, op1, NULL, NULL);
		else
			new_ic = create_call_intercode(op1, (char*)first_child -> unit_value);
		add_code_to_tail(ic_head, new_ic);
		return op1;
	}
	//array call
	if(first_child -> unit_code == Exp && second_child -> unit_code == LB) {
		//todo
	}
	//struct member call
	if(first_child -> unit_code == Exp && second_child -> unit_code == DOT) {
		//todo
	}

	assert(0);
}

void
translate_cond(struct tree_node* exp_node, struct operand* label_true, struct operand* label_false){
	assert(exp_node -> unit_code = Exp);

	struct intercode* new_ic = NULL;

	struct tree_node* first_child = exp_node -> child;
	struct tree_node* second_child = first_child -> sibling;

	//exp relop exp
	if(first_child -> unit_code == Exp && second_child -> unit_code == RELOP){

		struct operand* op1 = translate_exp(first_child);
		struct operand* op2 = translate_exp(second_child -> sibling);

		new_ic = create_if_intercode(op1, op2, label_true, (char*)second_child -> unit_value);
		add_code_to_tail(ic_head, new_ic);

		new_ic = create_intercode(IC_GOTO, label_false, NULL, NULL);
		add_code_to_tail(ic_head, new_ic);

		return ;
	}

	//not exp
	if(first_child -> unit_code == NOT){
		translate_cond(second_child, label_false, label_true);
		return ;
	}

	//exp and exp
	if(first_child -> unit_code == Exp  && second_child -> unit_code == AND){
		struct operand* label_new = create_operand(OP_LABEL, used_label_num++);
		
		translate_cond(first_child, label_new, label_false);

		new_ic = create_intercode(IC_LABEL, label_new, NULL, NULL);
		add_code_to_tail(ic_head, new_ic);

		translate_cond(second_child -> sibling, label_true, label_false);

		return;
	}

	//exp or exp
	if(first_child -> unit_code == Exp  && second_child -> unit_code == OR){

		struct operand* label_new = create_operand(OP_LABEL, used_label_num++);
		
		translate_cond(first_child, label_true, label_new);

		new_ic = create_intercode(IC_LABEL, label_new, NULL, NULL);
		add_code_to_tail(ic_head, new_ic);

		translate_cond(second_child -> sibling, label_true, label_false);

		return;
	}

	//other cases
	struct operand* op1 = translate_exp(exp_node);
	struct operand* op_zero = create_operand(OP_CONST, 0);

	new_ic = create_if_intercode(op1, op_zero, label_true, "!=");
	add_code_to_tail(ic_head, new_ic);

	new_ic = create_intercode(IC_GOTO, label_false, NULL, NULL);
	add_code_to_tail(ic_head, new_ic);

}






