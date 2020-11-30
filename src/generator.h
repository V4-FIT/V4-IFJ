#ifndef IFJ_GENERATOR_H
#define IFJ_GENERATOR_H

#include "symtable.h"

void gen_init();

void gen_func_begin(const char *identifier);
void gen_func_init_frame();
void gen_func_init_stack();
void gen_func_param(const char *identifier);
void gen_func_call(const char *identifier);
void gen_func_callarg_literal(token_t token);
void gen_func_end();

void gen_finish();

#endif //IFJ_GENERATOR_H
