#ifndef __TABLE_H__
#define __TABLE_H__

#include "type.h"
#include "tree.h"

struct func_descriptor{
	char func_name[20];
	struct type_descriptor* return_type;
	int param_num;
	struct var_descriptor* param_list_head;	//params will be treated as global variables
	struct func_descriptor* next;
};

struct struct_descriptor{
	char struct_name[20];
	int member_num;
	struct var_descriptor* member_list_head;
	struct struct_descriptor* next;
};

/*int float struct and array*/

struct var_descriptor{

	char var_name[20];
	struct type_descriptor *var_type;		//the type of a variable can be int\float\struct (cannot be null)
	struct array_descriptor *var_array;		//the array of the variable likes [10][20][30]...	(may be null)

	bool is_lvalue;
	struct var_descriptor* next;
};

/*heads of three tables*/
extern struct func_descriptor* func_table_head;
extern struct struct_descriptor* struct_table_head;
extern struct var_descriptor* var_table_head;

/*func table related functions*/
struct func_descriptor* create_func_descriptor(char*, struct type_descriptor*, int);
struct func_descriptor* find_func(struct func_descriptor*, char *);
void add_func(struct func_descriptor*, struct func_descriptor* );
void print_func_table(struct func_descriptor* );


/*struct table related functions*/
struct struct_descriptor* create_struct_descriptor(char*);
struct struct_descriptor* find_struct(struct struct_descriptor*, char*);
void add_struct(struct struct_descriptor*, struct struct_descriptor*);
void print_struct_table(struct struct_descriptor*);

/*var table related functions*/
struct var_descriptor* create_var_descriptor(char*, struct type_descriptor*, struct array_descriptor*);
struct var_descriptor* find_var(struct var_descriptor*, char*);
void add_var(struct var_descriptor*, struct var_descriptor*);
void print_var_table(struct var_descriptor*);

int find_var_seq(struct var_descriptor*, char*);

int calculate_struct_size(struct struct_descriptor* sd);
int calculate_var_size(struct var_descriptor* vd);
int calculate_member_offset(struct struct_descriptor*, char*);


#endif