#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "operand.h"

struct operand*
create_operand(int type, int value){
	assert(type >= OP_LABEL && type <= OP_STAR_VAR);
	struct operand* op = (struct operand*)malloc(sizeof(struct operand));
	op -> type = type;
	op -> value = value;
	return op;
}

/*buf size no less than 10*/
void
operand_to_string(char* buf, struct operand* op){
	switch(op -> type){
		case OP_LABEL :
			sprintf(buf, "label%d :", op -> value);
			break;
		case OP_TEMP :
			sprintf(buf, "t%d", op -> value);
			break;
		case OP_VAR :
			sprintf(buf, "v%d", op -> value);
			break;
		case OP_CONST :
			sprintf(buf, "#%d", op -> value);
			break;
		case OP_AT_VAR :
			sprintf(buf, "&%d", op -> value);
			break;
		case OP_STAR_VAR :
			sprintf(buf, "*%d", op -> value);
			break;
		default : assert(0);
	}
}