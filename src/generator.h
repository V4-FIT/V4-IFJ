#ifndef IFJ_GENERATOR_H
#define IFJ_GENERATOR_H

#include "symtable.h"

/// Helper functions
char *compose_immersion_string(const char *basestr, unsigned long counter);

/// Misc
void gen_init();
void gen_finish();

/// Function definition
void gen_func_begin(const char *identifier);
void gen_func_param(symbol_ref_t symbol_ref);
void gen_func_end(const char *identifier);

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

/// for
void gen_for_label_condition(flist_iterator_t immersion);
void gen_for_label_assignment(flist_iterator_t immersion);
void gen_for_label_content(flist_iterator_t immersion);
void gen_for_label_end(flist_iterator_t immersion);
void gen_for_jump_condition(flist_iterator_t immersion);
void gen_for_jump_assignment(flist_iterator_t immersion);
void gen_for_jump_content(flist_iterator_t immersion);
void gen_for_jump_cond_end(flist_iterator_t immersion);

/// Condition
void gen_if_label_finish(flist_iterator_t immersion, unsigned long elseid);
void gen_if_jump_finish(flist_iterator_t immersion, unsigned long elseid);

#endif //IFJ_GENERATOR_H
