#ifndef __TRANSLATE_H__
#define __TRANSLATE_H__

#include "../common/tree.h"

void intermediate_generate(struct tree_node*);

void translate_func(struct tree_node*);

void translate_stmt(struct tree_node* stmt_node);

struct operand* translate_exp(struct tree_node* exp_node);

#endif