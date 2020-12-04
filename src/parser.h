/**
 * @file parser.h
 * @brief Syntax analysis interface
 */

#ifndef IFJ_PARSER_H
#define IFJ_PARSER_H

#include <stdio.h>

#include "tokens.h"
#include "symtable.h"

typedef enum stmt_types
{
	STMT_DEFAULT,
	STMT_DEFINE,
	STMT_ASSIGN,
	STMT_CALL,
	STMT_CONDITIONAL,
	STMT_ITERATIVE,
	STMT_RETURN
} stmt_types_t;

typedef struct semantics
{
	symbol_ref_t func_cur;
	symbol_ref_t func_call;
	symbol_ref_t var;
	stmt_types_t stmt;
	data_type_t expression_type;
} semantics_t;

typedef struct parser
{
	symtable_t symtable;
	tklist_t tklist;
	tklist_iterator_t tkit;
	token_t token;
	token_t token_second;
	bool first_pass;
	semantics_t sem;
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
