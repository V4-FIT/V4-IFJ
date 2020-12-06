#ifndef IFJ_GENERATOR_H
#define IFJ_GENERATOR_H

#include "symtable.h"

/// Misc
void gen_init();
void gen_finish();

/// Function definition
void gen_func_begin(const char *identifier);
void gen_func_param(const char *identifier);
void gen_func_end();

/// Function call
void gen_func_call(const char *identifier);
void gen_func_call_arg(token_t token);

/// Variable
void gen_var_define(const char *identifier);
void gen_var_load(token_t token);
void gen_var_assign_expr_result(const char *identifier);
void gen_var_operator_unary(token_type_t operator, data_type_t data_type);
void gen_var_operator_binary(token_type_t operator, data_type_t data_type);
void gen_var_neg(data_type_t type);
void gen_var_not();

#endif //IFJ_GENERATOR_H
