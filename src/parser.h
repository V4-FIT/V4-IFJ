/**
 * @file parser.h
 * @author Adrián Kálazi, Kevin Lackó
 * @brief Syntax analysis interface
 * @date 2020-12-08
 *
 */

#ifndef IFJ_PARSER_H
#define IFJ_PARSER_H

#include <stdio.h>

#include "symtable.h"
#include "tokens.h"

#define PARSER_ERROR_MSG(...)                                          \
	fprintf(stderr, "ERROR (line %d) - ", parser->token->line_number); \
	fprintf(stderr, __VA_ARGS__);                                      \
	fprintf(stderr, "\n")

typedef enum stmt_types
{
	STMT_DEFAULT,
	STMT_DEFINE,
	STMT_ASSIGN,
	STMT_CALL,
	STMT_ASSIGN_CALL,
	STMT_CONDITIONAL,
	STMT_ITERATIVE,
	STMT_RETURN
} stmt_types_t;

extern const char *stmt2str_map[];

typedef struct semantics
{
	symbol_ref_t func_cur;
	symbol_ref_t func_call;
	symbol_ref_t func_print;
	union
	{
		flist_iterator_t func_return_it;
		flist_iterator_t func_param_it;
	};
	symbol_ref_t var;
	stmt_types_t stmt;
	data_type_t expr_data_type;
	bool expr_constant;
	tk_param_t expr_value;
	union
	{
		tklist_iterator_t expr_begin_it;
		tklist_iterator_t argument_begin_it;
	};
	tklist_iterator_t ids_begin_it;
	union
	{
		int expr_count;
		int argument_count;
	};
	int ids_count;
} semantics_t;

typedef struct BlockCounter
{
	unsigned long for_c;
	unsigned long if_c;
	unsigned long else_id;
} * counter_t;

typedef struct parser
{
	symtable_t symtable;
	tklist_t tklist;
	tklist_iterator_t tkit;
	tklist_iterator_t tkit2;
	token_t token;
	token_t token_second;
	bool first_pass;
	semantics_t sem;
	flist_t return_id_list;
	token_type_t assign_type;
	char *last_scope;
	flist_t blockcounter;
} * parser_t;

/**
 * @brief	Initializes the parser
 * @param	tklist
 * @return	pointer to the initialized parser
*/
parser_t parser_init(tklist_t tklist);

/**
 * @brief	Sets up token access needed for parsing
 * @param	parser
 * @return	0 on success
*/
int parser_setup(parser_t parser);

/**
 * @brief	Starts the two-pass parsing of the token list
 * @param	tklist
 * @return	0 on success, error codes otherwise
*/
int parser_parse(tklist_t tklist);

/**
 * @brief	Free the allocated memory during initialization
 * @param	parser
*/
void parser_free(parser_t parser);

#endif // !IFJ_PARSER_H
