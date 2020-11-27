#ifndef IFJ_PRECEDENCE_H
#define IFJ_PRECEDENCE_H

#include "scanner.h"

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
	CLOSE,
	EQUAL,
	EMPTY,
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

typedef struct stack
{
	token_t token;
	prec_token_type type;
	prec prec;
	struct stack *next;
} * stack_t;


prec_token_type convert_type(token_t t1);

int push_stack(stack_t *head, token_t token, prec_token_type prec);
void pop_stack(stack_t *head);

void rule_i(stack_t *head);
void rule_brackets(stack_t *head);
void rule_exit(stack_t *head);
void rule_un_neg(stack_t *head);
void rule_mul_div(stack_t *head);
void rule_rel(stack_t *head);
void rule_equal(stack_t *head);
void rule_and(stack_t *head);
void rule_or(stack_t *head);


int reduce(stack_t *head);

int parse_expr(scanner_t scanner);

#endif
