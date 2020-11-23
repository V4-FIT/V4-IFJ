/**
 * @file parser.h
 * @brief Syntax analysis interface
 */

#ifndef IFJ_PARSER_H
#define IFJ_PARSER_H

#include <stdio.h>
#include "scanner.h"

int parse_expr(scanner_t scanner);
int parse(FILE *stream);

#endif // !IFJ_PARSER_H
