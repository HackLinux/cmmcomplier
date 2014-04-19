#ifndef __TYPE_H__
#define __TYPE_H__

#define TYPE_INT 1
#define TYPE_FLOAT 2
#define TYPE_STRUCT 3
//#define TYPE_ARRAY 4
//#define TYPE_STRUCT_ARRAY 5


struct array_descriptor{
	int size;
	struct array_descriptor *subarray;
};

struct type_descriptor{
	int type_code;
	struct struct_descriptor* sd;	//when type_code = STRUCT, this field become effective
};

#include "tree.h"
//#include "table.h"	// this cannot move upside

struct type_descriptor* create_type_descriptor(int, struct struct_descriptor *);
struct type_descriptor* create_type_descriptor_by_specifier(struct tree_node*);
int get_type_code_from_specifier(struct tree_node*);
char* type_ctos(int);

#endif