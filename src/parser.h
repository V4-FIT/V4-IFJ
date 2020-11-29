/**
 * @file parser.h
 * @brief Syntax analysis interface
 */

#ifndef IFJ_PARSER_H
#define IFJ_PARSER_H

#include <stdio.h>

#include "tokens.h"
#include "symtable.h"

typedef struct parser
{
	tklist_t tklist;
	symtable_t symtable;
	token_t token;
} * parser_t;

parser_t parser_init(tklist_t tklist);

int parser_parse(tklist_t tklist);

void parser_free(parser_t parser);

#endif // !IFJ_PARSER_H
