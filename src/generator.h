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

/// Variable
void gen_var_define(const char *identifier);
void gen_var_assign_id(const char *to, const char *from);
void gen_var_assign_literal(const char *to, tk_param_t param, token_type_t token_type);

#endif //IFJ_GENERATOR_H
