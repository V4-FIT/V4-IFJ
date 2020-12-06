#ifndef IFJ_ERROR_H
#define IFJ_ERROR_H

#include <stdio.h>

/**
 * Enum representing the compiler errors and their respective exit codes
 */
enum CompilerErrors
{
	ERROR_LEX = 1,         ///< lexical errors
	ERROR_SYN = 2,         ///< syntactic errors
	ERROR_DEFINITION = 3,  ///< redefinition of identifier or undefined identifiers
	ERROR_TYPE_INIT = 4,   ///< new variable type mismatch
	ERROR_TYPE_COMPAT = 5, ///< arithmetic operation with uncompatible types
	ERROR_PARAM = 6,       ///< function return type, argument type or argument count mismatch
	ERROR_SEM = 7,         ///< other semantic errors
	ERROR_ZERO_DIV = 9,    ///< division by zero
	ERROR_MISC = 99,       ///< internal errors such as allocation error
};

#define ERROR_MSG(...)            \
	fprintf(stderr, "ERROR - ");  \
	fprintf(stderr, __VA_ARGS__); \
	fprintf(stderr, "\n")

#define PARSER_ERROR_MSG(...)                                          \
	fprintf(stderr, "ERROR (line %d) - ", parser->token->line_number); \
	fprintf(stderr, __VA_ARGS__);                                      \
	fprintf(stderr, "\n")

#define ALLOCATION_ERROR_MSG() fprintf(stderr, "ERROR - Allocation failed\n")

#endif // !IFJ_ERROR_H
