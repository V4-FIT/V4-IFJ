#ifndef IFJ_RULEMACROS_H
#define IFJ_RULEMACROS_H

#include "error.h"
////// Macros

//// Terminals

/**
* Get next token from the parser
*/
#define TK_NEXT()                    \
	do {                             \
		scanner_next_token(scanner); \
	} while (0)

/**
 * Get next token from the parser if current token is equal to _TOKEN
 */
#define TK_NEXT_IF(_TOKEN)                            \
	do {                                              \
		if (scanner_token(scanner)->type == _TOKEN) { \
			scanner_next_token(scanner);              \
		} else {                                      \
			return ERROR_SYN;                         \
		}                                             \
	} while (0)

/**
 * same as above but over a set
 */
#define TK_NEXT_IF_SET(_TKNUM, ...)                        \
	do {                                                   \
		token_type_t _TKS[_TKNUM] = {__VA_ARGS__};         \
		bool found = false;                                \
		for (int i = 0; i < _TKNUM; ++i) {                 \
			if (scanner_token(scanner)->type == _TKS[i]) { \
				found = true;                              \
			}                                              \
		}                                                  \
		if (found) {                                       \
			scanner_next_token(scanner);                   \
		} else {                                           \
			return ERROR_SYN;                              \
		}                                                  \
	} while (0)

/**
* Check syntax against current token
*/
#define TK_TEST(_TOKEN)                               \
	do {                                              \
		if (scanner_token(scanner)->type != _TOKEN) { \
			return ERROR_SYN;                         \
		}                                             \
	} while (0)

/**
 * same as above but over a set
 */
#define TK_TEST_SET(_TKNUM, ...)                           \
	do {                                                   \
		token_type_t _TKS[_TKNUM] = {__VA_ARGS__};         \
		bool found = false;                                \
		for (int i = 0; i < _TKNUM; ++i) {                 \
			if (scanner_token(scanner)->type == _TKS[i]) { \
				found = true;                              \
			}                                              \
		}                                                  \
		if (!found) {                                      \
			return ERROR_SYN;                              \
		}                                                  \
	} while (0)



//// Non-terminals

/**
 * if a child returns eps -> syntactic error
 */
#define EXECUTE_RULE(_RULEFUNC)                \
	do {                                       \
		if (_RULEFUNC(scanner) == ERROR_SYN) { \
			return ERROR_SYN;                  \
		}                                      \
	} while (0)

#define TRY_EXECUTE_RULE(_RULEFUNC, _TKNUM, ...)                 \
	do {                                                         \
		token_type_t token_type[_TKNUM] = {__VA_ARGS__};         \
		for (size_t i = 0; i < _TKNUM; i++) {                    \
			if (scanner_token(scanner)->type == token_type[i]) { \
				EXECUTE_RULE(_RULEFUNC);                         \
				break;                                           \
			}                                                    \
		}                                                        \
	} while (0)

#endif // !IFJ_RULEMACROS_H