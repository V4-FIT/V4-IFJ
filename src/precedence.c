#include <stdlib.h>

#include "precedence.h"
#include "error.h"

prec_token_type convert_type(token_t t) {
    if(t == NULL) {
        return EMPTY;
    }

    switch (t->type) {
        case TK_L_PARENTHESIS:
            return PREC_L_BR;
        case TK_R_PARENTHESIS:
            return PREC_R_BR;
        case TK_PLUS:
        case TK_MINUS:
            // todo
        case TK_NOT:
            return PREC_UNARY;
        case TK_MULTIPLY:
        case TK_DIVIDE:
            return PREC_MUL_DIV;
        case TK_LESS:
        case TK_LESS_EQUAL:
        case TK_GREATER:
        case TK_GREATER_EQUAL:
            return PREC_RELATION;
        case TK_EQUAL:
        case TK_NOT_EQUAL:
            return PREC_EQUAL;
        case TK_AND:
            return PREC_AND;
        case TK_OR:
            return PREC_OR;
        case TK_IDENTIFIER:
        case TK_INT_LIT:
        case TK_FLOAT_LIT:
        case TK_STR_LIT:
        case TK_KEYW_TRUE:
        case TK_KEYW_FALSE:
            return PREC_I;
        default:
            return PREC_DOLLAR;
    }
}


prec get_prec(prec_token_type t1, token_t t2) {
    int prec_table[11][11] = {
//     (     )      +-!      */      +-      <>     ==!=     &&      ||       I      EOF
    {OPEN,  EQUAL,  OPEN,   OPEN,   OPEN,   OPEN,   OPEN,   OPEN,   OPEN,   OPEN,   EMPTY}, // (
    {EMPTY, CLOSE,  CLOSE,  CLOSE,  CLOSE,  CLOSE,  CLOSE,  CLOSE,  CLOSE,  EMPTY,  CLOSE}, // )
    {OPEN,  OPEN,   CLOSE,  CLOSE,  CLOSE,  CLOSE,  CLOSE,  CLOSE,  CLOSE,  OPEN,   CLOSE}, // +,    -,  !
    {OPEN,  CLOSE,  OPEN,   CLOSE,  CLOSE,  CLOSE,  CLOSE,  CLOSE,  CLOSE,  OPEN,   CLOSE}, // *,    /
    {OPEN,  CLOSE,  OPEN,   OPEN,   CLOSE,  CLOSE,  CLOSE,  CLOSE,  CLOSE,  OPEN,   CLOSE}, // +,    -
    {OPEN,  CLOSE,  OPEN,   OPEN,   OPEN,   CLOSE,  CLOSE,  CLOSE,  CLOSE,  OPEN,   CLOSE}, // <,    <=, >,   >=
    {OPEN,  CLOSE,  OPEN,   OPEN,   OPEN,   OPEN,   CLOSE,  CLOSE,  CLOSE,  OPEN,   CLOSE}, // ==,   !=
    {OPEN,  CLOSE,  OPEN,   OPEN,   OPEN,   OPEN,   OPEN,   CLOSE,  CLOSE,  OPEN,   CLOSE}, // &&
    {OPEN,  CLOSE,  OPEN,   OPEN,   OPEN,   OPEN,   OPEN,   OPEN,   CLOSE,  OPEN,   CLOSE}, // ||
    {EMPTY, CLOSE,  OPEN,   OPEN,   OPEN,   OPEN,   OPEN,   OPEN,   CLOSE,  EMPTY,  CLOSE}, // id, string/int/float/bool literal
    {OPEN,  EMPTY,  OPEN,   OPEN,   OPEN,   OPEN,   OPEN,   OPEN,   OPEN,   OPEN,   EMPTY}, // eol?,   eof
};
    return prec_table[t1][convert_type(t2)];
}

bool stack_empty(stack_t *head) {
    return head == NULL;
}

stack_t *push_stack(stack_t *head, token_t token) {
    static prec_token_type previous = PREC_DOLLAR;

    stack_t *new = malloc(sizeof(stack_t));
    if (new == NULL) {
        fprintf(stderr, "ERROR: malloc failed\n");
        return NULL;
    }
    if (token == NULL) {
        fprintf(stderr, "ERROR: token null\n");
    }
    
    new->token = token;
    new->prec = get_prec(previous, new->token);

    previous = convert_type(new->token);

    if (stack_empty(head)) {
        new->prec = OPEN;
        head = new;
    } else {
        new->next = head;
        head = new;
    }

    return head;
}

stack_t *pop_stack(stack_t *head) {
    if (stack_empty(head)) {
        return head;
    }
    stack_t *tmp = head->next;

    free(head);
    head = NULL;
    return tmp;
}

stack_t *reduce(stack_t *head) {
    if (convert_type(head->token) == PREC_I) {
        printf("reduced 1x\n");
        return pop_stack(head);
    } /* else if(head->prec == EQUAL) {
        printf("reduced 2x");
        head = pop_stack(head);
        return pop_stack(head);
    } */
}

int parse_expr(scanner_t scanner) {
    stack_t *head;
    head = push_stack(head, scanner_token(scanner));
    
    while (true) {
        head = push_stack(head, scanner_next_token(scanner));

        if (head->prec == EQUAL || head->prec == CLOSE) {
            head = reduce(head);
        }

        if(head->token->type == TK_EOF || head->token->type == TK_EOL) {
            break;
        }
    }

    return ERROR_MISC;
}