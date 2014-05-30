#ifndef __OPERAND_H__
#define __OPERAND_H__

#include "../common/bool.h"

/**operand type define**/
#define OP_LABEL 0
#define OP_TEMP 1
#define OP_VAR 2
#define OP_CONST 3

struct operand {
	int type;
	int value;

	int star_num;
	int at_num;
};

struct operand* create_operand(int, int);
void operand_to_string(char*, struct operand*);

struct operand* create_operand_take_value(struct operand*);
struct operand* create_operand_take_address(struct operand*);

void take_value_of_operand(struct operand*);
void take_address_of_operand(struct operand*);

bool operand_equal(struct operand*, struct operand*);

#endif