#ifndef __SEMANTIC_H__
#define __SEMANTIC_H__

#include "tree.h"

void preorder_analyze(struct tree_node *);
void analyze_node(struct tree_node *);


void create_variable(struct tree_node* specifier_node, struct tree_node* vardec_node, struct var_descriptor* head);

void create_structure(struct tree_node* structspecifier_node);

void create_function(struct tree_node*, struct tree_node*);

void init_member_var_list(struct var_descriptor* head, struct tree_node* deflist_node);

int get_var_num(struct tree_node* node);

#endif