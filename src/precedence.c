#include <stdlib.h>

#include "precedence.h"
#include "error.h"
#include "rulemacros.h"
#include "rules.h"

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
	if (tmp->token) {
		token_free(tmp->token);
	}
	free(tmp);
	tmp = NULL;
}

int push_stack(stack *head, token_t token, prec_token_type type) {
	stack new = malloc(sizeof(struct Stack));
	if (new == NULL) {
		fprintf(stderr, "ERROR: malloc failed\n");
		return ERROR_MISC;
	}

	new->token = token ? token_copy(token) : NULL;
	new->type = type;
	new->next = *head;
	new->todo = true;

	*head = new;

	return EXIT_SUCCESS;
}

void delete_stack(stack head) {
	while (head != NULL) {
		pop_stack(&head);
	}
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

int parse_expr(parser_t parser) {
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
	prec_token_type type = convert_type(head, parser->token);
	do {
		switch (prec_table[head->type][type]) {
			case OPEN:
				LOAD_NEXT;
				break;
			case CLOS:
				res = reduce(&head);                     // expect to be able to reduce one
				while (reduce(&head) == EXIT_SUCCESS) {} // try reducing some more
				CHECK_RES;

				if (type == PREC_DOLLAR && head->type == PREC_I && head->next->type == PREC_DOLLAR) {
					rule_exit(&head);
					delete_stack(head);
					return EXIT_SUCCESS;
				}

				LOAD_NEXT;

				break;
			case EQUA:
				pop_stack(&head);
				TK_PREC_NEXT();

				type = convert_type(head, parser->token);
				CHECK_TYPE;
				break;
			case EMPT:
			default:
				if (head == NULL && type == PREC_DOLLAR) {
					return EXIT_SUCCESS;
				} else if (head != NULL && head->type == PREC_DOLLAR &&
						   head->next != NULL && head->next->type == PREC_I &&
						   head->next->next != NULL && head->next->next->type == PREC_DOLLAR) {
					rule_exit(&head);
					delete_stack(head);
					return EXIT_SUCCESS;
				} else {
					delete_stack(head);
					return ERROR_SYN;
				}
				break;
		}
	} while (res == EXIT_SUCCESS);
}
