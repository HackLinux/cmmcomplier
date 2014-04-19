#ifndef __TREE_H__
#define __TREE_H__

#include "common/bool.h"

extern bool error_flag;

union value{
	int int_value;
	float float_value;
};


struct tree_node {
	
	bool is_terminal;			/*a token(1) or a nonterminal(0)*/
	int lineno;					/*line number of the token*/

	int unit_code;				/*the code of the token or terminal, defined in tokenset.h*/
	char unit_name[20];			/*name of the syntax unit*/
	void *unit_value;			/*value of the syntax unit. so called lexeme.
								 *INT node has an integer, ID node has a string and son on*/
	struct tree_node *child;
	struct tree_node *sibling;
};

/*creators*/
struct tree_node *create_node(int, char*);
struct tree_node *create_int_node(int, char*, int);
struct tree_node *create_float_node(int, char*, float);
struct tree_node *create_id_node(int, char*, char*);
struct tree_node *create_type_node(int, char*, char*);
struct tree_node *build_a_production(int, char *, int, ...);

/*visitors*/
void print_syntax_tree(struct tree_node *root, int level);
void visit(struct tree_node *n, int level);

/*destructors*/
void destroy_tree(struct tree_node *);
void destroy_node(struct tree_node *);
#endif