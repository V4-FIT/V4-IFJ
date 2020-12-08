#ifndef IFJ_SEMANTICS_H
#define IFJ_SEMANTICS_H

#include "parser.h"

// defined in precedence.h
typedef struct prec_stack *prec_stack_t;

// pass1 - defines all the builting functions
int sem_define_builtin_functions(parser_t parser);

// Creates a new scope for storing symbols
int sem_enter_scope(parser_t parser);

// Exits the current scope
int sem_exit_scope(parser_t parser);

// pass1 - defines a functions and checks for redefinition
int sem_func_define(parser_t parser);

// pass2 - inserts function parameters into the symbol table
int sem_func_declare_param(parser_t parser);

// pass1 - add parameter types to the function signature
int sem_func_add_param_type(parser_t parser);

// pass1 - add return types to the function signature
int sem_func_add_return_type(parser_t parser);

// checks if a function with a returns had a return statement at the end
int sem_func_has_return_stmt(parser_t parser);

// check if the function is callable (defined)
int sem_func_callable(parser_t parser);

// check if the function main was defined and if done so with no parameters or returns
int sem_main_defined(parser_t parser);

// define variable in current scope and check for redefinition
int sem_var_define(parser_t parser);

// deduce variable type from expression
int sem_var_define_type(parser_t parser);

// check if variable has been defined
int sem_var_check(parser_t parser);

// call in function definition before stamtements
int sem_func_stmts_begin(parser_t parser);

// init define statmenet
int sem_define_begin(parser_t parser);

// init assign statmenet
int sem_assign_begin(parser_t parser);

// init call statmenet
int sem_call_begin(parser_t parser);

// init conditional statmenet
int sem_conditional_begin(parser_t parser);

// init iterative statmenet
int sem_iterative_begin(parser_t parser);

// init return statmenet
int sem_return_begin(parser_t parser);

// init semantics for expressions
int sem_expression_begin(parser_t parser);

// check type compatibility around a binary operation
int sem_binary_op_type_compat(parser_t parser, prec_stack_t *head);

// check type compatibility around a logical operation
int sem_logical_op_type_compat(parser_t parser, prec_stack_t *head);

// cheack unary operator type compatibility
int sem_unary_op_type_compat(parser_t parser, prec_stack_t *head);

// set expression return values for semantic purposes
int sem_prec_rule_exit(parser_t parser, prec_stack_t *head);

// evauluate binary constant expression value
int sem_evaulate_binary_const_expr(parser_t parser, prec_stack_t *head);

// evauluate unary constant expression value
int sem_evaulate_unary_const_expr(parser_t parser, prec_stack_t *head);

// check against zero dvision
int sem_zero_division(parser_t parser, prec_stack_t *head);

// check if condition expression is of type bool
int sem_bool_condition(parser_t parser);

// semantic action for ids
int sem_id_begin(parser_t parser);

// a function with return params need to be a part of an assignment
int sem_call_no_return(parser_t parser);

// check return expression type compatibility with function return params
int sem_return_expr_type_compat(parser_t parser);

// check if return statement expression count matches the function return count
int sem_return_expr_count(parser_t parser);

// semantic action at the beginning of rule_arguments
int sem_arguments_begin(parser_t parser);

// semantic action for arguments (calls sem_var_check for identifiers)
int sem_argument_begin(parser_t parser);

// check if function call argument count matches called function param count
int sem_call_argument_count(parser_t parser);

// check called function return against assignment list
int sem_assignment_call_return(parser_t parser);

// check expression type against assign into variable type
int sem_assign_expr_type_compat(parser_t parser);

// check expr count matches ids count
int sem_assign_expr_count(parser_t parser);

// check against zero div
int sem_assign_zero_div(parser_t parser);

#endif // !IFJ_SEMANTICS_H
