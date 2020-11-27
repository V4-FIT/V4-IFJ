#include <stdlib.h>

#include "precedence.h"
#include "error.h"

// stack operations

int prec_table[11][11] = {
		//(     )    +-!i   */    +-    <>   ==!=   &&    ||     i    EOF
		{OPEN, EQUA, OPEN, OPEN, OPEN, OPEN, OPEN, OPEN, OPEN, OPEN, EMPT}, // (
		{EMPT, CLOS, CLOS, CLOS, CLOS, CLOS, CLOS, CLOS, CLOS, EMPT, CLOS}, // )
		{OPEN, OPEN, OPEN, CLOS, CLOS, CLOS, CLOS, CLOS, CLOS, OPEN, CLOS}, // +,-,  !
		{OPEN, CLOS, OPEN, CLOS, CLOS, CLOS, CLOS, CLOS, CLOS, OPEN, CLOS}, // *,/
		{OPEN, CLOS, OPEN, OPEN, CLOS, CLOS, CLOS, CLOS, CLOS, OPEN, CLOS}, // +,-
		{OPEN, CLOS, OPEN, OPEN, OPEN, CLOS, CLOS, CLOS, CLOS, OPEN, CLOS}, // <,<=, >, >=
		{OPEN, CLOS, OPEN, OPEN, OPEN, OPEN, CLOS, CLOS, CLOS, OPEN, CLOS}, // ==,!=
		{OPEN, CLOS, OPEN, OPEN, OPEN, OPEN, OPEN, CLOS, CLOS, OPEN, CLOS}, // &&
		{OPEN, CLOS, OPEN, OPEN, OPEN, OPEN, OPEN, OPEN, CLOS, OPEN, CLOS}, // ||
		{EMPT, CLOS, EMPT, CLOS, CLOS, CLOS, CLOS, CLOS, CLOS, EMPT, CLOS}, // id, string/int/float/bool literal
		{OPEN, EMPT, OPEN, OPEN, OPEN, OPEN, OPEN, OPEN, OPEN, OPEN, EMPT}, // eol?,   eof
};

prec_token_type convert_type(stack head, token_t t) {
	if (t == NULL) {
		return EMPT;
	}

	switch (t->type) {
		case TK_L_PARENTHESIS:
			return PREC_L_BR;
		case TK_R_PARENTHESIS:
			return PREC_R_BR;
		case TK_NOT:
			return PREC_UNARY;
		case TK_PLUS:
		case TK_MINUS:
			if (head->type == PREC_I || head->type == PREC_R_BR) {
				return PREC_PLUS_MINUS;
			} else {
				return PREC_UNARY;
			}
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

void pop_stack(stack *head) {
	stack tmp = *head;
	(*head) = (*head)->next;

	free(tmp);
	tmp = NULL;
}

int push_stack(stack *head, token_t token, prec_token_type type) {
	stack new = malloc(sizeof(struct Stack));
	if (new == NULL) {
		fprintf(stderr, "ERROR: malloc failed\n");
		return ERROR_MISC;
	}

	new->token = token;
	new->type = type;
	new->next = *head;
	new->todo = true;

	*head = new;

	// printf("new: %d\n", (*head)->type);

	return EXIT_SUCCESS;
}


// grammar rules

void rule_i(stack *head) {
	// printf("E -> i\n");
	(*head)->todo = false;
}

void rule_brackets(stack *head) {
	// printf("E -> (E)\n");

	pop_stack(head);
	pop_stack(head);
	pop_stack(head);

	push_stack(head, NULL, PREC_I);
	(*head)->todo = false;
}

void rule_exit(stack *head) {
	// printf("E -> $\n");
	pop_stack(head); // remove E
}

void rule_un_neg(stack *head) {
	// printf("E -> +-!E\n");
	pop_stack(head);
	pop_stack(head);
	push_stack(head, NULL, PREC_I);
	(*head)->todo = false;
}

void rule_mul_div(stack *head) {
	// printf("E -> E*/E\n");
	pop_stack(head);
	pop_stack(head);
	(*head)->todo = false;
}

void rule_plus_minus(stack *head) {
	// printf("E -> E+-E\n");
	pop_stack(head);
	pop_stack(head);
	(*head)->todo = false;
}

void rule_rel(stack *head) {
	// printf("E -> E<>E\n");
	pop_stack(head);
	pop_stack(head);
	(*head)->todo = false;
}

void rule_equal(stack *head) {
	// printf("E -> E==E\n");
	pop_stack(head);
	pop_stack(head);
	(*head)->todo = false;
}

void rule_and(stack *head) {
	// printf("E -> E && E\n");
	pop_stack(head);
	pop_stack(head);
	(*head)->todo = false;
}

void rule_or(stack *head) {
	// printf("E -> E || E\n");
	pop_stack(head);
	pop_stack(head);
	(*head)->todo = false;
}

int reduce(stack *head) {
	// printf("reduce: ");

	switch ((*head)->type) {
		case PREC_I:
			if ((*head)->todo) {
				rule_i(head);
				return EXIT_SUCCESS;
			} else if ((*head)->next != NULL) {
				switch ((*head)->next->type) {
					case PREC_UNARY:
						if ((*head)->next->next != NULL) {
							switch ((*head)->next->next->type) {
								case PREC_L_BR:
									rule_un_neg(head);
									return EXIT_SUCCESS;
								default:
									rule_un_neg(head);
									return EXIT_SUCCESS;
									break;
							}
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
						break;
				}
			}
			break;
		case PREC_R_BR:
			if ((*head)->next != NULL && (*head)->next->type == PREC_I &&
				(*head)->next->next != NULL && (*head)->next->next->type == PREC_L_BR) {
				rule_brackets(head);
				return EXIT_SUCCESS;
			}
		default:
			break;
	}
	// printf("rule not found\n");
	return ERROR_SYN;
}

int parse_expr(token_t t, scanner_t scanner) {
	bool read = false;

	// init stack to $
	stack head = malloc(sizeof(struct Stack));
	if (head == NULL) {
		fprintf(stderr, "ERROR: malloc failed\n");
		return ERROR_MISC;
	}

	head->token = NULL;
	head->type = PREC_DOLLAR;
	head->next = NULL;

	// parse
	int res;
	prec_token_type type = convert_type(head, t);
	do {
		// printf("expr parsing (%d)\n", type);

		switch (prec_table[head->type][type]) {
			case OPEN:
				// printf("open\n");
				res = push_stack(&head, read ? scanner_token(scanner) : t, type);
				type = convert_type(head, read ? scanner_next_token(scanner) : scanner_token(scanner));
				read = true;

				break;
			case CLOS:
				res = reduce(&head);                     // expect to be able to reduce one
				while (reduce(&head) == EXIT_SUCCESS) {} // try reducing some more

				if (type == PREC_R_BR && head->type == PREC_I) { // push close brackets & reduce
					res = push_stack(&head, read ? scanner_token(scanner) : t, type);
					type = convert_type(head, read ? scanner_next_token(scanner) : scanner_token(scanner));
					res = reduce(&head);
					read = true;


					while (reduce(&head) == EXIT_SUCCESS) {} // try reducing some more
				} else if (type == PREC_DOLLAR && head->type == PREC_I && head->next->type == PREC_DOLLAR) {
					rule_exit(&head);
					while (head != NULL) {
						// printf("%d | ", head->type);
						pop_stack(&head);
					}
					return EXIT_SUCCESS;
				}

				res = push_stack(&head, read ? scanner_token(scanner) : t, type);
				type = convert_type(head, read ? scanner_next_token(scanner) : scanner_token(scanner));
				read = true;


				break;
			case EQUA:
				// printf("equal\n");
				pop_stack(&head);
				type = convert_type(head, read ? scanner_next_token(scanner) : scanner_token(scanner));
				read = true;

				break;
			case EMPT:
			default:
				if (head == NULL && type == PREC_DOLLAR) {
					// printf("ending\n");
					return EXIT_SUCCESS;
				} else if (head != NULL && head->type == PREC_DOLLAR &&
						   head->next != NULL && head->next->type == PREC_I &&
						   head->next->next != NULL && head->next->next->type == PREC_DOLLAR) {
					// printf("case 2\n");
					rule_exit(&head);
					while (head != NULL) {
						// printf("%d | ", head->type);
						pop_stack(&head);
					}
					return EXIT_SUCCESS;
				} else {
					// printf("(%d) ", type);
					while (head != NULL) {
						// printf("%d | ", head->type);
						pop_stack(&head);
					}
					// printf("\n");
					return ERROR_SYN;
				}
				break;
		}
	} while (res == EXIT_SUCCESS || type != PREC_DOLLAR);
	// next $ came in

	// printf("out of the loop\n");

	if (head->type == PREC_DOLLAR && type == PREC_DOLLAR) {
		// printf("ending\n");
		// remove head
		pop_stack(&head);
		return EXIT_SUCCESS;
	} else {
		// printf("(%d) ", type);
		while (head != NULL) {
			// printf("%d | ", head->type);
			pop_stack(&head);
		}
		// printf("\n");
		return ERROR_SYN;
	}
}
