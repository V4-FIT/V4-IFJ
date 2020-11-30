#ifndef IFJ_PRECEDENCE_H
#define IFJ_PRECEDENCE_H

#include "parser.h"


#define CHECK_RES                  \
	do {                           \
		if (res != EXIT_SUCCESS) { \
			delete_stack(head);    \
			return res;            \
		}                          \
	} while (0)

#define CHECK_TYPE                \
	do {                          \
		if (type == PREC_ERROR) { \
			delete_stack(head);   \
			return ERROR_SYN;     \
		}                         \
	} while (0)

#define LOAD_NEXT                                 \
	res = push_stack(&head, parser->token, type); \
	TK_PREC_NEXT();                               \
	type = convert_type(head, parser->token);     \
	CHECK_TYPE;                                   \
	CHECK_RES


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
} * stack;


prec_token_type convert_type(stack head, token_t t1);

int push_stack(stack *head, token_t token, prec_token_type prec);
void pop_stack(stack *head);
void delete_stack(stack head);

void rule_i(stack *head);
void rule_brackets(stack *head);
void rule_exit(stack *head);
void rule_un_neg(stack *head);
void rule_mul_div(stack *head);
void rule_rel(stack *head);
void rule_equal(stack *head);
void rule_and(stack *head);
void rule_or(stack *head);


int reduce(stack *head);

int parse_expr(parser_t parser);

#endif
