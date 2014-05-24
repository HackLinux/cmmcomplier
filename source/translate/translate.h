#ifndef __TRANSLATE_H__
#define __TRANSLATE_H__

#include "../common/tree.h"

void intermediate_generate(struct tree_node*);

void translate_func(struct tree_node*);

void translate_def(struct tree_node*);

void translate_stmt(struct tree_node*);

void find_necessary_exp(struct tree_node*);

struct operand* translate_exp(struct tree_node*);

void translate_cond(struct tree_node*, struct operand*, struct operand*);

#endif