#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "operand.h"

struct operand*
create_operand(int type, int value){
	assert(type >= OP_LABEL && type <= OP_CONST);
	struct operand* op = (struct operand*)malloc(sizeof(struct operand));
	op -> type = type;
	op -> value = value;

	op -> star_num = 0;
	op -> at_num = 0;
	return op;
}

struct operand*
create_operand_full(int type, int value, int star_num, int at_num){
	
	assert(star_num * at_num == 0);	//at least one zero 
	struct operand* new_op = create_operand(type, value);
	new_op -> star_num = star_num;
	new_op -> at_num = at_num;

	return new_op;
}

/*create an operand that add a star symbol(*) on the original operand*/
struct operand*
take_value_of_operand(struct operand* origin_op){

	assert(origin_op -> type == OP_TEMP || origin_op -> type == OP_VAR);
	struct operand* new_op = create_operand_full(
		origin_op -> type, origin_op -> value, origin_op -> star_num, origin_op -> at_num);

	new_op -> star_num ++;

	while(new_op -> star_num * new_op -> at_num != 0){	//both non-zero
		new_op -> star_num --;
		new_op -> at_num --;
	}

	return new_op;
}

/*create an operand that add a at symbol(&) on the original operand*/
struct operand*
take_address_of_operand(struct operand* origin_op){
	assert(origin_op -> type == OP_TEMP || origin_op -> type == OP_VAR);
	struct operand* new_op = create_operand_full(
		origin_op -> type, origin_op -> value, origin_op -> star_num, origin_op -> at_num);

	new_op -> at_num ++; 

	while(new_op -> star_num * new_op -> at_num != 0){	//both non-zero
		new_op -> star_num --;
		new_op -> at_num --;
	}

	return new_op;
}


/*buf size no less than 10*/
void
operand_to_string(char* buf, struct operand* op){
	
	assert(op -> star_num * op -> at_num == 0);	//cannot both non-zero

	int i = 0, j = 0;
	if(op -> star_num != 0)
		for (; i < op -> star_num; ++i)
			buf[i] = '*';
	
	if(op -> at_num != 0)
		for (; j < op -> at_num; ++j)
			buf[i + j] = '&';

	switch(op -> type){
		case OP_LABEL :
			sprintf(buf + i + j, "label%d", op -> value);
			break;
		case OP_TEMP :
			sprintf(buf + i + j, "t%d", op -> value);
			break;
		case OP_VAR :
			sprintf(buf + i + j, "v%d", op -> value);
			break;
		case OP_CONST :
			sprintf(buf + i + j, "#%d", op -> value);
			break;
		default : assert(0);
	}
}