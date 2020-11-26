#include <stdlib.h>

#include "precedence.h"
#include "error.h"

int prec_table[11][11] = {
		//     (     )      +-!      */      +-      <>     ==!=     &&      ||       I      EOF
		{OPEN, EQUAL, OPEN, OPEN, OPEN, OPEN, OPEN, OPEN, OPEN, OPEN, EMPTY},          // (
		{EMPTY, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, EMPTY, CLOSE}, // )
		{OPEN, OPEN, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, OPEN, CLOSE},    // +,    -,  !
		{OPEN, CLOSE, OPEN, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, OPEN, CLOSE},    // *,    /
		{OPEN, CLOSE, OPEN, OPEN, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, OPEN, CLOSE},     // +,    -
		{OPEN, CLOSE, OPEN, OPEN, OPEN, CLOSE, CLOSE, CLOSE, CLOSE, OPEN, CLOSE},      // <,    <=, >,   >=
		{OPEN, CLOSE, OPEN, OPEN, OPEN, OPEN, CLOSE, CLOSE, CLOSE, OPEN, CLOSE},       // ==,   !=
		{OPEN, CLOSE, OPEN, OPEN, OPEN, OPEN, OPEN, CLOSE, CLOSE, OPEN, CLOSE},        // &&
		{OPEN, CLOSE, OPEN, OPEN, OPEN, OPEN, OPEN, OPEN, CLOSE, OPEN, CLOSE},         // ||
		{EMPTY, CLOSE, OPEN, OPEN, OPEN, OPEN, OPEN, OPEN, CLOSE, EMPTY, CLOSE},       // id, string/int/float/bool literal
		{OPEN, EMPTY, OPEN, OPEN, OPEN, OPEN, OPEN, OPEN, OPEN, OPEN, EMPTY},          // eol?,   eof
};

prec_token_type convert_type(token_t t) {
	if (t == NULL) {
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

bool stack_empty(stack_t *head) {
	return head == NULL;
}

void pop_stack(stack_t *head) {
	stack_t tmp = *head;
	(*head) = (*head)->next;

	free(tmp);
	tmp = NULL;
}

int push_stack(stack_t *head, token_t token) {
	stack_t new = malloc(sizeof(stack_t));
	if (new == NULL) {
		fprintf(stderr, "ERROR: malloc failed\n");
		return ERROR_MISC;
	}

	new->token = token;
	new->type = convert_type(token);
	new->next = *head;

	*head = new;

	return EXIT_SUCCESS;
}


int parse_expr(scanner_t scanner) {

	// init stack to $
	stack_t head = malloc(sizeof(struct stack));
	if (head == NULL) {
		fprintf(stderr, "ERROR: malloc failed\n");
		return ERROR_MISC;
	}

	head->token = NULL;
	head->type = PREC_DOLLAR;
	head->next = NULL;

	// parse
	int res;
	prec_token_type type = convert_type(scanner_token(scanner));
	do {
		printf("table: %d, %d\n", head->type, type);
		switch (prec_table[head->type][type]) {
			case OPEN:
				printf("open\n");
				head->prec = OPEN;
				res = push_stack(&head, scanner_token(scanner));
				break;
			case CLOSE:
				printf("close\n");
				break;
			case EQUAL:
				printf("equal\n");
				// res = push_stack(&head, scanner_token(scanner));
				pop_stack(&head);
				break;
			case EMPTY:
				printf("empty\n");
			default:
				printf("ERROR!\n");
				while (head != NULL) {
					pop_stack(&head);
				}
				return ERROR_SYN;
				break;
		}
		type = convert_type(scanner_next_token(scanner));
	} while (type != PREC_DOLLAR && res == EXIT_SUCCESS);
	// next $ came in


	// remove head
	pop_stack(&head);
	return EXIT_SUCCESS;
}
