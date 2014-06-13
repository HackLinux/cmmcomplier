#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "operand.h"
#include "intercode.h"

/*creators*/
struct intercode*
create_intercode(int type, struct operand* op1, struct operand* op2, struct operand* op3){
	struct intercode* new_ic = (struct intercode*)malloc(sizeof(struct intercode));
	new_ic -> type = type;
	new_ic -> op1 = op1;
	new_ic -> op2 = op2;
	new_ic -> op3 = op3;
	new_ic -> dec_size = -1;
	new_ic -> relop = NULL;
	new_ic -> func_name = NULL;
	new_ic -> prev = NULL;
	new_ic -> next = NULL;
	return new_ic;
}

struct intercode*
create_dec_intercode(struct operand* op1, int dec_size){
	struct intercode* new_ic = create_intercode(IC_DEC, op1, NULL, NULL);
	new_ic -> dec_size = dec_size;
	return new_ic;
}

struct intercode*
create_if_intercode(struct operand* op1, struct operand* op2, struct operand* op3, char* relop){
	struct intercode* new_ic = create_intercode(IC_IF, op1, op2, op3);
	new_ic -> relop = (char *)malloc(strlen(relop));
	strcpy(new_ic -> relop, relop);
	return new_ic;
}

struct intercode*
create_call_intercode(struct operand* op1, char* func_name){
	struct intercode* new_ic = create_intercode(IC_CALL, op1, NULL, NULL);
	new_ic -> func_name = (char *)malloc(strlen(func_name));
	strcpy(new_ic -> func_name, func_name);
	return new_ic;
}

struct intercode*
create_func_intercode(char* func_name){
	struct intercode* new_ic = create_intercode(IC_FUNC, NULL, NULL, NULL);
	new_ic -> func_name = (char *)malloc(strlen(func_name));
	strcpy(new_ic -> func_name, func_name);
	return new_ic;
}

/*adders*/
void
add_code_to_tail(struct intercode* ic_head, struct intercode* new_ic){
	assert(ic_head != NULL && new_ic != NULL);
	assert(new_ic -> next == NULL && new_ic -> prev == NULL);
	
	struct intercode* ic_tail = ic_head;
	while(ic_tail -> next != NULL)
		ic_tail = ic_tail -> next;
	ic_tail -> next = new_ic;
	new_ic -> prev = ic_tail;
}

// add a intercode before "old_ic"
void
add_code_before(struct intercode* old_ic, struct intercode* new_ic){
	assert(old_ic != NULL && new_ic != NULL);
	assert(new_ic -> next == NULL && new_ic -> prev == NULL);
	assert(old_ic -> prev != NULL);

	struct intercode* prev_ic = old_ic -> prev;

	prev_ic -> next = new_ic;
	old_ic -> prev = new_ic;
	new_ic -> next = old_ic;
	new_ic -> prev = prev_ic;
}

/*destructors*/
void
destroy_intercode(struct intercode *ic){
	if(ic -> relop != NULL)
		free(ic -> relop);
	if(ic -> func_name != NULL)
		free(ic -> func_name);
	ic -> relop = NULL;
	ic -> func_name = NULL;
	free(ic);
	ic = NULL;
}

void
destroy_intercode_list(struct intercode* ic_head){
	if(ic_head == NULL)
		return ;
	destroy_intercode_list(ic_head -> next);
	destroy_intercode(ic_head);
}

/*printers*/
void
print_intercode(struct intercode* ic, FILE *fp){
	
	assert(ic != NULL);

	char op1_buf[10], op2_buf[10], op3_buf[10];

	if(ic -> op1 != NULL)
		operand_to_string(op1_buf, ic -> op1);
	if(ic -> op2 != NULL)
		operand_to_string(op2_buf, ic -> op2);
	if(ic -> op3 != NULL)	
		operand_to_string(op3_buf, ic -> op3);

	switch(ic -> type){
		case IC_LABEL : 
			fprintf(fp, "LABEL %s :\n", op1_buf);
			break;
		case IC_ASSIGN : 
			fprintf(fp, "%s := %s\n", op1_buf, op2_buf);
			break;
		case IC_ADD : 
			fprintf(fp, "%s := %s + %s\n", op1_buf, op2_buf, op3_buf);
			break;
		case IC_SUB : 
			fprintf(fp, "%s := %s - %s\n", op1_buf, op2_buf, op3_buf);
			break;
		case IC_MUL : 
			fprintf(fp, "%s := %s * %s\n", op1_buf, op2_buf, op3_buf);
			break;
		case IC_DIV : 
			fprintf(fp, "%s := %s / %s\n", op1_buf, op2_buf, op3_buf);
			break;
		case IC_GOTO : 
			fprintf(fp, "GOTO %s\n", op1_buf);
			break;
		case IC_IF : 
			fprintf(fp, "IF %s %s %s GOTO %s\n",  op1_buf, ic -> relop, op2_buf, op3_buf);
			break;
		case IC_RETURN : 
			fprintf(fp, "RETURN %s\n", op1_buf);
			break;
		case IC_DEC :
			fprintf(fp, "DEC %s %d\n", op1_buf, ic -> dec_size); 
			break;
		case IC_ARG : 
			fprintf(fp, "ARG %s\n", op1_buf);
			break;
		case IC_CALL : 
			fprintf(fp, "%s := CALL %s\n", op1_buf, ic -> func_name);
			break;
		case IC_PARAM :
			fprintf(fp, "PARAM %s\n", op1_buf);
			break;
		case IC_READ : 
			fprintf(fp, "READ %s\n", op1_buf);
			break;
		case IC_WRITE : 
			fprintf(fp, "WRITE %s\n", op1_buf);
			break;
		case IC_FUNC :
			if(fp == stdout)
				fprintf(fp, "\n");
			fprintf(fp, "FUNCTION %s :\n", ic -> func_name);
			break;
		default : assert(0);
	}
}

void
print_intercode_list(struct intercode* ic_head, FILE *fp){
	assert(ic_head != NULL);
	struct intercode* p = ic_head -> next;
	while(p != NULL){
		print_intercode(p, fp);
		p = p -> next;
	}
}

struct intercode*
find_func_stop_ic(struct intercode* func_start_ic){
	struct intercode* func_stop_ic = func_start_ic;
	while(1){
		if(func_stop_ic -> next == NULL)
			break;
		if(func_stop_ic -> next -> type == IC_FUNC)
			break;
		func_stop_ic = func_stop_ic -> next;
	}
	return func_stop_ic;
}

