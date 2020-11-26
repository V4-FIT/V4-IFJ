#ifndef IFJ_GENERATOR_H
#define IFJ_GENERATOR_H

#include "symtable.h"

void gen_init();

void gen_func_begin(const char *identifier);
void gen_func_end();

void gen_finish();

#endif //IFJ_GENERATOR_H
