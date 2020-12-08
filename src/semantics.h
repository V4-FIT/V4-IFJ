/**
 * @file semantics.h
 * @author Kevin Lackó, Adrián Kálazi
 * @brief Parser interface for sematic analysis
 * @date 2020-12-08
 *
 */

#ifndef IFJ_SEMANTICS_H
#define IFJ_SEMANTICS_H

#include "parser.h"

// defined in precedence.h
typedef struct prec_stack *prec_stack_t;

/**
 * @brief Defines all builting functions
 *
 * @param parser
 * @return int
 */
int sem_define_builtin_functions(parser_t parser);

/**
 * @brief Create new scope for storing symbols
 *
 * @param parser
 * @return int
 */
int sem_enter_scope(parser_t parser);

/**
 * @brief Exit current scope
 *
 * @param parser
 * @return int
 */
int sem_exit_scope(parser_t parser);

/**
 * @brief Define functions and check for redefinition
 *
 * @param parser
 * @return int
 */
int sem_func_define(parser_t parser);

/**
 * @brief Insert function parameters into the symbol table
 *
 * @param parser
 * @return int
 */
int sem_func_declare_param(parser_t parser);

/**
 * @brief Add parameter type to function signature
 *
 * @param parser
 * @return int
 */
int sem_func_add_param_type(parser_t parser);

// pass1 - add return types to the function signature
int sem_func_add_return_type(parser_t parser);

/**
 * @brief Check if function with return values has a return statement
 *
 * @param parser
 * @return int
 */
int sem_func_has_return_stmt(parser_t parser);

/**
 * @brief Check if function can be called (is defined)
 *
 * @param parser
 * @return int
 */
int sem_func_callable(parser_t parser);

/**
 * @brief Check if function main is defined and is defined correctly
 *
 * @param parser
 * @return int
 */
int sem_main_defined(parser_t parser);

/**
 * @brief Define variable in current scope and check for redefinition
 *
 * @param parser
 * @return int
 */
int sem_var_define(parser_t parser);

/**
 * @brief Determine variable type
 *
 * @param parser
 * @return int
 */
int sem_var_define_type(parser_t parser);

/**
 * @brief Check for variable definition
 *
 * @param parser
 * @return int
 */
int sem_var_check(parser_t parser);

/**
 * @brief Call in function definition before stamtements
 *
 * @param parser
 * @return int
 */
int sem_func_stmts_begin(parser_t parser);

/**
 * @brief Initialize definition statement
 *
 * @param parser
 * @return int
 */
int sem_define_begin(parser_t parser);

/**
 * @brief Initialize assignment statement
 *
 * @param parser
 * @return int
 */
int sem_assign_begin(parser_t parser);

/**
 * @brief Initialize call statement
 *
 * @param parser
 * @return int
 */
int sem_call_begin(parser_t parser);

/**
 * @brief Initialize conditional statement
 *
 * @param parser
 * @return int
 */
int sem_conditional_begin(parser_t parser);

/**
 * @brief Initialize iterative statement
 *
 * @param parser
 * @return int
 */
int sem_iterative_begin(parser_t parser);

/**
 * @brief Initialize return statement
 *
 * @param parser
 * @return int
 */
int sem_return_begin(parser_t parser);

/**
 * @brief Initialize expression
 *
 * @param parser
 * @return int
 */
int sem_expression_begin(parser_t parser);

/**
 * @brief Check operator and operand compatibility
 *
 * @param parser
 * @param head
 * @return int
 */
int sem_binary_op_type_compat(parser_t parser, prec_stack_t *head);

/**
 * @brief Check logical operator and operand compatibility
 *
 * @param parser
 * @param head
 * @return int
 */
int sem_logical_op_type_compat(parser_t parser, prec_stack_t *head);

/**
 * @brief Check unary operator and operand compatibility
 *
 * @param parser
 * @param head
 * @return int
 */
int sem_unary_op_type_compat(parser_t parser, prec_stack_t *head);

/**
 * @brief Determine expression return values
 *
 * @param parser
 * @param head
 * @return int
 */
int sem_prec_rule_exit(parser_t parser, prec_stack_t *head);

/**
 * @brief Determine binary constant expression value
 *
 * @param parser
 * @param head
 * @return int
 */
int sem_evaulate_binary_const_expr(parser_t parser, prec_stack_t *head);

/**
 * @brief Determine unary constant expression value
 *
 * @param parser
 * @param head
 * @return int
 */
int sem_evaulate_unary_const_expr(parser_t parser, prec_stack_t *head);

/**
 * @brief Check zero division
 *
 * @param parser
 * @param head
 * @return int
 */
int sem_zero_division(parser_t parser, prec_stack_t *head);

/**
 * @brief Check expression type in condition
 *
 * @param parser
 * @return int
 */
int sem_bool_condition(parser_t parser);

/**
 * @brief Semantic action for identificators
 *
 * @param parser
 * @return int
 */
int sem_id_begin(parser_t parser);

/**
 * @brief Check if function with return parameters is part of assignment statement
 *
 * @param parser
 * @return int
 */
int sem_call_no_return(parser_t parser);

/**
 * @brief Check return expression type compatibility with function return parameters
 *
 * @param parser
 * @return int
 */
int sem_return_expr_type_compat(parser_t parser);

/**
 * @brief Check if return statement expression count matches the function return count
 *
 * @param parser
 * @return int
 */
int sem_return_expr_count(parser_t parser);

/**
 * @brief Semantic action at the beginning of rule_arguments
 *
 * @param parser
 * @return int
 */
int sem_arguments_begin(parser_t parser);

/**
 * @brief Semantic action for arguments (calls sem_var_check for identifiers)
 *
 * @param parser
 * @return int
 */
int sem_argument_begin(parser_t parser);

/**
 * @brief Check if function call argument count matches called function param count
 *
 * @param parser
 * @return int
 */
int sem_call_argument_count(parser_t parser);

/**
 * @brief Check called function return against assignment list
 *
 * @param parser
 * @return int
 */
int sem_assignment_call_return(parser_t parser);

/**
 * @brief Check if expression type against assign into variable type
 *
 * @param parser
 * @return int
 */
int sem_assign_expr_type_compat(parser_t parser);

/**
 * @brief Check if exprression count matches identifier count
 *
 * @param parser
 * @return int
 */
int sem_assign_expr_count(parser_t parser);

/**
 * @brief Check zero division
 *
 * @param parser
 * @return int
 */
int sem_assign_zero_div(parser_t parser);

#endif // !IFJ_SEMANTICS_H
