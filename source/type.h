#ifndef __TYPE_H__
#define __TYPE_H__

#include "tree.h"

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

struct type_descriptor* create_type_descriptor(int, struct struct_descriptor *);
int get_type_code_from_specifier(struct tree_node*);

bool is_struct_definition(struct tree_node*);
bool is_struct_type(struct tree_node*);
char* type_ctos(int);

struct array_descriptor* create_array_descriptor(int, struct array_descriptor*);
struct array_descriptor* create_array_descriptor_by_vardec(struct tree_node*);

bool type_equal(struct type_descriptor*, struct type_descriptor*);
bool array_equal(struct array_descriptor*, struct array_descriptor*);

#endif