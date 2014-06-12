#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "assemble.h"
#include "reg.h"
#include "../translate/intercode.h"
#include "../translate/operand.h"

#define S_REG_NUM 9
#define T_REG_NUM 10
struct reg_descriptor s_reg[S_REG_NUM];		//for variable
struct reg_descriptor t_reg[T_REG_NUM];		//for temp

void
assemble_intercode_list(struct intercode* ic_head){

	assert(ic_head != NULL);
	struct intercode* p = ic_head -> next;

	/*init reg descriptor arrays*/
	int i;
	for (i = 0; i < S_REG_NUM; ++i)
		init_reg(&s_reg[i], S_REG, i);
	for (i = 0; i < T_REG_NUM; ++i)
		init_reg(&t_reg[i], T_REG, i);

	/*assembling*/
	while(p != NULL){
		assemble_intercode(p);
		p = p -> next;
	}
}

void
assemble_intercode(struct intercode* ic){
	
	assert(ic != NULL);

	switch(ic -> type){
		
		case IC_LABEL:{
			char op_buf[10];
			operand_to_string(op_buf, ic -> op1);
			printf("%s:\n", op_buf);
			break;
		}
		case IC_ASSIGN:{

			char left_reg_buf[10];
			reg_to_string(left_reg_buf, get_reg(ic -> op1));
			if(ic -> op2 -> type == OP_CONST){

				printf("li %s, %d\n", left_reg_buf, ic -> op2 -> value);
			}
			else{	//OP_TEMP or OP_VAR
				char right_reg_buf[10];
				reg_to_string(right_reg_buf, get_reg(ic -> op2));
				printf("move %s, %s\n", left_reg_buf, right_reg_buf);
			}
			break;
		}
		case IC_ADD:{

			char reg_buf1[10];
			char reg_buf2[10];
			reg_to_string(reg_buf1, get_reg(ic -> op1));
			reg_to_string(reg_buf2, get_reg(ic -> op2));
			if(ic -> op3 -> type == OP_CONST){
				printf("addi %s, %s, %d\n", reg_buf1, reg_buf2, ic -> op3 -> value);
			}
			else{	
				char reg_buf3[10];
				reg_to_string(reg_buf3, get_reg(ic -> op3));
				printf("add %s, %s, %s\n", reg_buf1, reg_buf2, reg_buf3);	
			}
						
			break;
		}
		case IC_SUB:{
			
			break;
		}
		case IC_MUL:{
			
			break;
		}
		case IC_DIV:{
			
			break;
		}
		case IC_GOTO:{
			
			break;
		}
		case IC_IF:{
			
			break;
		}
		case IC_RETURN:{
			
			break;
		}
		case IC_DEC:{
			
			break;
		}
		case IC_ARG:{
			
			break;
		}
		case IC_CALL:{
			
			break;
		}
		case IC_PARAM:{
			
			break;
		}
		case IC_READ:{
			
			break;
		}
		case IC_WRITE:{
			
			break;
		}
		case IC_FUNC:{
			printf("%s:\n", ic -> func_name);
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
			if(i == S_REG_NUM){
				return alloc_reg(op);	//not in register, alloc for it.
			}

			s_reg[i].used = true;
			s_reg[i].stored_op = op;
			return &s_reg[i];
		}

		case OP_TEMP:{

			int i;
			for (i = 0; i < T_REG_NUM; ++i)
				if(operand_equal(t_reg[i].stored_op, op))
					break;
			if(i == T_REG_NUM){
				return alloc_reg(op);	//not in register, alloc for it.
			}
			/*after one use, the temp will be thrown out of register*/
			t_reg[i].used = false;
			t_reg[i].stored_op = NULL;
			return &t_reg[i];
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