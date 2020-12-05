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

#define PARSER_EXPR_ERROR_MSG(_EXTRA_MSG)                                                 \
	fprintf(stderr, "ERROR (line %d) - Invalid operation: ", parser->token->line_number); \
	while (parser->sem.expr_begin_it.ptr != parser->tkit.ptr) {                           \
		if (fprintf(stderr, "%s", tklist_get(parser->sem.expr_begin_it)->lexeme) > 0) {   \
			fprintf(stderr, " ");                                                         \
		}                                                                                 \
		parser->sem.expr_begin_it = tklist_it_next(parser->sem.expr_begin_it);            \
	}                                                                                     \
	_EXTRA_MSG;                                                                           \
	fprintf(stderr, "\n")

#define PARSER_COND_EXPR_ERROR_MSG()                                                    \
	fprintf(stderr, "ERROR (line %d) - non-bool ", parser->token->line_number);         \
	while (parser->sem.expr_begin_it.ptr != parser->tkit.ptr) {                         \
		if (fprintf(stderr, "%s", tklist_get(parser->sem.expr_begin_it)->lexeme) > 0) { \
			fprintf(stderr, " ");                                                       \
		}                                                                               \
		parser->sem.expr_begin_it = tklist_it_next(parser->sem.expr_begin_it);          \
	}                                                                                   \
	fprintf(stderr, "(type %s) used as %s condition\n",                                 \
			dt2str_map[parser->sem.expr_data_type],                                     \
			stmt2str_map[parser->sem.stmt])

#define MISMATCHED_TYPES_MSG                        \
	fprintf(stderr, "(mismatched types %s and %s)", \
			dt2str_map[STACK_THIRD->sem.data_type], \
			dt2str_map[STACK_FIRST->sem.data_type])

#define INVALID_TYPE_MSG                 \
	fprintf(stderr, "(invalid type %s)", \
			dt2str_map[STACK_FIRST->sem.data_type])

#define OPERATION_NOT_DEFINED_MSG                      \
	fprintf(stderr, "(operator %s not defined on %s)", \
			STACK_SECOND->token->lexeme,               \
			dt2str_map[STACK_FIRST->sem.data_type])


#define ALLOCATION_ERROR_MSG() fprintf(stderr, "ERROR - Allocation failed\n")

#endif // !IFJ_ERROR_H
