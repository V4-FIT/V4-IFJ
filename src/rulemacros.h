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
#define REQUIRE_SET(_TKSET, _TKSETSIZE)                      \
	do {                                                     \
		bool found = false;                                  \
		for (int i = 0; i < _TKSETSIZE; ++i) {               \
			if (scanner_token(scanner)->type == _TKSET[i]) { \
				found = true;                                \
			}                                                \
		}                                                    \
		if (found) {                                         \
			scanner_next_token(scanner);                     \
		} else {                                             \
			return ERROR_SYN;                                \
		}                                                    \
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

#define TRY_EXECUTE_RULE(_RULEFUNC, _TKNUM, ...)      \
	do {                                              \
		if (token_is(scanner, _TKNUM, __VA_ARGS__)) { \
			EXECUTE_RULE(_RULEFUNC);                  \
		}                                             \
	} while (0)

#endif // !IFJ_RULEMACROS_H