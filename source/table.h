#ifndef __TABLE_H__
#define __TABLE_H__

#define TYPE_INT 0
#define TYPE_FLOAT 1
#define TYPE_ARRAY 2
#define TYPE_STRUCT 3

struct func_symbol{
	char func_name[20];
	int return_type;	//functions can only return basic type
	int param_num;

	struct func_symbol* next;
};

extern struct func_symbol* func_list_head;

struct func_symbol* create_func_symbol(char*, int, int);

void insert_into_func_list(struct func_symbol *);


#endif