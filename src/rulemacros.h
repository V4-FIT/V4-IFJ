#ifndef IFJ_RULEMACROS_H
#define IFJ_RULEMACROS_H

#include <stdbool.h>
#include "error.h"

////// Macros

#define TOKEN_TYPE parser->token->type
#define TOKEN_SECOND_TYPE parser->token_second->type

// Ends the first pass by skipping all the tokens until the next function
#define FIRST_PASS_END()                                                 \
	do {                                                                 \
		if (parser->first_pass) {                                        \
			while (TOKEN_TYPE != TK_KEYW_FUNC && TOKEN_TYPE != TK_EOF) { \
				TK_NEXT();                                               \
			}                                                            \
			SEM_ACTION(sem_exit_scope);                                  \
			return EXIT_SUCCESS;                                         \
		}                                                                \
	} while (0)

//// Terminals

// Get the next token and token_second
#define TK_NEXT()                                             \
	do {                                                      \
		parser->token = parser->token_second;                 \
		parser->tkit = parser->tkit2;                         \
		tklist_iterator_t it = tklist_it_next(parser->tkit2); \
		if (tklist_it_valid(it)) {                            \
			parser->tkit2 = it;                               \
			parser->token_second = tklist_get(it);            \
		}                                                     \
	} while (0)


// Check syntax against current token
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
			SYNTAX_ERROR_MESSAGE();                          \
			return ERROR_SYN;                                \
		}                                                    \
	} while (0)

// Check syntax against current token and then get the next one
#define TK_MATCH(...)         \
	do {                      \
		TK_TEST(__VA_ARGS__); \
		TK_NEXT();            \
	} while (0)

#define SYNTAX_ERROR_MESSAGE()                                                       \
	fprintf(stderr, "ERROR (line %d) - syntax error: unexpected %s, expecting %s",   \
			parser->token->line_number, parser->token->lexeme, tk2str_map[_TKS[0]]); \
	for (int i = 1; i < _TKNUM; ++i) {                                               \
		fprintf(stderr, " or %s", tk2str_map[_TKS[i]]);                              \
	}                                                                                \
	fprintf(stderr, "\n")


//// Non-terminals

// Execute a grammar rule and check for errors
#define EXECUTE_RULE(_RULEFUNC)      \
	do {                             \
		int ret = _RULEFUNC(parser); \
		if (ret != EXIT_SUCCESS) {   \
			return ret;              \
		}                            \
	} while (0)

//// Semantics

// Execute a semantic action and check for semantic errors
#define SEM_ACTION(_SEM_FUNC)        \
	do {                             \
		int ret = _SEM_FUNC(parser); \
		if (ret != EXIT_SUCCESS) {   \
			return ret;              \
		}                            \
	} while (0)

////// Code generation

/**
 * Generate only on the second pass
 * @note used everywhere before FIRST_PASS_END
 */
#define GENERATE_ONCE(_asm)        \
	do {                           \
		if (!parser->first_pass) { \
			_asm;                  \
		}                          \
	} while (0)

#endif // !IFJ_RULEMACROS_H
