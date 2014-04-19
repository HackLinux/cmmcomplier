#ifndef __TYPE_H__
#define __TYPE_H__

#define TYPE_INT 1
#define TYPE_FLOAT 2
#define TYPE_STRUCT 3
#define TYPE_ARRAY 4
#define TYPE_STRUCT_ARRAY 5

//todo type descriptor ????

struct array_type{
	int size;
	int final_type_code;	// when subtype = null , this field is used
	struct array_type *subtype;
};

struct type_descriptor{
	int type_code;
	struct array_type* at;
	struct struct_descriptor* sd;
};

#include "tree.h"
//#include "table.h"	// this cannot move upside

struct type_descriptor* create_type_descriptor(int, struct array_type *, struct struct_descriptor *);
int get_type_code_from_specifier(struct tree_node*);
char* type_ctos(int);

#endif