#ifndef IFJ_GENERATOR_H
#define IFJ_GENERATOR_H

#include "symtable.h"

/// Misc
void gen_init();
void gen_finish();

/// Function definition
void gen_func_begin(const char *identifier);
void gen_func_param(symbol_ref_t symbol_ref);
void gen_func_end();

/// Function call
void gen_func_call(const char *identifier);
void gen_func_call_arg(symtable_t symtable, token_t token);

/// Variable
void gen_var_define(symbol_ref_t symbol_ref);
void gen_var_load(symtable_t symtable, token_t token);
void gen_var_load_id_before(symbol_ref_t symbol_ref);
void gen_var_assign_expr_result(symbol_ref_t symbol_ref);
void gen_var_operator_unary(token_type_t operator, data_type_t data_type);
void gen_var_operator_binary(token_type_t operator, data_type_t data_type);

#endif //IFJ_GENERATOR_H
