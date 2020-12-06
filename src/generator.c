#include "generator.h"

#include <stdio.h>
#include <ctype.h>

#include "error.h"
#include "generator_static.h"

/**
 * Some notes on code generation
 *
 * TERMINOLOGY
 * return stack vs argument stack - same physical stack but in a different context
 *
 * CALLING CONVENTION:
 * Functions use the CDECL calling convention with the exception that
 * return values are pushed onto the stack instead of passed trough a register
 * (this design choice is the result of multiple possible return values instead of one)
 * The root of argument stack is NIL -> for variadic arguments as in print(...)
 * RETURN VAUES are in order from LEFT to RIGHT [fun(2, 1, 0) (0, 1, 2)] (order of stack pushes)
 * TL;DR: Function arguments and return values are stored on the stack until nil is met (stack root value must be nil)
 *
 * Expression operations are implemented similar to FPU (stack) operations
 *
 * Allowed label chars:
 *  _ - $ & % * ! ?
 *
 * Label prefixes:
 * ^! -> function end (cleanup or error)
 * ^? -> loop label
 * !_main -> program end
 *
 * TODO
 * - scoped variables
 * - check the usage of gen_func_init_stack for redundant nil-s left in the stack
 */

////// Conversion tables and functions (private)

static const char *tk_type2type[] = {
		[DT_UNDEFINED] = NULL,
		[DT_INTEGER] = "int",
		[DT_FLOAT64] = "float",
		[DT_STRING] = "string",
		[DT_BOOL] = "bool",
};

static void encode_string_literal(const char *string) {
	for (const char *c = string; *c != '\0'; c++) {
		if (isalnum(*c)) {
			putc(*c, stdout);
		} else {
			printf("\\%03d", *c);
		}
	}
}

////// Dynamic code segment generation -> private

static void push_token(token_t token) {
	switch (token->type) {
		case TK_IDENTIFIER:
			INSTRUCTION("PUSHS TF@", token->lexeme);
			break;
		case TK_INT_LIT:
			INSTRUCTION_PART("PUSHS int@");
			printf("%lld", token->param.i);
			INSTRUCTION_END();
			break;
		case TK_FLOAT_LIT:
			INSTRUCTION_PART("PUSHS float@");
			printf("%a", token->param.f);
			INSTRUCTION_END();
			break;
		case TK_STR_LIT:
			INSTRUCTION_PART("PUSHS string@");
			encode_string_literal(token->lexeme);
			INSTRUCTION_END();
			break;
		case TK_KEYW_TRUE:
			INSTRUCTION("PUSHS bool@true");
			break;
		case TK_KEYW_FALSE:
			INSTRUCTION("PUSHS bool@false");
			break;
		default:
			break;
	}
}

////// Code segment generation -> public interface implementation

void gen_init() {
	header();
	builtin_define();
}

void gen_finish() {
	INSTRUCTION("LABEL !_main");

#ifndef NDEBUG
	INSTRUCTION("BREAK");
#endif

	// just for the sake of completeness
	fflush(stdout);
}

/// Function definition

void gen_func_begin(const char *identifier) {
	COMMENT("Begin function - ", identifier);

	INSTRUCTION("LABEL ", identifier);
	INSTRUCTION("PUSHFRAME");
	INSTRUCTION("CREATEFRAME");
}

void gen_func_init_stack() {
	INSTRUCTION("PUSHS nil@nil");
}

void gen_func_restore_stack() {
	INSTRUCTION("POPS GF@rega");
}

void gen_func_param(const char *identifier) {
	INSTRUCTION("DEFVAR TF@", identifier);
	INSTRUCTION("POPS TF@", identifier);
}

void gen_func_end() {
	INSTRUCTION("POPFRAME");
	INSTRUCTION("RETURN");

	COMMENT("End funtion");
}

/// Function call

void gen_func_call(const char *identifier) {
	INSTRUCTION("CALL ", identifier);
}

/**
 * Pushes the function call argument onto the argument stack
 * @param token literal or identifier token
 */
void gen_func_call_arg(token_t token) {
	push_token(token);
}

void gen_var_define(const char *identifier) {
	INSTRUCTION("DEFVAR TF@", identifier);
}

/**
 * Pushes the literal/variable onto the stack for further computation
 * @param token literal or identifier token
 */
void gen_var_load(token_t token) {
	if (token == NULL) {
		return;
	}

	push_token(token);
}

/**
 * Assigns expression result to a frame variable
 * @param identifier
 */
void gen_var_assign_expr_result(const char *identifier) {
	INSTRUCTION("POPS TF@", identifier);
}

/**
 * Generates an operator specific instruction
 * @param operator token type, one of {+ - * /}
 */
void gen_var_operator(token_t operator) {
	if (operator== NULL) {
		INFO("Operator is NULL!\n");
		return;
	}

	switch (operator->type) {
		case TK_PLUS:
			INSTRUCTION("ADDS");
			break;
		case TK_MINUS:
			INSTRUCTION("SUBS");
			break;
		case TK_MULTIPLY:
			INSTRUCTION("MULS");
			break;
		case TK_DIVIDE:
			INSTRUCTION("DIVS");
			// TODO: For floats
			// INSTRUCTION("IDIVS");
			break;
		default:
			break;
	}
}

/**
 * Negates the last stack value
 * @param type type of variable to be negated, one of {int, float}
 */
void gen_var_neg(data_type_t type) {
	INSTRUCTION("POPS GF@rega");
	INSTRUCTION("PUSHS ", tk_type2type[type], "@0");
	INSTRUCTION("PUSHS GF@rega");
	INSTRUCTION("SUBS");
}

void gen_var_not() {
	INSTRUCTION("NOTS");
}
