#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "tree.h"
#include "table.h"
#include "common/tokenset.h"
#include "type.h"

/******************************functions about type(int\float\struct)***********************************/
struct type_descriptor*
create_type_descriptor(int type_code, struct struct_descriptor *sd){
	struct type_descriptor* new_type = (struct type_descriptor*)malloc(sizeof(struct type_descriptor));
	new_type -> type_code = type_code;
	new_type -> sd = sd;
	return new_type;
}

struct type_descriptor*
create_type_descriptor_by_specifier(struct tree_node* specifier_node){
	
	int type_code = get_type_code_from_specifier(specifier_node);
	struct struct_descriptor* sd;

	if(type_code == TYPE_INT || type_code == TYPE_FLOAT){ //Specifier -> TYPE
		sd = NULL;
	}
	if(type_code == TYPE_STRUCT){ //Specifier -> StructSpecifier

		struct tree_node* structspecifier_node = specifier_node -> child;
		if(structspecifier_node -> child -> sibling -> unit_code == Tag){	//exist struct
			char *struct_name = structspecifier_node -> child -> sibling -> child -> unit_value;
			sd = find_struct(struct_table_head, struct_name);
			assert(sd != NULL);	//if sd == null then error: no such struct
		} else{	//new struct
			sd = create_struct_descriptor_by_structspecifier(structspecifier_node);
			//todo : remove this func
			//todo : create failed by name repeat or assignop
			add_struct(struct_table_head, sd);
		}
	}
	return create_type_descriptor(type_code, sd);
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
		//case TYPE_ARRAY: return "array";
		//case TYPE_STRUCT_ARRAY: return "struct array";
		default: break;
	}
	assert(0);
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
	struct array_descriptor* p = head -> subarray;
	/*while(p != NULL){
		printf("[%d]", p -> size);
		p = p -> subarray; 
	} printf("\n");*/
	return head -> subarray;
}

/*
void
array_to_string(struct array_descriptor *array){

}*/