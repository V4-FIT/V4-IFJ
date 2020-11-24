#ifndef IFJ_RULEMACROS_H
#define IFJ_RULEMACROS_H

#include <stdbool.h>
#include "error.h"
////// Macros

//// Terminals

/**
* Get next token from the parser and check for error
*/
#define TK_NEXT()                                            \
	do {                                                     \
		token_t token = scanner_next_token(parser->scanner); \
		if (token->type == TK_ERROR) {                       \
			return token->param.i;                           \
		}                                                    \
	} while (0)

/**
 * Get next token from the parser if current token is equal to _TOKEN
 */
#define TK_MATCH(...)                                        \
	do {                                                     \
		token_type_t _TKS[] = {__VA_ARGS__};                 \
		size_t _TKNUM = sizeof(_TKS) / sizeof(token_type_t); \
		bool found = false;                                  \
		token_t token = scanner_token(parser->scanner);      \
		for (int i = 0; i < _TKNUM; ++i) {                   \
			if (token->type == _TKS[i]) {                    \
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
		token_t token = scanner_token(parser->scanner);      \
		for (int i = 0; i < _TKNUM; ++i) {                   \
			if (token->type == _TKS[i]) {                    \
				found = true;                                \
				break;                                       \
			}                                                \
		}                                                    \
		if (!found) {                                        \
			return ERROR_SYN;                                \
		}                                                    \
	} while (0)

#define TOKEN scanner_token(parser->scanner)->type

//// Non-terminals

#define EXECUTE_RULE(_RULEFUNC)               \
	do {                                      \
		int ret = _RULEFUNC(parser); \
		if (ret != EXIT_SUCCESS) {            \
			return ret;                       \
		}                                     \
	} while (0)

#endif // !IFJ_RULEMACROS_H
