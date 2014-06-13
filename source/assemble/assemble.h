#ifndef __ASSEMBLE_H__
#define __ASSEMBLE_H__

#include "../translate/intercode.h"

void assemble_intercode(struct intercode*);
void assemble_func(struct intercode*, struct intercode*);
void assemble_one_intercode(struct intercode*);

struct reg_descriptor* alloc_reg(struct operand*);
struct reg_descriptor* get_reg(struct operand*);

void push_stack();
void pop_stack();
int calculate_offset();

#endif