#ifndef IFJ_SEMANTICS_H
#define IFJ_SEMANTICS_H

#include "parser.h"

int sem_define_func(parser_t parser);

int sem_func_add_param(parser_t parser);

int sem_func_add_return(parser_t parser);

#endif // !IFJ_SEMANTICS_H
