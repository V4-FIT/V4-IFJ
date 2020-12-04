#include "precedence.h"

#include <stdlib.h>
#include <assert.h>

#include "error.h"
#include "rules.h"

////// Forward declarations
static prec_token_type convert_type(prec_stack_t head, token_t t1);
static int reduce(parser_t parser, prec_stack_t *head);
static data_type_t get_data_type(parser_t parser);
// stack operations
static prec_stack_t stack_init();
static int stack_push(prec_stack_t *head, token_t token, prec_token_type prec, data_type_t data_type);
static void stack_pop(prec_stack_t *head);
static void stack_delete(prec_stack_t head);
// reduction type checks
static bool stack_term(prec_stack_t *head);
static bool stack_un_term(prec_stack_t *head);
static bool stack_lparenthesis_term_rparenthesis(prec_stack_t *head);
static bool stack_term_op_term(prec_stack_t *head);
// rule definitions
static int rule_i(parser_t parser, prec_stack_t *head);
static int rule_brackets(parser_t parser, prec_stack_t *head);
static int rule_exit(parser_t parser, prec_stack_t *head);
static int rule_un(parser_t parser, prec_stack_t *head);
static int rule_mul_div(parser_t parser, prec_stack_t *head);
static int rule_rel(parser_t parser, prec_stack_t *head);
static int rule_equal(parser_t parser, prec_stack_t *head);
static int rule_and(parser_t parser, prec_stack_t *head);
static int rule_or(parser_t parser, prec_stack_t *head);

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

prec_token_type convert_type(prec_stack_t head, token_t t) {
	if (t == NULL) {
		return PREC_ERROR;
	}

	switch (t->type) {
		case TK_L_PARENTHESIS:
			return PREC_L_PARENTHESIS;
		case TK_R_PARENTHESIS:
			return PREC_R_PARENTHESIS;
		case TK_NOT:
			return PREC_UNARY;
		case TK_PLUS:
		case TK_MINUS:
			if (head->type == PREC_I || head->type == PREC_R_PARENTHESIS) {
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

data_type_t get_data_type(parser_t parser) {
	symbol_ref_t symbol_ref;
	switch (TOKEN_TYPE) {
		case TK_IDENTIFIER:
			symbol_ref = symtable_find(parser->symtable, parser->token);
			assert(symbol_valid(symbol_ref));
			return symbol_ref.symbol->var.data_type;
			break;
		case TK_INT_LIT:
			return DT_INTEGER;
		case TK_FLOAT_LIT:
			return DT_FLOAT64;
		case TK_STR_LIT:
			return DT_STRING;
		case TK_KEYW_TRUE:
		case TK_KEYW_FALSE:
			return DT_BOOL;
		default:
			return DT_UNDEFINED;
	}
}

void stack_pop(prec_stack_t *head) {
	prec_stack_t tmp = *head;
	(*head) = (*head)->next;
	free(tmp);
}

prec_stack_t stack_init() {
	// init stack to $
	prec_stack_t head = malloc(sizeof(struct prec_stack));
	if (head == NULL) {
		return NULL;
	}
	head->token = NULL;
	head->type = PREC_DOLLAR;
	head->next = NULL;
	return head;
}

int stack_push(prec_stack_t *head, token_t token, prec_token_type type, data_type_t data_type) {
	prec_stack_t new = malloc(sizeof(struct prec_stack));
	if (new == NULL) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}

	new->token = token;
	new->type = type;
	new->data_type = data_type;
	new->next = *head;
	new->todo = true;

	*head = new;

	return EXIT_SUCCESS;
}

void stack_delete(prec_stack_t head) {
	while (head != NULL) {
		stack_pop(&head);
	}
}

// reduction type checks

bool stack_term(prec_stack_t *head) {
	return STACK_FIRST->todo &&
		   STACK_FIRST->type == PREC_I;
}

bool stack_un_term(prec_stack_t *head) {
	return STACK_FIRST->type == PREC_I &&
		   STACK_FIRST->todo == false &&
		   STACK_SECOND != NULL &&
		   STACK_SECOND->type == PREC_UNARY;
}

bool stack_lparenthesis_term_rparenthesis(prec_stack_t *head) {
	return STACK_FIRST->type == PREC_R_PARENTHESIS &&
		   STACK_SECOND != NULL &&
		   STACK_SECOND->type == PREC_I &&
		   STACK_THIRD != NULL &&
		   STACK_THIRD->type == PREC_L_PARENTHESIS;
}

bool stack_term_op_term(prec_stack_t *head) {
	return STACK_FIRST->type == PREC_I &&
		   STACK_SECOND != NULL &&
		   STACK_SECOND->next != NULL &&
		   STACK_THIRD->type == PREC_I &&
		   STACK_THIRD->todo == true;
}

// grammar rules

int rule_i(parser_t parser, prec_stack_t *head) {
	// printf("E -> i\n");
	(*head)->todo = false;
	return EXIT_SUCCESS;
}

int rule_brackets(parser_t parser, prec_stack_t *head) {
	// printf("E -> (E)\n");

	stack_pop(head);
	token_t tk = (*head)->token;
	data_type_t dt = (*head)->data_type;
	stack_pop(head);
	stack_pop(head);

	stack_push(head, tk, PREC_I, dt);
	(*head)->todo = false;
	return EXIT_SUCCESS;
}

int rule_exit(parser_t parser, prec_stack_t *head) {
	// printf("E -> $\n");
	stack_pop(head); // remove E
	return EXIT_SUCCESS;
}

int rule_un(parser_t parser, prec_stack_t *head) {
	// printf("E -> +-!E\n");
	token_t tk = (*head)->token;
	data_type_t dt = (*head)->data_type;
	stack_pop(head);
	stack_pop(head);
	stack_push(head, tk, PREC_I, dt);
	(*head)->todo = false;
	return EXIT_SUCCESS;
}

int rule_mul_div(parser_t parser, prec_stack_t *head) {
	// printf("E -> E*/E\n");
	stack_pop(head);
	stack_pop(head);
	(*head)->todo = false;
	return EXIT_SUCCESS;
}

int rule_plus_minus(parser_t parser, prec_stack_t *head) {
	// printf("E -> E+-E\n");
	stack_pop(head);
	stack_pop(head);
	(*head)->todo = false;
	return EXIT_SUCCESS;
}

int rule_rel(parser_t parser, prec_stack_t *head) {
	// printf("E -> E<>E\n");
	stack_pop(head);
	stack_pop(head);
	(*head)->todo = false;
	return EXIT_SUCCESS;
}

int rule_equal(parser_t parser, prec_stack_t *head) {
	// printf("E -> E==E\n");
	stack_pop(head);
	stack_pop(head);
	(*head)->todo = false;
	return EXIT_SUCCESS;
}

int rule_and(parser_t parser, prec_stack_t *head) {
	// printf("E -> E && E\n");
	stack_pop(head);
	stack_pop(head);
	(*head)->todo = false;
	return EXIT_SUCCESS;
}

int rule_or(parser_t parser, prec_stack_t *head) {
	// printf("E -> E || E\n");
	stack_pop(head);
	stack_pop(head);
	(*head)->todo = false;
	return EXIT_SUCCESS;
}

int reduce(parser_t parser, prec_stack_t *head) {
	if (stack_term(head)) {
		return rule_i(parser, head);
	} else if (stack_un_term(head)) {
		return rule_un(parser, head);
	} else if (stack_lparenthesis_term_rparenthesis(head)) {
		return rule_brackets(parser, head);
	} else if (stack_term_op_term(head)) {
		switch (STACK_SECOND->type) {
			case PREC_PLUS_MINUS:
				return rule_plus_minus(parser, head);
			case PREC_MUL_DIV:
				return rule_mul_div(parser, head);
			case PREC_RELATION:
				return rule_rel(parser, head);
			case PREC_EQUAL:
				return rule_equal(parser, head);
			case PREC_AND:
				return rule_and(parser, head);
			case PREC_OR:
				return rule_or(parser, head);
		}
	}
	return ERROR_SYN;
}

///// Public

int parse_expr(parser_t parser) {
	prec_stack_t head = stack_init();
	if (head == NULL) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}

	// parse
	int res;
	prec_token_type type = convert_type(head, parser->token);
	CHECK_TYPE();
	do {
		switch (prec_table[HEAD()->type][type]) {
			case OPEN:
				head->todo = true;
				LOAD_NEXT();
				break;
			case CLOS:
				REDUCE(); // expect to be able to reduce one
				break;
			case EQUA:
				LOAD_NEXT();
				break;
			case EMPT: // empty means error
				stack_delete(head);
				return ERROR_SYN;
		}
	} while (!(type == PREC_DOLLAR && head->type == PREC_I && !head->todo && head->next->type == PREC_DOLLAR));

	// clear stack and exit
	rule_exit(parser, &head);
	stack_delete(head);

	return EXIT_SUCCESS;
}
