#include <assert.h>

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
	//remove temp like t = XXX v = t 
	//reverse order remove unused var
}