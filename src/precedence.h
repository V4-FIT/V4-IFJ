/**
 * @file precedence.h
 * @author Julie Gyselová, Adrián Kálazi, Kevin Lackó
 * @brief Expression parsing - precedence syntax analysis
 * @date 2020-12-08
 *
 */

#ifndef IFJ_PRECEDENCE_H
#define IFJ_PRECEDENCE_H

#include "parser.h"
#include "rulemacros.h"

//// Macros

#define STACK_FIRST (*head)
#define STACK_SECOND (*head)->next
#define STACK_THIRD (*head)->next->next

// setup stack and type variable
#define PARSE_EXPR_BEGIN()            \
	prec_stack_t head = stack_init(); \
	if (head == NULL) {               \
		ALLOCATION_ERROR_MSG();       \
		return ERROR_MISC;            \
	}                                 \
	prec_token_type type

// Execute a grammar rule and check for errors, also clean the stack on error
#define EXPR_EXECUTE_RULE(_RULEFUNC) \
	do {                             \
		int ret = _RULEFUNC(parser); \
		if (ret != EXIT_SUCCESS) {   \
			stack_delete(head);      \
			return ret;              \
		}                            \
	} while (0)

// finish parsing expr and clean up
#define PARSE_EXPR_END() stack_delete(head)

// current precedence
#define PREC_TABLE() prec_table[HEAD()->type][type]

// Get next token and skip TK_EOLs when possible
#define TK_PREC_NEXT()                             \
	do {                                           \
		switch (TOKEN_TYPE) {                      \
			case TK_L_PARENTHESIS:                 \
			case TK_PLUS:                          \
			case TK_MINUS:                         \
			case TK_MULTIPLY:                      \
			case TK_DIVIDE:                        \
			case TK_EQUAL:                         \
			case TK_NOT_EQUAL:                     \
			case TK_LESS:                          \
			case TK_GREATER:                       \
			case TK_LESS_EQUAL:                    \
			case TK_GREATER_EQUAL:                 \
			case TK_OR:                            \
			case TK_AND:                           \
				TK_NEXT();                         \
				EXPR_EXECUTE_RULE(rule_eol_opt_n); \
				break;                             \
			case TK_EOL:                           \
				break;                             \
			default:                               \
				TK_NEXT();                         \
				break;                             \
		}                                          \
	} while (0);

// check semantics in parse_expr
#define SEM_PREC_ACTION(_SEM_FUNC)   \
	do {                             \
		int ret = _SEM_FUNC(parser); \
		if (ret != EXIT_SUCCESS) {   \
			stack_delete(head);      \
			return ret;              \
		}                            \
	} while (0)

// check semantics in reduction rules
#define SEM_PREC_RULE_ACTION(_SEM_FUNC)    \
	do {                                   \
		int ret = _SEM_FUNC(parser, head); \
		if (ret != EXIT_SUCCESS) {         \
			return ret;                    \
		}                                  \
	} while (0)

// don't acount for E in stack head
#define HEAD() (head->type == PREC_I && head->processed && head->next->type != PREC_UNARY ? head->next : head)

// get precedence type
#define GET_PREC_TYPE()                           \
	do {                                          \
		type = convert_type(head, parser->token); \
		if (type == PREC_ERROR) {                 \
			stack_delete(head);                   \
			return ERROR_MISC;                    \
		}                                         \
	} while (0)

// save current and load next token
#define LOAD_NEXT()                                                              \
	do {                                                                         \
		int ret = stack_push(&head, parser->token, type, get_stack_sem(parser)); \
		if (ret != EXIT_SUCCESS) {                                               \
			stack_delete(head);                                                  \
			return ret;                                                          \
		}                                                                        \
		TK_PREC_NEXT();                                                          \
		GET_PREC_TYPE();                                                         \
	} while (0)

// execute reduction rule
#define REDUCE()                         \
	do {                                 \
		int ret = reduce(parser, &head); \
		if (ret != EXIT_SUCCESS) {       \
			stack_delete(head);          \
			return ret;                  \
		}                                \
	} while (0)

typedef enum
{
	PREC_L_PARENTHESIS, // (
	PREC_R_PARENTHESIS, // )
	PREC_UNARY,         // +,-,!
	PREC_MUL_DIV,       // *,/
	PREC_PLUS_MINUS,    // +,-
	PREC_RELATION,      // <, <=, >, >=
	PREC_EQUALITY,      // ==, !=
	PREC_AND,           // &&
	PREC_OR,            // ||
	PREC_I,             // id, string/int/float/bool literal
	PREC_DOLLAR,        // eol?, eof, =
	PREC_ERROR
} prec_token_type;

typedef enum
{
	OPEN,
	CLOS,
	EQUA,
	EMPT,
} prec;

// stack

typedef struct prec_stack_sem
{
	data_type_t data_type;
	bool constant;
	tk_param_t value;
} prec_stack_sem_t;

typedef struct prec_stack
{
	token_t token;
	prec_token_type type;
	bool processed;
	prec_stack_sem_t sem;
	struct prec_stack *next;
} * prec_stack_t;


/**
 * @brief Expression parser main function
 *
 * @param parser
 * @return int Exit code
 */
int parse_expr(parser_t parser);

#endif
