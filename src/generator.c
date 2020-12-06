#include "generator.h"

#include <stdio.h>
#include <ctype.h>
#include <assert.h>

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
 * The root nil should not be removed
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
 * - check if every generation is after a semantic test
 * - test div and idiv
 */

////// Conversion tables and functions (private)

static const char *datatype2metatype[] = {
		[DT_UNDEFINED] = NULL,
		[DT_INTEGER] = "int",
		[DT_FLOAT64] = "float",
		[DT_STRING] = "string",
		[DT_BOOL] = "bool",
};

static void encode_string_literal(const char *string) {
	if (string == NULL) {
		return;
	}

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
	assert(token != NULL);

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
			assert(false);
	}
}

////// Code segment generation -> public interface implementation

void gen_init() {
	header();
	builtin_define();
}

void gen_finish() {
	INSTRUCTION("LABEL !_main");
	INSTRUCTION("CLEARS");

#ifndef NDEBUG
	INSTRUCTION("BREAK");
#endif

	// just for the sake of completeness
	fflush(stdout);
}

/// Function definition

void gen_func_begin(const char *identifier) {
	assert(identifier != NULL);

	COMMENT("Begin function - ", identifier);

	INSTRUCTION("LABEL ", identifier);
	INSTRUCTION("PUSHFRAME");
	INSTRUCTION("CREATEFRAME");
}

void gen_func_param(const char *identifier) {
	assert(identifier != NULL);

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
	assert(identifier != NULL);

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
	assert(identifier != NULL);

	INSTRUCTION("DEFVAR TF@", identifier);
}

/**
 * Pushes the literal/variable onto the stack for further computation
 * @param token literal or identifier token
 */
void gen_var_load(token_t token) {
	push_token(token);
}

/**
 * Assigns expression result to a frame variable
 * @param identifier
 */
void gen_var_assign_expr_result(const char *identifier) {
	assert(identifier != NULL);

	INSTRUCTION("POPS TF@", identifier);
}

/**
 * Generates a unary operator specific instruction(s)
 * @param operator token type, one of {+ - !}
 */
void gen_var_operator_unary(token_type_t operator, data_type_t data_type) {
	switch (operator) {
		case TK_PLUS:
			break;
		case TK_MINUS:
			assert(data_type == DT_INTEGER || data_type == DT_FLOAT64);
			INSTRUCTION("POPS GF@rega");
			INSTRUCTION("PUSHS ", datatype2metatype[data_type], "@0");
			INSTRUCTION("PUSHS GF@rega");
			INSTRUCTION("SUBS");
			break;
		case TK_NOT:
			INSTRUCTION("NOTS");
			break;
		default:
			assert(false);
	}
}

/**
 * Generates a binary operator specific instruction
 * @param operator token type, one of {+ - * / == != <= < >= > && ||}
 */
void gen_var_operator_binary(token_type_t operator, data_type_t data_type) {
	switch (operator) {
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
			switch (data_type) {
				case DT_INTEGER:
					INSTRUCTION("IDIVS");
					break;
				case DT_FLOAT64:
					INSTRUCTION("DIVS");
					break;
				default:
					assert(false);
			}
			break;
		case TK_EQUAL:
			INSTRUCTION("EQS");
			break;
		case TK_NOT_EQUAL:
			INSTRUCTION("EQS");
			INSTRUCTION("NOTS");
			break;
		case TK_LESS_EQUAL:
			INSTRUCTION("POPS GF@regb");
			INSTRUCTION("POPS GF@rega");
			INSTRUCTION("PUSHS GF@rega");
			INSTRUCTION("PUSHS GF@regb");
			INSTRUCTION("EQS");
			INSTRUCTION("PUSHS GF@rega");
			INSTRUCTION("PUSHS GF@regb");
			INSTRUCTION("LTS");
			INSTRUCTION("ORS");
			break;
		case TK_LESS:
			INSTRUCTION("LTS");
			break;
		case TK_GREATER_EQUAL:
			INSTRUCTION("POPS GF@regb");
			INSTRUCTION("POPS GF@rega");
			INSTRUCTION("PUSHS GF@rega");
			INSTRUCTION("PUSHS GF@regb");
			INSTRUCTION("EQS");
			INSTRUCTION("PUSHS GF@rega");
			INSTRUCTION("PUSHS GF@regb");
			INSTRUCTION("GTS");
			INSTRUCTION("ORS");
			break;
		case TK_GREATER:
			INSTRUCTION("GTS");
			break;
		case TK_AND:
			INSTRUCTION("ANDS");
			break;
		case TK_OR:
			INSTRUCTION("ORS");
			break;
		default:
			assert(false);
	}
}
