/**
 * @file generator.h
 * @author Adrián Kálazi
 * @brief IFJcode20 generator
 * @date 2020-12-08
 *
 */

#ifndef IFJ_GENERATOR_H
#define IFJ_GENERATOR_H

#include "symtable.h"

/// Helper functions

/**
 * @brief Compose immerstion string
 *
 * @param basestr
 * @param counter
 * @return char* ... or NULL if calloc failed
 */
char *compose_immersion_string(const char *basestr, unsigned long counter);

/// Misc

/**
 * @brief Initialize code generation - create header and define builtin functions
 *
 */
void gen_init();

/**
 * @brief Finalize code generation and clean up
 *
 */
void gen_finish();


/// Function definition

/**
 * @brief Start function definition
 *
 * @param identifier Function identifier
 */
void gen_func_begin(const char *identifier);

/**
 * @brief Generate function parameters
 *
 * @param symbol_ref
 */
void gen_func_param(symbol_ref_t symbol_ref);

/**
 * @brief End function definition
 *
 * @param identifier Function identifier
 */
void gen_func_end(const char *identifier);

/// Function call

/**
 * @brief Generate function call
 *
 * @param identifier Function identifier
 */
void gen_func_call(const char *identifier);

/**
 * @brief Push function call argument onto argument stack
 *
 * @param symtable Symbol table
 * @param token Literal or identifier token
 */
void gen_func_call_arg(symtable_t symtable, token_t token);

/**
 * @brief Generates the fucking function return
 *
 * @param immersion
 * @param elseid
 */
void gen_func_return();

/// Variable

/**
 * @brief Generate variable definition
 *
 * @param symbol_ref
 */
void gen_var_define(symbol_ref_t symbol_ref);

/**
 * @brief Pushes the literal/identifier onto the stack for further computation
 *
 * @param symtable Symbol table
 * @param token Literal or identifier token
 */
void gen_var_load(symtable_t symtable, token_t token);

/**
 * @brief Pushes the identifier onto the stack, before the last pushed item for further computation
 *
 * @param symbol_ref Identifier name
 */
void gen_var_load_id_before(symbol_ref_t symbol_ref);

/**
 * @brief Assigns expression result to a frame variable
 *
 * @param symbol_ref Identifier name
 */
void gen_var_assign_expr_result(hmap_t assign_ids_map, symbol_ref_t symbol_ref);

/**
 * @brief Generates a unary operator specific instruction(s)
 *
 * @param operator token type, one of {+ - !}
 * @param data_type data token type
 */
void gen_var_operator_unary(token_type_t operator, data_type_t data_type);

/**
 * @brief Generates a binary operator specific instruction
 *
 * @param operator token type, one of {+ - * / == != <= < >= > && ||}
 * @param data_type data token type
 */
void gen_var_operator_binary(token_type_t operator, data_type_t data_type);

/// Conditional jumps

/**
 * @brief Generate label end
 *
 * @param immersion
 */
void gen_label_end(flist_iterator_t immersion);

/**
 * @brief Generate jump condition end
 *
 * @param immersion
 */
void gen_jump_cond_end(flist_iterator_t immersion);

/// for

/**
 * @brief Generate for cycle condition label
 *
 * @param immersion
 */
void gen_for_label_condition(flist_iterator_t immersion);

/**
 * @brief Generate for cycle assignment label
 *
 * @param immersion
 */
void gen_for_label_assignment(flist_iterator_t immersion);

/**
 * @brief Generate for cycle content label
 *
 * @param immersion
 */
void gen_for_label_content(flist_iterator_t immersion);

/**
 * @brief Generate for cycle jump condition
 *
 * @param immersion
 */
void gen_for_jump_condition(flist_iterator_t immersion);

/**
 * @brief Generate for cycle assignment
 *
 * @param immersion
 */
void gen_for_jump_assignment(flist_iterator_t immersion);

/**
 * @brief Generate for cycle content
 *
 * @param immersion
 */
void gen_for_jump_content(flist_iterator_t immersion);

/// if

/**
 * @brief Generate if label finish
 *
 * @param immersion
 * @param elseid
 */
void gen_if_label_finish(flist_iterator_t immersion, unsigned long elseid);

/**
 * @brief Generate if jump finish
 *
 * @param immersion
 * @param elseid
 */
void gen_if_jump_finish(flist_iterator_t immersion, unsigned long elseid);

#endif //IFJ_GENERATOR_H
