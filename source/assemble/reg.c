#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "reg.h"
#include "../common/bool.h"

/*creators*/
void
init_reg(struct reg_descriptor* rd, int type, int num){
	rd -> type = type;
	rd -> num = num;
	rd -> used = false;
	rd -> stored_op = NULL;

	reg_to_string(rd -> name, rd);
}

void
reg_to_string(char* buf, struct reg_descriptor* rd){
	assert(rd != NULL);
	switch(rd -> type){
		case V_REG : sprintf(buf, "$v%d", rd -> num);break;
		case A_REG : sprintf(buf, "$a%d", rd -> num);break;
		case T_REG : sprintf(buf, "$t%d", rd -> num);break;
		case S_REG : sprintf(buf, "$s%d", rd -> num);break;
		default: assert(0);
	}
}
