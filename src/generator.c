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
 * SCOPES
 * Each variable identifier with has an unique name $
 *
 * Allowed label chars:
 *  _ - $ & % * ! ?
 *
 * Label prefixes: <outdated>
 * ^! -> function end (cleanup or error)
 * ^? -> loop label
 * ^$ -> conditional label
 * !_main -> program end
 *
 * TODO
 * - think about how the else label will look like -> this is generated after expression evaluation
 * - return statement in functions
 * - definitions in for
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

static void immersion_label(flist_iterator_t immersion) {
	for (flist_iterator_t it = immersion; flist_it_valid(it); it = flist_it_next(it)) {
		assert(flist_get(it) != NULL);
		INSTRUCTION_PART("?");
		INSTRUCTION_PART(*((char **)flist_get(it)));
	}
}

static void immersion_var(flist_iterator_t immersion, const char *name) {
	for (flist_iterator_t it = immersion; flist_it_valid(it); it = flist_it_next(it)) {
		assert(flist_get(it) != NULL);
		INSTRUCTION_PART(*((char **)flist_get(it)));
		INSTRUCTION_PART("$");
	}
	INSTRUCTION_PART(name);
}

static void push_identifier(symbol_ref_t symbol_ref) {
	assert(symbol_ref.symbol != NULL);

	INSTRUCTION_PART("PUSHS TF@");
	immersion_var(symbol_ref.immersion_it, symbol_ref.symbol->name);
	INSTRUCTION_END();
}

static void push_literal(token_t token) {
	assert(token != NULL);

	switch (token->type) {
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

static void concat_stack() {
	INSTRUCTION("POPS GF@rega");
	INSTRUCTION("POPS GF@regb");
	INSTRUCTION("CONCAT GF@rega GF@regb GF@rega");
	INSTRUCTION("PUSHS GF@rega");
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

void gen_func_param(symbol_ref_t symbol_ref) {
	assert(symbol_ref.symbol != NULL);

	INSTRUCTION_PART("DEFVAR TF@");
	immersion_var(symbol_ref.immersion_it, symbol_ref.symbol->name);
	INSTRUCTION_END();
	INSTRUCTION_PART("POPS TF@");
	immersion_var(symbol_ref.immersion_it, symbol_ref.symbol->name);
	INSTRUCTION_END();
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
void gen_func_call_arg(symtable_t symtable, token_t token) {
	gen_var_load(symtable, token);
}

void gen_var_define(symbol_ref_t symbol_ref) {
	assert(symbol_ref.symbol != NULL);

	INSTRUCTION_PART("DEFVAR TF@");
	immersion_var(symbol_ref.immersion_it, symbol_ref.symbol->name);
	INSTRUCTION_END();
}

/**
 * Pushes the literal/identifier onto the stack for further computation
 * @param token literal or identifier token
 */
void gen_var_load(symtable_t symtable, token_t token) {
	if (token->type == TK_IDENTIFIER) {
		push_identifier(symtable_find(symtable, token));
	} else {
		push_literal(token);
	}
}

/**
 * Pushes the identifier onto the stack, before the last pushed item for further computation
 * @param identifier name
 */
void gen_var_load_id_before(symbol_ref_t symbol_ref) {
	assert(symbol_ref.symbol != NULL);

	INSTRUCTION("POPS GF@rega");
	INSTRUCTION_PART("PUSHS TF@");
	immersion_var(symbol_ref.immersion_it, symbol_ref.symbol->name);
	INSTRUCTION_END();
	INSTRUCTION("PUSHS GF@rega");
}

/**
 * Assigns expression result to a frame variable
 * @param identifier
 */
void gen_var_assign_expr_result(symbol_ref_t symbol_ref) {
	if (symbol_ref.symbol == NULL) { // in case of _ = ...
		// TODO: maybe test if this really happens only for _
		INSTRUCTION("POPS GF@rega");
		return;
	}

	INSTRUCTION_PART("POPS TF@");
	immersion_var(symbol_ref.immersion_it, symbol_ref.symbol->name);
	INSTRUCTION_END();
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
		case TK_PLUS_ASSIGN:
		case TK_PLUS:
			if (data_type == DT_STRING) {
				concat_stack();
			} else {
				INSTRUCTION("ADDS");
			}
			break;
		case TK_MINUS_ASSIGN:
		case TK_MINUS:
			INSTRUCTION("SUBS");
			break;
		case TK_MULTIPLY_ASSIGN:
		case TK_MULTIPLY:
			INSTRUCTION("MULS");
			break;
		case TK_DIVIDE_ASSIGN:
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

void gen_for_label_condition(flist_iterator_t immersion) {
	INSTRUCTION_PART("LABEL ");
	immersion_label(immersion);
	INSTRUCTION_END();
}

void gen_for_label_assignment(flist_iterator_t immersion) {
	INSTRUCTION_PART("LABEL *");
	immersion_label(immersion);
	INSTRUCTION_END();
}

void gen_for_label_content(flist_iterator_t immersion) {
	INSTRUCTION_PART("LABEL _");
	immersion_label(immersion);
	INSTRUCTION_END();
}

void gen_for_label_end(flist_iterator_t immersion) {
	INSTRUCTION_PART("LABEL !");
	immersion_label(immersion);
	INSTRUCTION_END();
}

void gen_for_jump_condition(flist_iterator_t immersion) {
	INSTRUCTION_PART("JUMP ");
	immersion_label(immersion);
	INSTRUCTION_END();
}

void gen_for_jump_assignment(flist_iterator_t immersion) {
	INSTRUCTION_PART("JUMP *");
	immersion_label(immersion);
	INSTRUCTION_END();
}

void gen_for_jump_content(flist_iterator_t immersion) {
	INSTRUCTION_PART("JUMP _");
	immersion_label(immersion);
	INSTRUCTION_END();
}

void gen_for_jump_cond_end(flist_iterator_t immersion) {
	INSTRUCTION("PUSHS bool@true");
	INSTRUCTION_PART("JUMPIFNEQS !");
	immersion_label(immersion);
	INSTRUCTION_END();
}
