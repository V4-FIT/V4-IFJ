#ifndef IFJ_SEMANTICS_H
#define IFJ_SEMANTICS_H

#include "parser.h"

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

// check if variable has been defined
int sem_var_check(parser_t parser);

#endif // !IFJ_SEMANTICS_H
