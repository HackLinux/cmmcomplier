#include <assert.h>
#include <stdlib.h>

#include "tree.h"
#include "table.h"
#include "common/tokenset.h"
#include "type.h"

struct type_descriptor*
create_type_descriptor(int type_code, struct struct_descriptor *sd){
	struct type_descriptor* new_type = (struct type_descriptor*)malloc(sizeof(struct type_descriptor));
	new_type -> type_code = type_code;
	new_type -> sd = sd;
}

struct type_descriptor*
create_type_descriptor_by_specifier(struct tree_node* specifier_node){
	struct type_descriptor* new_type;
	int type_code = get_type_code_from_specifier(specifier_node);
	if (type_code == TYPE_INT || type_code == TYPE_FLOAT){
		//Specifier -> TYPE
		new_type = create_type_descriptor(type_code, NULL);
		return new_type;
	}
	//Specifier -> StructSpecifier
	assert(type_code == TYPE_STRUCT);
	struct tree_node* structspecifier_node = specifier_node -> child;
	struct struct_descriptor* sd;
	if(structspecifier_node -> child -> sibling -> unit_code == Tag){
		//StructSpecifier -> Struct Tag (exist struct)
		//Tag -> ID
		char *struct_name = structspecifier_node -> child -> sibling -> child -> unit_value;
		sd = find_struct(struct_table_head, struct_name);
		if(sd == NULL){
			//semantic error: undefined struct struct_name
			return NULL;
		}
	} else{
		//StructSpecifier -> Struct OptTag LC DefList RC(new struct)
		sd = create_struct_descriptor_by_structspecifier(structspecifier_node);
		//todo : create failed by name repeat or assignop
		add_struct(struct_table_head, sd);
	}
	new_type = create_type_descriptor(type_code, sd);
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

/*type code to string*/
char*
type_ctos(int type_code){
	switch(type_code){
		case TYPE_INT: return "int";
		case TYPE_FLOAT: return "float";
		case TYPE_STRUCT: return "struct";
		//case TYPE_ARRAY: return "array";
		//case TYPE_STRUCT_ARRAY: return "struct array";
		default: break;
	}
	assert(0);
}
