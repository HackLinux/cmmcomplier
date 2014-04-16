/*symbol table related functions used in semantic analysis*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "table.h"

struct func_symbol *func_list_head = NULL;

struct func_symbol *
create_func_symbol(char *func_name, int return_type, int param_num){

	assert(strlen(func_name) < 20);

	struct func_symbol *new_func_symbol = (struct func_symbol *)malloc(sizeof(struct func_symbol));
	strcpy(new_func_symbol -> func_name, func_name);
	new_func_symbol -> return_type = return_type;
	new_func_symbol -> param_num = param_num;
	new_func_symbol -> next = NULL;

	return new_func_symbol;
}

void 
insert_into_func_list(struct func_symbol *new_func_symbol){
	if(func_list_head == NULL)
		func_list_head = new_func_symbol;
	else{
		new_func_symbol -> next = func_list_head;
		func_list_head = new_func_symbol;
	}
}

void
print_func_table(struct func_symbol* head){
	int i = 0;
	struct func_symbol* p;
	printf("function table :\n");
	printf("+============================================================+\n");
	printf("%29s", "function name");
	printf("%15s", "return type");
	printf("%17s", "param numbers");
	for ( p = head; p != NULL; p = p -> next){
		printf("\n %d\t ", i++);
		printf("%20s", p -> func_name);
		printf("%15s", (p -> return_type == TYPE_INT)? "int" : "float");
		printf("%17d", p -> param_num);
	}
	printf("\n+============================================================+\n");
}