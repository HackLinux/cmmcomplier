#ifndef __ASSEMBLE_H__
#define __ASSEMBLE_H__

#include "../translate/intercode.h"

void assemble_intercode_list(struct intercode*);
void assemble_intercode(struct intercode*);

struct reg_descriptor* alloc_reg(struct operand*);
struct reg_descriptor* get_reg(struct operand*);

#endif