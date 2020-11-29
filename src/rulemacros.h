#ifndef IFJ_RULEMACROS_H
#define IFJ_RULEMACROS_H

#include <stdbool.h>
#include "error.h"
////// Macros

#define TOKEN_TYPE parser->token->type
#define TOKEN_SECOND_TYPE tklist_second(parser->tklist)->type

//// Terminals

/**
* Get next token from the parser and check for error
*/
#define TK_NEXT()                                     \
	do {                                              \
		tklist_pop_front(parser->tklist);             \
		parser->token = tklist_front(parser->tklist); \
	} while (0)

/**
 * Get next token from the parser if current token is equal to _TOKEN
 */
#define TK_MATCH(...)                                        \
	do {                                                     \
		token_type_t _TKS[] = {__VA_ARGS__};                 \
		size_t _TKNUM = sizeof(_TKS) / sizeof(token_type_t); \
		bool found = false;                                  \
		for (int i = 0; i < _TKNUM; ++i) {                   \
			if (parser->token->type == _TKS[i]) {            \
				found = true;                                \
				break;                                       \
			}                                                \
		}                                                    \
		if (!found) {                                        \
			return ERROR_SYN;                                \
		} else {                                             \
			TK_NEXT();                                       \
		}                                                    \
	} while (0)
/**
* Check syntax against current token
*/
#define TK_TEST(...)                                         \
	do {                                                     \
		token_type_t _TKS[] = {__VA_ARGS__};                 \
		size_t _TKNUM = sizeof(_TKS) / sizeof(token_type_t); \
		bool found = false;                                  \
		for (int i = 0; i < _TKNUM; ++i) {                   \
			if (parser->token->type == _TKS[i]) {            \
				found = true;                                \
				break;                                       \
			}                                                \
		}                                                    \
		if (!found) {                                        \
			return ERROR_SYN;                                \
		}                                                    \
	} while (0)

#define TK_PREC_NEXT()                        \
	do {                                      \
		switch (TOKEN_TYPE) {                 \
			case TK_L_PARENTHESIS:            \
			case TK_PLUS:                     \
			case TK_MINUS:                    \
			case TK_MULTIPLY:                 \
			case TK_DIVIDE:                   \
			case TK_EQUAL:                    \
			case TK_NOT_EQUAL:                \
			case TK_LESS:                     \
			case TK_GREATER:                  \
			case TK_LESS_EQUAL:               \
			case TK_GREATER_EQUAL:            \
			case TK_OR:                       \
			case TK_AND:                      \
				TK_NEXT();                    \
				EXECUTE_RULE(rule_eol_opt_n); \
				break;                        \
			case TK_EOL:                      \
				break;                        \
			default:                          \
				TK_NEXT();                    \
				break;                        \
		}                                     \
	} while (0);

//// Non-terminals

#define EXECUTE_RULE(_RULEFUNC)      \
	do {                             \
		int ret = _RULEFUNC(parser); \
		if (ret != EXIT_SUCCESS) {   \
			return ret;              \
		}                            \
	} while (0)

//// Semantics

#define SEM_DEFINE_FUNC()                                                       \
	do {                                                                        \
		symbol_ref_t symbol = symtable_find(parser->symtable, parser->token);   \
		if (symbol_valid(symbol)) {                                             \
			if (symbol.symbol->func.defined) {                                  \
				return ERROR_DEFINITION; /*redefinition*/                       \
			} else {                                                            \
				symbol.symbol->func.defined = true;                             \
			}                                                                   \
		} else {                                                                \
			symbol = symtable_insert(parser->symtable, parser->token, ST_FUNC); \
			if (!symbol_valid(symbol)) {                                        \
				return ERROR_MISC; /*allocation error*/                         \
			}                                                                   \
			symbol.symbol->func.defined = true;                                 \
		}                                                                       \
	} while (0)

#endif // !IFJ_RULEMACROS_H
