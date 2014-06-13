#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "assemble.h"
#include "reg.h"
#include "../translate/intercode.h"
#include "../translate/operand.h"

#define S_REG_NUM 9
#define T_REG_NUM 9
#define A_REG_NUM 4
struct reg_descriptor s_reg[S_REG_NUM];			//for variable
struct reg_descriptor t_reg[T_REG_NUM + 1];		//0-8 for temp and 9 for const
struct reg_descriptor a_reg[A_REG_NUM];			//for arguments


void
assemble_intercode(struct intercode* ic_head){

	assert(ic_head != NULL);
	struct intercode* func_start_ic = ic_head -> next;
	
	printf(	".data\n"
			"_prompt: .asciiz \"Enter an integer:\"\n"
			"_ret: .asciiz \"\\n\"\n"
			".globl main\n"
			".text\n"
			"\nread:\n"
			"	li $v0, 4\n"
			"	la $a0, _prompt\n"
			"	syscall\n"
			"	li $v0, 5\n"
			"	syscall\n"
			"	jr $ra\n"
			"\nwrite:\n"
			"	li $v0, 1\n"
			"	syscall\n"
			"	li $v0, 4\n"
			"	la $a0, _ret\n"
			"	syscall\n"
			"	move $v0, $0\n"
			"	jr $ra\n");

	/*init a_reg*/
	int i;
	for (i = 0; i < A_REG_NUM; ++i)
			init_reg(&a_reg[i], A_REG, i);

	/*assembling*/
	while(func_start_ic != NULL){

		struct intercode* func_stop_ic = find_func_stop_ic(func_start_ic);

		/*init s_reg and t_reg per func*/
		for (i = 0; i < S_REG_NUM; ++i)
			init_reg(&s_reg[i], S_REG, i);
		for (i = 0; i < T_REG_NUM + 1; ++i)
			init_reg(&t_reg[i], T_REG, i);
		
		assemble_func(func_start_ic, func_stop_ic);
		func_start_ic = func_stop_ic -> next;

	}
}

void
assemble_func(struct intercode* func_start, struct intercode* func_stop){
	assert(func_start != NULL);
	assert(func_stop != NULL);
	struct intercode* p = func_start;
	while(p != func_stop -> next){
		assemble_one_intercode(p);
		p = p -> next;
	}
}


void
assemble_one_intercode(struct intercode* ic){
	
	assert(ic != NULL);

	//todo array

	//todo & and * symbol in operand

	switch(ic -> type){
		
		case IC_LABEL:{
			char op_buf[10];
			operand_to_string(op_buf, ic -> op1);
			printf("%s:\n", op_buf);
			break;
		}
		case IC_ASSIGN:{
			struct reg_descriptor* reg1 = get_reg(ic -> op1);
			if(ic -> op2 -> type == OP_CONST){
				printf("\tli %s, %d\n", reg1 -> name, ic -> op2 -> value);
			}
			else{	//OP_TEMP or OP_VAR
				struct reg_descriptor* reg2 = get_reg(ic -> op2);
				printf("\tmove %s, %s\n", reg1 -> name, reg2 -> name);
			}
			break;
		}
		case IC_ADD:{

			struct reg_descriptor* reg1 = get_reg(ic -> op1);
			struct reg_descriptor* reg2 = get_reg(ic -> op2);
			if(ic -> op3 -> type == OP_CONST){
				printf("\taddi %s, %s, %d\n", reg1 -> name, reg2 -> name, ic -> op3 -> value);
			}
			else{
				struct reg_descriptor* reg3 = get_reg(ic -> op3);	
				printf("\tadd %s, %s, %s\n", reg1 -> name, reg2 -> name, reg3 -> name);
			}
						
			break;
		}
		case IC_SUB:{

			struct reg_descriptor* reg1 = get_reg(ic -> op1);
			struct reg_descriptor* reg2 = get_reg(ic -> op2);
			if(ic -> op3 -> type == OP_CONST){
				printf("\taddi %s, %s, %d\n", reg1 -> name, reg2 -> name, -ic -> op3 -> value);
			}
			else{
				struct reg_descriptor* reg3 = get_reg(ic -> op3);	
				printf("\tsub %s, %s, %s\n", reg1 -> name, reg2 -> name, reg3 -> name);
			}
			
			break;
		}
		case IC_MUL:{

			struct reg_descriptor* reg1 = get_reg(ic -> op1);
			struct reg_descriptor* reg2 = get_reg(ic -> op2);
			struct reg_descriptor* reg3 = get_reg(ic -> op3);
			printf("\tmul %s, %s, %s\n", reg1 -> name, reg2 -> name, reg3 -> name);	
			
			break;
		}
		case IC_DIV:{

			struct reg_descriptor* reg1 = get_reg(ic -> op1);
			struct reg_descriptor* reg2 = get_reg(ic -> op2);
			struct reg_descriptor* reg3 = get_reg(ic -> op3);
			printf("\tdiv %s, %s\n", reg2 -> name, reg3 -> name);
			printf("\tmflo %s\n", reg1 -> name);	
			
			break;
		}
		case IC_GOTO:{
			char op_buf[10];
			operand_to_string(op_buf, ic -> op1);
			printf("\tj %s\n", op_buf);
			break;
		}
		case IC_IF:{

			if(ic -> op1 -> type == OP_CONST && ic -> op2 -> type == OP_CONST)
				assert(0);

			//x, y
			struct reg_descriptor* reg1 = get_reg(ic -> op1);
			struct reg_descriptor* reg2 = get_reg(ic -> op2);
			
			//label
			char label_buf[10];
			operand_to_string(label_buf, ic -> op3);
			
			//relop
			char relop_buf[10];
			if(strcmp(ic -> relop, "==") == 0)
				strcpy(relop_buf, "beq");
			else if(strcmp(ic -> relop, "!=") == 0)
				strcpy(relop_buf, "bne");
			else if(strcmp(ic -> relop, ">") == 0)
				strcpy(relop_buf, "bgt");
			else if(strcmp(ic -> relop, "<") == 0)
				strcpy(relop_buf, "blt");
			else if(strcmp(ic -> relop, ">=") == 0)
				strcpy(relop_buf, "bge");
			else if(strcmp(ic -> relop, "<=") == 0)
				strcpy(relop_buf, "ble");
			else
				assert(0);

			printf("\t%s %s, %s, %s\n", relop_buf, reg1 -> name, reg2 -> name, label_buf);
			
			break;
		}
		case IC_RETURN:{
			printf("\tmove $v0, %s\n", get_reg(ic -> op1) -> name);
			printf("\tjr $ra\n");
			break;
		}
		case IC_DEC:{
			assert(0);
			break;
		}
		case IC_ARG:{
			
			break;
		}
		case IC_CALL:{

			/*params*/
			struct intercode* arg_ic = ic -> prev;
			int used_a_reg = 0;
			while(arg_ic -> type == IC_ARG){
				printf(	"\tmove $a%d, %s\n", used_a_reg++, get_reg(arg_ic -> op1) -> name);
				arg_ic = arg_ic -> prev;
			}
			assert(used_a_reg <= 4);

			
			
			printf(	"\taddi $sp, $sp, -4\n"
					"\tsw $ra, 0($sp)\n");
			printf(	"\tjal %s\n", ic -> func_name);
			printf(	"\tlw $ra, 0($sp)\n"
					"\taddi $sp, $sp, 4\n");
			printf("\tmove %s, $v0\n", get_reg(ic -> op1) -> name);

		}
		case IC_PARAM:{
			//printf("move $v0, \n");
			break;
		}
		case IC_READ:{

			printf(	"\taddi $sp, $sp, -4\n"
					"\tsw $ra, 0($sp)\n"
					"\tjal read\n"
					"\tlw $ra, 0($sp)\n"
					"\taddi $sp, $sp, 4\n");
			printf("\tmove %s, $v0\n", get_reg(ic -> op1) -> name);

			
			break;
		}
		case IC_WRITE:{

			printf(	"\tmove $a0, %s\n", get_reg(ic -> op1) -> name);
			printf(	"\taddi $sp, $sp, -4\n"
					"\tsw $ra, 0($sp)\n"
					"\tjal write\n"
					"\tlw $ra, 0($sp)\n"
					"\taddi $sp, $sp, 4\n");
			break;
		}
		case IC_FUNC:{
			printf("\n%s:\n", ic -> func_name);

			/*params*/
			struct intercode* param_ic = ic -> next;
			int used_a_reg = 0;
			while(param_ic -> type == IC_PARAM){
				printf(	"\tmove %s, $a%d\n", get_reg(param_ic -> op1) -> name, used_a_reg++);
				param_ic = param_ic -> next;
			}
			assert(used_a_reg <= 4);

			break;
		}
		default:	assert(0);break;
	}
}

/*get the reg descriptor of a variable or temp*/
struct reg_descriptor*
get_reg(struct operand *op){
	assert(op != NULL);

	switch(op -> type){
		
		case OP_VAR:{
			int i;
			for (i = 0; i < S_REG_NUM; ++i)
				if(operand_equal(s_reg[i].stored_op, op))
					break;
			if(i < S_REG_NUM){
				return &s_reg[i];	//find in s_reg
			}
			return alloc_reg(op);	//not in register, alloc for it.
		}

		case OP_TEMP:{

			int i;
			for (i = 0; i < T_REG_NUM; ++i)
				if(operand_equal(t_reg[i].stored_op, op))
					break;
			if(i < T_REG_NUM){
				/*after one use, the temp will be thrown out of register*/
				t_reg[i].used = false;
				t_reg[i].stored_op = NULL;
				return &t_reg[i];
			}
			return alloc_reg(op);	//not in register, alloc for it.
		}

		case OP_CONST:{
			printf("\tli %s, %d\n", t_reg[9].name, op -> value);
			return &t_reg[9];
		}

		default: assert(0);
	}
	return NULL;
}

/*alloc a reg for a variable or a temp*/
struct reg_descriptor*
alloc_reg(struct operand* op){
	assert(op != NULL);

	switch(op -> type){
		
		case OP_VAR:{
			int i;
			for (i = 0; i < S_REG_NUM; ++i)
				if(!s_reg[i].used)
					break;
			if(i == S_REG_NUM){
				printf("no reg for variable, exit\n");
				exit(-1);
			}

			s_reg[i].used = true;
			s_reg[i].stored_op = op;
			return &s_reg[i];
		}

		case OP_TEMP:{
			int i;
			for (i = 0; i < T_REG_NUM; ++i)
				if(!t_reg[i].used)
					break;
			if(i == T_REG_NUM){
				printf("no more register for variable, exit\n");
				exit(-1);
			}

			t_reg[i].used = true;
			t_reg[i].stored_op = op;
			return &t_reg[i];
		}

		default: assert(0);
	}
	return NULL;
}