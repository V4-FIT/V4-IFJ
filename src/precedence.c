#include <stdlib.h>

#include "precedence.h"
#include "error.h"

// stack operations

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

void pop_stack(stack_t *head) {
	stack_t tmp = *head;
	(*head) = (*head)->next;

	free(tmp);
	tmp = NULL;
}

int push_stack(stack_t *head, token_t token, prec_token_type prec) {
	stack_t new = malloc(sizeof(stack_t));
	if (new == NULL) {
		fprintf(stderr, "ERROR: malloc failed\n");
		return ERROR_MISC;
	}

	new->token = token;
	new->type = prec;
	new->next = *head;

	*head = new;

	// printf("new: %d\n", (*head)->type);

	return EXIT_SUCCESS;
}


// grammar rules

void rule_i(stack_t *head) {
	printf("RULE: E -> i\n");
	(*head)->prec = DONE;
}

void rule_brackets(stack_t *head) {
	if ((*head) != NULL &&
		(*head)->next != NULL &&
		(*head)->next->next != NULL) {
		printf("RULE: E -> (E)\n");

		pop_stack(head);
		pop_stack(head);
		pop_stack(head);

		push_stack(head, NULL, PREC_I);
		(*head)->prec = DONE;
	}
}

void rule_exit(stack_t *head) {
	printf("E -> $\n");
	pop_stack(head);
}

int reduce(stack_t *head) {
	printf("reduce: ");

	switch ((*head)->type) {
		case PREC_I:
			if ((*head)->prec != DONE) {
				rule_i(head);
				return EXIT_SUCCESS;
			} else if ((*head)->next != NULL && (*head)->next->type == PREC_DOLLAR) {
				rule_exit(head);
			}
			break;
		case PREC_R_BR:
			if ((*head)->next != NULL && (*head)->next->type == PREC_I &&
				(*head)->next->next != NULL && (*head)->next->next->type == PREC_L_BR) {
				rule_brackets(head);
				return EXIT_SUCCESS;
			} else {
				printf("rule not found\n");
			}

		default:
			break;
	}
	return ERROR_SYN;
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
		// printf("table: %d, %d\n", head->type, type);
		switch (prec_table[head->type][type]) {
			case OPEN:
				// printf("open\n");
				res = push_stack(&head, scanner_token(scanner), type);
				type = convert_type(scanner_next_token(scanner));
				head->prec = OPEN;

				break;
			case CLOSE:
				// printf("close\n");
				// res = push_stack(&head, scanner_token(scanner), type);
				if (res != EXIT_SUCCESS) {
					break;
				}
				res = reduce(&head);
				if (type == PREC_R_BR && head->type == PREC_I) {
					printf("reduce more\n");
					res = push_stack(&head, scanner_token(scanner), type);
					type = convert_type(scanner_next_token(scanner));

					res = reduce(&head);
				}
				break;
			case EQUAL:
				printf("equal\n");
				// res = push_stack(&head, scanner_token(scanner));
				pop_stack(&head);
				type = convert_type(scanner_next_token(scanner));

				break;
			case EMPTY:
				printf("empty\n");
			default:
				if (head->type == PREC_DOLLAR && type == PREC_DOLLAR) {
					// remove head
					pop_stack(&head);
					return EXIT_SUCCESS;
				} else {
					printf("(%d) ", type);
					while (head != NULL) {
						printf("%d | ", head->type);
						pop_stack(&head);
					}
					return ERROR_SYN;
				}
				break;
		}
	} while (res == EXIT_SUCCESS);
	// next $ came in

	if (head->type == PREC_DOLLAR && type == PREC_DOLLAR) {
		// remove head
		pop_stack(&head);
		return EXIT_SUCCESS;
	} else {
		printf("(%d) ", type);
		while (head != NULL) {
			printf("%d | ", head->type);
			pop_stack(&head);
		}
		return ERROR_SYN;
	}
}
