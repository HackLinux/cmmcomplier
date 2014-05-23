#ifndef __OPERAND_H__
#define __OPERAND_H__

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


struct operand* take_value_of_operand(struct operand* origin_op);
struct operand* take_address_of_operand(struct operand* origin_op);

#endif