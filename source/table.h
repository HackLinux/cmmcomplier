#ifndef __TABLE_H__
#define __TABLE_H__

#define TYPE_INT 1
#define TYPE_FLOAT 2
#define TYPE_STRUCT 3
#define TYPE_ARRAY 4
#define TYPE_STRUCT_ARRAY 5

struct func_descriptor{
	char func_name[20];
	int return_type;	//functions can only return basic type
	int param_num;
	//struct var_descriptor* param_list_head;	//params will be treated as global variables
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
	int type_code;
	char var_name[20];
	struct struct_descriptor* sd;	//struct variables have this and sd -> next is null
	struct array_type* at;			//array variables have this. it is a list as long as the dimension of the array
									//struct array have both
	struct var_descriptor* next;
};

struct array_type{
	int size;
	int final_type_code;	// when subtype = null , this field is used
	struct array_type *subtype;
};

/*heads of three tables*/
extern struct func_descriptor* func_table_head;
extern struct struct_descriptor* struct_table_head;
extern struct var_descriptor* var_table_head;

/*func table related functions*/
struct func_descriptor* create_func_descriptor(char*, int, int);
struct func_descriptor* find_func(struct func_descriptor*, char *);
void add_func(struct func_descriptor*, struct func_descriptor* );
void print_func_table(struct func_descriptor* );

/*struct table related functions*/

/*var table related functions*/

#endif