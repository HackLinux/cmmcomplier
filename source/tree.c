/*syntax tree related functions used in syntax analysis*/
/*a multi tree will be build after syntax analysis*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>		//variable parameters

#include "tree.h"
#include "common/bool.h"
#include "common/tokenset.h"

/*a global error flag for both lexical and syntax errors*/
bool error_flag = false;

/*creators*/
struct tree_node*
create_node(int lineno, char *unit_name){

	assert(strlen(unit_name) >= 2);
	assert(strlen(unit_name) < 20);
	struct tree_node *node = (struct tree_node *)malloc(sizeof(struct tree_node));
	node -> unit_code = token_stoc(unit_name);
	node -> is_terminal = is_terminal(node -> unit_code);
	node -> lineno = lineno;
	node -> unit_value = NULL;
 	node -> child = NULL;
	node -> sibling = NULL;
	strcpy(node -> unit_name, unit_name);
	return node;
}


struct tree_node*
create_int_node(int lineno, char *unit_name, int int_value){
	struct tree_node *node = create_node(lineno, unit_name);
	node -> unit_value = malloc(sizeof(int));
	*(int *)(node -> unit_value) = int_value;
	return node;
}

struct tree_node*
create_float_node(int lineno, char *unit_name, float float_value){
	struct tree_node *node = create_node(lineno, unit_name);
	node -> unit_value = malloc(sizeof(float));
	*(float *)(node -> unit_value) = float_value;
	return node;
}

struct tree_node*
create_id_node(int lineno, char *unit_name, char* id_name){
	
	assert(strlen(id_name) < 20);	/*need modify*/

	struct tree_node *node = create_node(lineno, unit_name);
	node -> unit_value = malloc(strlen(id_name));
	strcpy((char *)node -> unit_value, id_name);
	return node;
}

struct tree_node*
create_type_node(int lineno, char *unit_name, char* type_name){
	return create_id_node(lineno, unit_name, type_name);
}

/* build a small tree according to a production.
 * use this function in syntax.y like below:
 		build_a_production(@$.start_line, "Program", 1, $1);
 		build_a_production(@$.start_line, "EXP", 3, $1, S2, S3);
 *@param lineno:	line number of the nonterminal on the left of the production
 *@param left_name:	name of the left nonterminal on the left of the production
 *@param right_num:	number of the symbols on right part of the production, no less than 0
 *@param ...:  		variable parameters, each points to a symbol on right part of the production 
 *@return : the pointer of the left non-terminal node */
struct tree_node*
build_a_production(int lineno, char *left_name, int right_num, ...){
	assert(right_num >= 0);

	int i;
	va_list ap;
	struct tree_node *pnode;
	/*create the non-terminal node on the left og the production*/
	struct tree_node *new_node = create_node(lineno,left_name);	// create the left non-terminal
	if(right_num == 0)
		return new_node;

	/*hanging up all childs on the right of the production*/
	va_start(ap, right_num);
	pnode = va_arg(ap, struct tree_node*);
	new_node -> child = pnode;
	for (i = 1; i < right_num; ++i){
		 pnode -> sibling = va_arg(ap, struct tree_node*);
		 pnode = pnode -> sibling;
		 assert(pnode != NULL);
	}
	va_end(ap);
	return new_node;
}



/*visitors*/
void
print_syntax_tree(struct tree_node *root, int level){
	assert(root != NULL);
	visit(root, level);
	level++;
	struct tree_node *child;
	for (child = root -> child; child != NULL; child = child -> sibling){
		print_syntax_tree(child, level);
	}
}

void 
visit(struct tree_node *n, int level){
	switch(n -> is_terminal){
		case true: /*terminals*/
			while(level -- > 0)
				printf("  ");
			printf("%s", n -> unit_name);
			if(strcmp(n -> unit_name, "INT") == 0)
				printf(": %d\n", *(int *)n -> unit_value);
			else if(strcmp(n -> unit_name, "FLOAT") == 0)
				printf(": %f\n", *(float *)n -> unit_value);
			else if(strcmp(n -> unit_name, "ID") == 0)
				printf(": %s\n", (char *)n -> unit_value);
			else if(strcmp(n -> unit_name, "TYPE") == 0)
				printf(": %s\n", (char *)n -> unit_value);
			else
				printf("\n");
			break;
		case false: /*non terminals*/
			if( n -> child == NULL) /*empty node*/
				return;
			while(level -- > 0)
				printf("  ");
			printf("%s (%d)\n", n -> unit_name, n -> lineno);
			break;
		default: assert(0);
	}
}

/*destructors*/
void
destroy_tree(struct tree_node *root){
	/*post order*/
	struct tree_node *child;
	for (child = root -> child; child != NULL; child = child -> sibling)
		destroy_tree(child);
	destroy_node(root);

}

void
destroy_node(struct tree_node *n){
	free(n -> unit_value);
	free(n);
}


/*some additional functions*/
struct tree_node*
find_id_node_in_vardec(struct tree_node *vardec_node){
	assert(vardec_node -> unit_code == VarDec);
	struct tree_node* p = vardec_node -> child;
	while(p -> unit_code != ID)
		p = p -> child;
	return p;
}