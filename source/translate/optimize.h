#ifndef __OPTIMIZE_H__
#define __OPTIMIZE_H__

#include "intercode.h"

void optimize_func(struct intercode*, struct intercode*);

void reduce_temp(struct intercode*);

void reduce_calculate(struct intercode*);

void remove_intercode(struct intercode* );

void exchange_op1(struct intercode*, struct intercode*);

#endif