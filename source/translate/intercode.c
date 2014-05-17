#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "intercode.h"

struct operand*
create_operand(int type){
	assert(type >= 0 && type <= 3);
	struct operand* op = (struct operand*)malloc(sizeof(struct operand));
	op -> type = type;
	return op;
}

/*buf size no less than 10*/
void
operand_to_string(char* buf, struct operand* op){
	switch(op -> type){
		case OP_TEMP :
			sprintf(buf, "t%d", op -> value);
			break;
		case OP_VARIABLE :
			sprintf(buf, "v%d", op -> value);
			break;
		case OP_CONST_INT :
			sprintf(buf, "#%d", op -> value);
			break;
		case OP_CONST_FLOAT :
			sprintf(buf, "#%f", op -> float_value);
			break;
	}
}