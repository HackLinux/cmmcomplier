#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "optimize.h"
#include "intercode.h"
#include "operand.h"

void
optimize_func(struct intercode* start_ic, struct intercode* end_ic){
	assert(start_ic -> type == IC_FUNC);
	assert(end_ic -> type == IC_RETURN);
	//todo 
	//remove neighbour label
	//remove no use code after return
	//remove unused label
	//reverse order remove unused var

	struct intercode* p = start_ic -> next;
	while(p != end_ic){

		/*two assign intercode*/
		if (p -> type == IC_ASSIGN || p -> type == IC_ADD ||
			p -> type == IC_SUB || p -> type == IC_MUL ||
			p -> type == IC_DIV || p -> type == IC_CALL ||
			p -> type == IC_READ || p -> type == IC_ASSIGN){
			struct intercode* pn = p -> next;	//next intercode after p
			if(pn -> type == IC_ASSIGN){
				reduce_temp(p);
			}
		}

		p = p -> next;
	}

}

/*
ic1	:		t1 = ***
ic1 next:	** = t1
*/
void
reduce_temp(struct intercode* ic1){
	assert(ic1 -> op1 != NULL);
	assert(ic1 -> next -> type == IC_ASSIGN);

	struct intercode* ic2 = ic1 -> next;

	if(ic1 -> op1 -> type == OP_TEMP){
		if(operand_equal(ic1 -> op1, ic2 -> op2)){
			
			//exchange ic1 -> op1 with ic2 -> op1
			struct operand* temp = ic1 -> op1;
			ic1 -> op1 = ic2 -> op1;
			ic2 -> op1 = temp;

			//remove ic2 from list
			remove_intercode(ic2);
		}
	}
}

/*
ic1 :		LABEL label*
ic1 next : 	LABEL label**
*/
void
reduce_lable(struct intercode* ic1){
	
}


/*remove an intercode out of list*/
void
remove_intercode(struct intercode* target_ic){
	if(target_ic == NULL)
		return;
	struct intercode* prev_ic = target_ic -> prev;
	struct intercode* next_ic = target_ic -> next;
	assert(prev_ic != NULL);

	prev_ic -> next = next_ic;
	if(next_ic != NULL)
		next_ic -> prev = prev_ic;
	free(target_ic);
}