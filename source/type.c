#include <assert.h>

#include "type.h"
#include "tree.h"
#include "common/tokenset.h"

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

/*type code to string*/
char*
type_ctos(int type_code){
	switch(type_code){
		case TYPE_INT: return "int";
		case TYPE_FLOAT: return "float";
		case TYPE_STRUCT: return "struct";
		case TYPE_ARRAY: return "array";
		case TYPE_STRUCT_ARRAY: return "struct array";
		default: break;
	}
	assert(0);
}
