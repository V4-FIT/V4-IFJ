#include <stdlib.h>

#include "precedence.h"
#include "error.h"

// stack operations

int prec_table[11][11] = {
		//(     )      +-!      */      +-      <>     ==!=     &&      ||       I      EOF
		{OPEN, EQUAL, OPEN, OPEN, OPEN, OPEN, OPEN, OPEN, OPEN, OPEN, EMPTY},          // (
		{EMPTY, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, EMPTY, CLOSE}, // )
		{OPEN, OPEN, OPEN, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, OPEN, CLOSE},     // +,    -,  !
		{OPEN, CLOSE, OPEN, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, OPEN, CLOSE},    // *,    /
		{OPEN, CLOSE, OPEN, OPEN, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, OPEN, CLOSE},     // +,    -
		{OPEN, CLOSE, OPEN, OPEN, OPEN, CLOSE, CLOSE, CLOSE, CLOSE, OPEN, CLOSE},      // <,    <=, >,   >=
		{OPEN, CLOSE, OPEN, OPEN, OPEN, OPEN, CLOSE, CLOSE, CLOSE, OPEN, CLOSE},       // ==,   !=
		{OPEN, CLOSE, OPEN, OPEN, OPEN, OPEN, OPEN, CLOSE, CLOSE, OPEN, CLOSE},        // &&
		{OPEN, CLOSE, OPEN, OPEN, OPEN, OPEN, OPEN, OPEN, CLOSE, OPEN, CLOSE},         // ||
		{EMPTY, CLOSE, OPEN, OPEN, OPEN, OPEN, OPEN, OPEN, OPEN, EMPTY, CLOSE},        // id, string/int/float/bool literal
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
	printf("RULE: E -> %d\n", (*head)->token->type);
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

void rule_un_neg(stack_t *head) {
	printf("E -> +-!E\n");
	pop_stack(head);
	pop_stack(head);
	push_stack(head, NULL, PREC_I);
	(*head)->prec = DONE;
}

void rule_mul_div(stack_t *head) {
	printf("E -> E*/E\n");
	pop_stack(head);
	pop_stack(head);
	(*head)->prec = DONE;
}

void rule_plus_minus(stack_t *head) {
	printf("E -> E+-E\n");
	pop_stack(head);
	pop_stack(head);
	(*head)->prec = DONE;
}

void rule_rel(stack_t *head) {
	printf("E -> E<>E\n");
	pop_stack(head);
	pop_stack(head);
	(*head)->prec = DONE;
}

void rule_equal(stack_t *head) {
	printf("E -> E==E\n");
	pop_stack(head);
	pop_stack(head);
	(*head)->prec = DONE;
}

void rule_and(stack_t *head) {
	printf("E -> E && E\n");
	pop_stack(head);
	pop_stack(head);
	(*head)->prec = DONE;
}

void rule_or(stack_t *head) {
	printf("E -> E || E\n");
	pop_stack(head);
	pop_stack(head);
	(*head)->prec = DONE;
}

int reduce(stack_t *head) {
	printf("reduce: ");

	switch ((*head)->type) {
		case PREC_I:
			if ((*head)->prec != DONE) {
				rule_i(head);
				return EXIT_SUCCESS;
			} else if ((*head)->next != NULL && (*head)->next->type == PREC_DOLLAR) {
				// rule_exit(head);
				// no exit_success
			} else if ((*head)->next != NULL) {
				switch ((*head)->next->type) {
					case PREC_UNARY:
						if ((*head)->next->next != NULL) {
							switch ((*head)->next->next->type) {
								case PREC_I:
									(*head)->next->type = PREC_PLUS_MINUS;
									return reduce(head);
								case PREC_L_BR:
									rule_un_neg(head);
									return EXIT_SUCCESS;
								default:
									rule_un_neg(head);
									return EXIT_SUCCESS;
									break;
							}
						} else {
							printf("we may have a problem\n");
						}

						break;
					case PREC_MUL_DIV:
						if ((*head)->next->next != NULL && (*head)->next->next->type == PREC_I) {
							rule_mul_div(head);
							return EXIT_SUCCESS;
						}

					case PREC_PLUS_MINUS:
						if ((*head)->next->next != NULL && (*head)->next->next->type == PREC_I) {
							rule_plus_minus(head);
							return EXIT_SUCCESS;
						}
					case PREC_RELATION:
						if ((*head)->next->next != NULL && (*head)->next->next->type == PREC_I) {
							rule_rel(head);
							return EXIT_SUCCESS;
						}
					case PREC_EQUAL:
						if ((*head)->next->next != NULL && (*head)->next->next->type == PREC_I) {
							rule_equal(head);
							return EXIT_SUCCESS;
						}
					case PREC_AND:
						if ((*head)->next->next != NULL && (*head)->next->next->type == PREC_I) {
							rule_and(head);
							return EXIT_SUCCESS;
						}

					case PREC_OR:
						if ((*head)->next->next != NULL && (*head)->next->next->type == PREC_I) {
							rule_or(head);
							return EXIT_SUCCESS;
						}
					default:
						printf("problem");
						break;
				}
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
		printf("table: %d %d (%d)\n", head->type, type, prec_table[head->type][type]);
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
				res = reduce(&head);                     // expect to be able to reduce one
				while (reduce(&head) == EXIT_SUCCESS) {} // try reducing some more
				printf("\nresult: %d\n", res);

				if (type == PREC_R_BR && head->type == PREC_I) { // push close brackets & reduce
					printf("reduce more\n");
					res = push_stack(&head, scanner_token(scanner), type);
					type = convert_type(scanner_next_token(scanner));

					res = reduce(&head);
				} else if (type == PREC_DOLLAR && head->type == PREC_I) {
					rule_exit(&head);
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
				// printf("something went wrong\n");
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
					printf("\n");
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
		printf("\n");
		return ERROR_SYN;
	}
}
