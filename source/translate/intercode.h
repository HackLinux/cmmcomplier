#ifndef __INTERCODE_H__
#define __INTERCODE_H__

#define OP_TEMP 0
#define OP_VARIABLE 1
#define OP_CONST_INT 2
#define OP_CONST_FLOAT 3

struct operand {
	int type;
	int value;
	float float_value;
};



struct intercode{
	
	int type;
	struct operand op1;
	struct operand op2;
	struct operand op3;
	void* extra_value;

	struct intercode* prev;
	struct intercode* next;
};

struct operand* create_operand(int type);
void operand_to_string(char* buf, struct operand* op);

#endif