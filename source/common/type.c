#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bool.h"
#include "tree.h"
#include "table.h"
#include "tokenset.h"
#include "type.h"
#include "../semantic/semantic.h"

/******************************functions about type(int\float\struct)***********************************/
struct type_descriptor*
create_type_descriptor(int type_code, struct struct_descriptor *sd){
	struct type_descriptor* new_type = (struct type_descriptor*)malloc(sizeof(struct type_descriptor));
	new_type -> type_code = type_code;
	new_type -> sd = sd;
	return new_type;
}

int 
get_type_code_from_specifier(struct tree_node* node){
	assert(node -> unit_code == Specifier);
	int type_code;
	switch(node -> child -> unit_code){
		case TYPE:
			type_code = strcmp(node -> child -> unit_value, "int") == 0 ? TYPE_INT : TYPE_FLOAT;
			break;
		case StructSpecifier:
			type_code = TYPE_STRUCT;
			break;
		default: assert(0); 
	}
	return type_code;
}

bool
is_struct_type(struct tree_node* specifier_node){
	assert(specifier_node -> unit_code == Specifier);
	if (specifier_node -> child -> unit_code == StructSpecifier)
		return true;	//Specifier -> StructSpecifier
	return false;		//Specifier -> TYPE
}

bool
is_struct_definition(struct tree_node* structspecifier_node){
	assert(structspecifier_node -> unit_code == StructSpecifier);
	if (structspecifier_node -> child -> sibling -> unit_code == OptTag)
		return true;	//STRUCT OptTag LC DefList RC
	return false;		//STRUCT Tag
}

/*type code to string*/
char*
type_ctos(int type_code){
	switch(type_code){
		case TYPE_INT: return "int";
		case TYPE_FLOAT: return "float";
		case TYPE_STRUCT: return "struct";
		default: break;
	}
	assert(0);
}

/*determine two types equal or not*/
bool
type_equal(struct type_descriptor* t1, struct type_descriptor* t2){
	assert(t1 != NULL && t2 != NULL);
	if(t1 -> type_code != t2 -> type_code)
		return false;
	if(t1 -> type_code == TYPE_STRUCT){
		char* struct1_name = t1 -> sd -> struct_name;
		char* strcuct2_name = t2 -> sd -> struct_name;
		//todo : field equal ??
		return ((strcmp(struct1_name, strcuct2_name) == 0) ? true : false);
	}
	return true;
}


/*********************************functions about array***************************************/
struct array_descriptor*
create_array_descriptor(int size, struct array_descriptor* subarray){
	struct array_descriptor *new_array = (struct array_descriptor*)malloc(sizeof(struct array_descriptor));
	new_array -> size = size;
	new_array -> subarray = subarray;
	return new_array; 
}

struct array_descriptor*
create_array_descriptor_by_vardec(struct tree_node* vardec_node){
	struct array_descriptor *head = create_array_descriptor(-1, NULL);
	while(vardec_node -> child -> unit_code == VarDec){
		vardec_node = vardec_node -> child;
		int size = *(int *)(vardec_node -> sibling -> sibling -> unit_value);
		struct array_descriptor* new_array = create_array_descriptor(size, head -> subarray);
		head -> subarray = new_array;
	}
	
	return head -> subarray;
}

bool
array_equal(struct array_descriptor* a1, struct array_descriptor* a2){
	if(a1 == NULL && a2 == NULL)
		return true;
	if(a1 == NULL || a2 == NULL)
		return false;
	return array_equal(a1 -> subarray, a2 -> subarray);
}
