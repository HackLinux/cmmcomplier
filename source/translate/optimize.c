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

	struct intercode* p = start_ic -> next;
	while(p != end_ic){

		/*two assign intercode*/
		if (p -> type == IC_ASSIGN || p -> type == IC_ADD ||
			p -> type == IC_SUB || p -> type == IC_MUL ||
			p -> type == IC_DIV || p -> type == IC_CALL ||
			p -> type == IC_READ || p -> type == IC_ASSIGN){
			struct intercode* pn = p -> next;
			if(pn -> type == IC_ASSIGN){
				reduce_temp(p);
			}
		}

		/*two calculate interocde*/
		if( p -> type == IC_ADD || p -> type == IC_SUB ||
			p -> type == IC_DIV || p -> type == IC_MUL){
			struct intercode* pn = p -> next;
			if( pn -> type == IC_ADD || pn -> type == IC_SUB ||
				pn -> type == IC_DIV || pn -> type == IC_MUL){
				reduce_calculate(p);
			}
		}

		p = p -> next;
	}

}

/*
ic1	:		t1 = XXX
ic1 next:	XX = t1
*/
void
reduce_temp(struct intercode* ic1){
	assert(ic1 -> op1 != NULL);
	assert(ic1 -> next -> type == IC_ASSIGN);

	struct intercode* ic2 = ic1 -> next;
	if(ic2 -> op1 -> star_num != 0)
		return ;	//ic2 : *t1 = XXX

	if(ic1 -> op1 -> type == OP_TEMP){
		
		if(operand_equal(ic1 -> op1, ic2 -> op2)){
		
			//exchange ic1 -> op1 with ic2 -> op1
			exchange_op1(ic1, ic2);
			
			//remove ic2 from list
			remove_intercode(ic2);
		}
	}
}

/*
ic1:		t1 = x [operator] y
ic1 next:	t2 = t1 [operator] z
if y and z are both constant, ic1 and ic1 next can be reduced as one
*/
void
reduce_calculate(struct intercode* ic1){
	struct intercode* ic2 = ic1 -> next;

	if(ic1 -> op1 -> type != OP_TEMP)
		return;
	if(!operand_equal(ic1 -> op1, ic2 -> op2))
		return;
	if(ic1 -> op3 -> type != OP_CONST || ic2 -> op3 -> type != OP_CONST)
		return;

	//two plus/minus
	if(ic1 -> type == IC_ADD || ic1 -> type == IC_SUB){
		if(ic2 -> type == ic1 -> type){
			ic1 -> op3 -> value += ic2 -> op3 -> value;
			
			exchange_op1(ic1, ic2);
			remove_intercode(ic2);
		}
		else if(ic2 -> type == IC_ADD || ic2 -> type == IC_SUB){
			ic1 -> op3 -> value -= ic2 -> op3 -> value;

			exchange_op1(ic1, ic2);
			remove_intercode(ic2);
		}
	}
	
	//two multiply/divide
	if(ic1 -> type == IC_MUL || ic1 -> type == IC_DIV){
		if(ic2 -> type == ic1 -> type){
			ic1 -> op3 -> value *= ic2 -> op3 -> value;
			
			exchange_op1(ic1, ic2);
			remove_intercode(ic2);
		}
	}
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

void
exchange_op1(struct intercode* ic1, struct intercode* ic2){
	struct operand* temp = ic1 -> op1;
	ic1 -> op1 = ic2 -> op1;
	ic2 -> op1 = temp;
}