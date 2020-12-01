#ifndef IFJ_GENERATOR_H
#define IFJ_GENERATOR_H

#include "symtable.h"

/// Misc
void gen_init();
void gen_finish();

/// Function definition
void gen_func_begin(const char *identifier);
void gen_func_init_frame();
void gen_func_init_stack();
void gen_func_param(const char *identifier);
void gen_func_end();

/// Function call
void gen_func_call(const char *identifier);
void gen_func_call_arg_literal(token_t token);

#endif //IFJ_GENERATOR_H
