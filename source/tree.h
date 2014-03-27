#ifndef __TREE_H__
#define __TREE_H__

#include "bool.h"

#define NODE_SIZE sizeof(struct tree_node)

union value{
	int int_value;
	float float_value;
};

/*A node in the syntax tree.
 *Each node describes a token or a non-terminal */
struct tree_node {
	bool is_token;				/*a token(1) or a nonterminal(0)*/
	int lineno;					/*line number of the token*/
	char unit_name[20];			/*name of the syntax unit*/
	union value v;				/*int or float tokens have value*/
	char id_name[20];			/*id tokens have id name*/
	struct tree_node *child;	
	struct tree_node *sibling;	

};

/*creators*/
struct tree_node *create_node(bool, int, char*);
struct tree_node *create_int_node(int, char*, int);
struct tree_node *create_float_node(int, char*, float);
struct tree_node *create_id_node(int, char*, char*);
struct tree_node *create_type_node(int, char*, char*);
struct tree_node *build_a_production(int, char *, int, ...);

/*visitors*/
void preorder_traverse(struct tree_node *root, int level);
void visit(struct tree_node *n, int level);
#endif