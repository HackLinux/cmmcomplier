#ifndef __REG_H__
#define __REG_H__

#include "../common/bool.h"
#include "../translate/operand.h"

#define V_REG 0
#define A_REG 1
#define T_REG 2
#define S_REG 3

struct reg_descriptor{
	int type;
	int num;

	bool used;
	struct operand* stored_op;
};

void init_reg(struct reg_descriptor* rd, int type, int num);
void reg_to_string(char* buf, struct reg_descriptor* rd);


#endif