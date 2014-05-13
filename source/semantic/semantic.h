#ifndef __SEMANTIC_H__
#define __SEMANTIC_H__

#include "../common/tree.h"
#include "../common/table.h"

void semantic_analyze(struct tree_node *);

void analyze_extdef_node(struct tree_node *);

void analyze_function_node(struct tree_node*, struct tree_node*, struct tree_node*);

void analyze_compst_node(struct func_descriptor*, struct tree_node*);

struct func_descriptor* create_function(struct tree_node*, struct tree_node*);

struct var_descriptor* create_variable(struct tree_node*, struct tree_node*, struct var_descriptor*, bool);

struct struct_descriptor* create_structure(struct tree_node*);

struct type_descriptor* create_type(struct tree_node*);

int init_member_list(struct var_descriptor*, struct tree_node*);

int init_param_list(struct var_descriptor*, struct tree_node*);

void check_stmt_valid(struct func_descriptor*, struct tree_node*);

struct var_descriptor* check_exp_valid(struct tree_node*);



bool check_params_valid(struct var_descriptor* params_list_head, struct tree_node* args_node);

bool var_type_equal(struct var_descriptor* v1, struct var_descriptor* v2);



#endif