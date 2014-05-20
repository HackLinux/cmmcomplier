#ifndef __INTERCODE_H__
#define __INTERCODE_H__

#include "operand.h"

/**intercode type define**/
#define IC_LABEL 0
#define IC_ASSIGN 1
#define IC_ADD 2
#define IC_SUB 3
#define IC_MUL 4
#define IC_DIV 5
#define IC_GOTO 6
#define IC_IF 7
#define IC_RETURN 8
#define IC_DEC 9
#define IC_ARG 10
#define IC_CALL 11
#define IC_PARAM 12
#define IC_READ 13
#define IC_WRITE 14
#define IC_FUNC 15

struct intercode{
	
	int type;
	struct operand *op1;
	struct operand *op2;
	struct operand *op3;
	int dec_size;		//dec intercode has this
	char* relop;		//if intercode has this
	char* func_name;	//func or call intercode has this

	struct intercode* prev;
	struct intercode* next;
};

struct operand* create_operand(int, int);
void operand_to_string(char*, struct operand*);

struct intercode* create_intercode(int, struct operand*, struct operand*, struct operand*);
struct intercode* create_dec_intercode(struct operand*, int);
struct intercode* create_if_intercode(struct operand*, struct operand*, struct operand*, char*);
struct intercode* create_call_intercode(struct operand*, char*);
struct intercode* create_func_intercode(char*);


void destroy_intercode(struct intercode*);
void destroy_intercode_list(struct intercode*);

void print_intercode(struct intercode*);
void print_intercode_list(struct intercode*);

void add_code_to_tail(struct intercode*, struct intercode*);

#endif