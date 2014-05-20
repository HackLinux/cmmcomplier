#ifndef __OPERAND_H__
#define __OPERAND_H__

/**operand type define**/
#define OP_LABEL 0
#define OP_TEMP 1
#define OP_VAR 2
#define OP_CONST 3
#define OP_AT_VAR 4
#define OP_STAR_VAR 5

struct operand {
	int type;
	int value;
};

struct operand* create_operand(int, int);
void operand_to_string(char*, struct operand*);

#endif