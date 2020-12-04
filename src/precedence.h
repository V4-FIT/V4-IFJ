#ifndef IFJ_PRECEDENCE_H
#define IFJ_PRECEDENCE_H

#include "parser.h"
#include "rulemacros.h"

//// Macros

#define STACK_FIRST (*head)
#define STACK_SECOND (*head)->next
#define STACK_THIRD (*head)->next->next

// Get next token and skip TK_EOLs when possible
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

// don't acount for E in stack head
#define HEAD() (head->type == PREC_I && head->todo == false && head->next->type != PREC_UNARY ? head->next : head)

#define CHECK_RES()                \
	do {                           \
		if (res != EXIT_SUCCESS) { \
			stack_delete(head);    \
			return res;            \
		}                          \
	} while (0)

#define CHECK_TYPE()              \
	do {                          \
		if (type == PREC_ERROR) { \
			stack_delete(head);   \
			return ERROR_MISC;    \
		}                         \
	} while (0)

#define LOAD_NEXT()                                                      \
	res = stack_push(&head, parser->token, type, get_data_type(parser)); \
	TK_PREC_NEXT();                                                      \
	type = convert_type(head, parser->token);                            \
	CHECK_TYPE();                                                        \
	CHECK_RES()

#define REDUCE()                 \
	res = reduce(parser, &head); \
	CHECK_RES()

typedef enum
{
	PREC_L_PARENTHESIS, // (
	PREC_R_PARENTHESIS, // )
	PREC_UNARY,         // +,-,!
	PREC_MUL_DIV,       // *,/
	PREC_PLUS_MINUS,    // +,-
	PREC_RELATION,      // <, <=, >, >=
	PREC_EQUAL,         // ==, !=
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
	DONE
} prec;

typedef enum
{
	R_EMPTY,
	R_UNARY,
	R_BINARY,
	R_TERTIARY
} rule_size;

// stack

typedef struct prec_stack
{
	token_t token;
	prec_token_type type;
	bool todo;
	data_type_t data_type;
	struct prec_stack *next;
} * prec_stack_t;

int parse_expr(parser_t parser);

#endif
