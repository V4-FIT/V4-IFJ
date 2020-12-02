#ifndef IFJ_SEMANTICS_H
#define IFJ_SEMANTICS_H

#include "parser.h"

// pass1 - defines a functions and checks for redefinition
int sem_define_func(parser_t parser);

// pass2 - inserts function parameters into the symbol table
int sem_func_declare_param(parser_t parser);

// pass1 - add parameter types to the function signature
int sem_func_add_param_type(parser_t parser);

// pass1 - add return types to the function signature 
int sem_func_add_return_type(parser_t parser);

// checks if the function is callable (defined)
int sem_func_callable(parser_t parser);

#endif // !IFJ_SEMANTICS_H
