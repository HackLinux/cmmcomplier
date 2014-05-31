#ifndef __OPTIMIZE_H__
#define __OPTIMIZE_H__

#include "intercode.h"
#include "../common/bool.h"


void optimize_func(struct intercode*, struct intercode*);

bool reduce_temp(struct intercode*);

bool reduce_calculate(struct intercode*);

bool if_to_ifFalse(struct intercode*);

void remove_intercode(struct intercode* );

void exchange_op1(struct intercode*, struct intercode*);

#endif