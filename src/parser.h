/**
 * @file parser.h
 * @brief Syntax analysis interface
 */

#ifndef IFJ_PARSER_H
#define IFJ_PARSER_H

#include <stdio.h>

#include "scanner.h"
#include "symtable.h"

typedef struct parser
{
	scanner_t scanner;
	symtable_t symtable;
	token_t token;
} * parser_t;

parser_t parser_init(FILE *stream);

int parser_parse(parser_t parser);

void parser_free(parser_t parser);

#endif // !IFJ_PARSER_H
