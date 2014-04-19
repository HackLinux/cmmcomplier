#ifndef __SEMANTIC_H__
#define __SEMANTIC_H__

#include "tree.h"

void preorder_analyze(struct tree_node *);
void analyze_node(struct tree_node *);
int get_type_code_from_specifier(struct tree_node*);


#endif