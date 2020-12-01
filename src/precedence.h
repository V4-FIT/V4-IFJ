#ifndef IFJ_PRECEDENCE_H
#define IFJ_PRECEDENCE_H

#include "parser.h"


#define CHECK_RES()                \
	do {                           \
		if (res != EXIT_SUCCESS) { \
			delete_stack(head);    \
			return res;            \
		}                          \
	} while (0)

#define CHECK_TYPE()              \
	do {                          \
		if (type == PREC_ERROR) { \
			delete_stack(head);   \
			return ERROR_MISC;    \
		}                         \
	} while (0)

#define LOAD_NEXT()                               \
	res = push_stack(&head, parser->token, type); \
	TK_PREC_NEXT();                               \
	type = convert_type(head, parser->token);     \
	CHECK_TYPE();                                 \
	CHECK_RES()


typedef enum
{
	PREC_L_BR,       // (
	PREC_R_BR,       // )
	PREC_UNARY,      // +,-,!
	PREC_MUL_DIV,    // *,/
	PREC_PLUS_MINUS, // +,-
	PREC_RELATION,   // <, <=, >, >=
	PREC_EQUAL,      // ==, !=
	PREC_AND,        // &&
	PREC_OR,         // ||
	PREC_I,          // id, string/int/float/bool literal
	PREC_DOLLAR,     // eol?, eof, =
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

// rule definitions


typedef enum
{
	R_EMPTY,
	R_UNARY,
	R_BINARY,
	R_TERTIARY
} rule_size;

// stack

typedef struct Stack
{
	token_t token;
	prec_token_type type;
	bool todo;
	struct Stack *next;
} * prec_stack_t;


prec_token_type convert_type(prec_stack_t head, token_t t1);

int push_stack(prec_stack_t *head, token_t token, prec_token_type prec);
void pop_stack(prec_stack_t *head);
void delete_stack(prec_stack_t head);

void rule_i(prec_stack_t *head);
void rule_brackets(prec_stack_t *head);
void rule_exit(prec_stack_t *head);
void rule_un_neg(prec_stack_t *head);
void rule_mul_div(prec_stack_t *head);
void rule_rel(prec_stack_t *head);
void rule_equal(prec_stack_t *head);
void rule_and(prec_stack_t *head);
void rule_or(prec_stack_t *head);


int reduce(prec_stack_t *head);

int parse_expr(parser_t parser);

#endif
