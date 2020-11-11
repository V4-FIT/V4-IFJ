#ifndef IFJ_RULEMACROS_H
#define IFJ_RULEMACROS_H

#include "error.h"
////// Macros

//// Terminals

/**
* For getting the first token at the beginning of the program
*/
#define GET_NEXT_TOKEN()             \
	do {                             \
		scanner_next_token(scanner); \
	} while (0)

/**
 * expects keyword, if not then accepts everything but immediately returns with eps return value
 * how the eps value will be handled depends on REQUIRE_NONTERMINAL and EXPECT_NONTERMINAL
 */
#define REQUIRE(_TOKEN)                               \
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
#define REQUIRE_SET(_TKNUM, ...)                           \
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