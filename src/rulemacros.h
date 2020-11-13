#ifndef IFJ_RULEMACROS_H
#define IFJ_RULEMACROS_H

#include <stdbool.h>
#include "error.h"
////// Macros

//// Terminals

/**
* Get next token from the parser
*/
#define TK_NEXT()                    \
    do {                             \
        if (scanner_next_token(scanner)->type == TK_ERROR) { \
            return scanner_token(scanner)->param.i; \
        } \
    } while (0)

/**
 * Get next token from the parser if current token is equal to _TOKEN
 */
#define TK_NEXT_IF(_TOKEN)                            \
    do {                                              \
        if (scanner_token(scanner)->type == _TOKEN) { \
            if (scanner_next_token(scanner)->type == TK_ERROR) { \
                return scanner_token(scanner)->param.i; \
            } \
        } else {                                      \
            return ERROR_SYN;                         \
        }                                             \
    } while (0)

/**
 * same as above but over a set
 */
#define TK_NEXT_IF_SET(...)                                  \
    do {                                                     \
        token_type_t _TKS[] = {__VA_ARGS__};                 \
        size_t _TKNUM = sizeof(_TKS) / sizeof(token_type_t); \
        bool found = false;                                  \
        for (int i = 0; i < _TKNUM; ++i) {                   \
            if (scanner_token(scanner)->type == _TKS[i]) {   \
                found = true;                                \
            }                                                \
        }                                                    \
        if (found) {                                         \
            if (scanner_next_token(scanner)->type == TK_ERROR) { \
                return scanner_token(scanner)->param.i; \
            } \
        } else {                                             \
            return ERROR_SYN;                                \
        }                                                    \
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
#define TK_TEST_SET(...)                                     \
	do {                                                     \
		token_type_t _TKS[] = {__VA_ARGS__};                 \
		size_t _TKNUM = sizeof(_TKS) / sizeof(token_type_t); \
		bool found = false;                                  \
		for (int i = 0; i < _TKNUM; ++i) {                   \
			if (scanner_token(scanner)->type == _TKS[i]) {   \
				found = true;                                \
			}                                                \
		}                                                    \
		if (!found) {                                        \
			return ERROR_SYN;                                \
		}                                                    \
	} while (0)

#define TRY_TK_NEXT_IF(_TOKEN)                        \
	do {                                              \
		TRY_SUCCESS = false;                          \
		if (scanner_token(scanner)->type == _TOKEN) { \
			scanner_next_token(scanner);              \
			TRY_SUCCESS = true;                       \
		}                                             \
	} while (0)

#define TRY_TK(_TOKEN)                                \
	do {                                              \
		TRY_SUCCESS = false;                          \
		if (scanner_token(scanner)->type == _TOKEN) { \
			TRY_SUCCESS = true;                       \
		}                                             \
	} while (0)

#define TOKEN scanner_token(scanner)->type

//// Non-terminals

#define RULE_INIT()           \
	bool TRY_SUCCESS = false; \
	do {                      \
	} while (0)

#define EXECUTE_RULE(_RULEFUNC)                \
    do {                                       \
        int ret = _RULEFUNC(scanner); \
        if (ret != EXIT_SUCCESS) { \
            debug = ret;       \
            return ret;                  \
        }                                      \
    } while (0)

#define TRY_EXECUTE_RULE(_RULEFUNC, ...)                     \
	do {                                                     \
		TRY_SUCCESS = false;                                 \
		token_type_t _TKS[] = {__VA_ARGS__};                 \
		size_t _TKNUM = sizeof(_TKS) / sizeof(token_type_t); \
		for (size_t i = 0; i < _TKNUM; i++) {                \
			if (scanner_token(scanner)->type == _TKS[i]) {   \
				EXECUTE_RULE(_RULEFUNC);                     \
				TRY_SUCCESS = true;                          \
				break;                                       \
			}                                                \
		}                                                    \
	} while (0)

#endif // !IFJ_RULEMACROS_H
