#ifndef IFJ_PRECEDENCE_H
#define IFJ_PRECEDENCE_H

#include "scanner.h"

typedef enum {
    PREC_L_BR,          // (
    PREC_R_BR,          // )
    PREC_UNARY,         // +,-,!
    PREC_MUL_DIV,       // *,/
    PREC_PLUS_MINUS,    // +,-
    PREC_RELATION,      // <, <=, >, >=
    PREC_EQUAL,         // ==, !=
    PREC_AND,           // &&
    PREC_OR,            // ||
    PREC_I,             // id, string/int/float/bool literal
    PREC_DOLLAR         // eol?, eof, =
} prec_token_type;


typedef enum {
    OPEN,
    CLOSE,
    EQUAL,
    EMPTY
} prec;

typedef struct stack {
    token_t token;
    prec prec;
    struct stack *next;
} stack_t;

stack_t *push_stack(stack_t *head, token_t token);
stack_t *pop_stack(stack_t *head);
bool stack_empty(stack_t *head);

prec get_prec(prec_token_type t1, token_t t2);
prec_token_type convert_type(token_t t1);

stack_t *reduce(stack_t *head);

int parse_expr(scanner_t scanner);

#endif // IFJ_PRECEDENCE_H