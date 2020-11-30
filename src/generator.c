#include "generator.h"

#include <stdio.h>
#include <ctype.h>

/**
 * Some notes on code generation
 *
 * TERMINOLOGY
 * return stack vs argument stack - same physical stack but in a different context
 *
 * CALLING CONVENTION:
 * Functions use the CDECL calling convention with the exception that
 * arguments are in reverse order and
 * return values are pushed onto the stack instead of passed trough a register
 * (this design choice is the result of multiple possible return values instead of one)
 * The root of argument stack is NIL -> for variadic arguments as in print(...)
 * RETURN VAUES are in order from LEFT to RIGHT [fun(0, 1, 2) (0, 1, 2)] (order of stack pushes)
 * TODO: When 2-pass is complete decide which order would be better
 * TL;DR: Function arguments and return values are stored on the stack until nil is met (stack root value must be nil)
 *
 * Allowed label chars:
 *  _ - $ & % * ! ?
 *
 * Label prefixes:
 * ^! -> function end (cleanup or error)
 * ^? -> loop label
 * !_main -> program end
 */

////// Conversion tables and functions

static const char *tk2type[] = {
		[TK_STR_LIT] = "string",
		[TK_INT_LIT] = "string",
		[TK_FLOAT_LIT] = "float",
		[TK_KEYW_TRUE] = "bool",
		[TK_KEYW_FALSE] = "bool",
};

static const char *encode_string_literal(const char *string) {
	for (const char *c = string; *c != '\0'; c++) {
		if (isalnum(*c)) {
			putc(*c, stdout);
		} else {
			printf("\\%03d", *c);
		}
	}
	return ""; // to be able to be passed to fputs
}

////// Macros

/**
 * Single line comment
 */
#define COMMENT(_txt) \
do { \
    fputs("# " _txt "\n", stdout); \
} while (0)

/**
 * Print variadic arguments
 */
#define INSTRUCTION_PART(...) \
do { \
    const char *_asm[] = {__VA_ARGS__}; \
    size_t count = sizeof(_asm)/sizeof(const char *); \
    for (int i = 0; i < count; ++i) { \
        fputs(_asm[i], stdout); \
    } \
} while (0)

/**
 * Insert a newline
 */
#define INSTRUCTION_END(...) \
do { \
    fputs("\n", stdout); \
} while (0)

/**
 * Print variadic arguments and insert a newline
 */
#define INSTRUCTION(...) \
do { \
    INSTRUCTION_PART(__VA_ARGS__); \
    fputs("\n", stdout); \
} while (0)

////// Builtin function definitions (private)

static void builtin_print() {
	COMMENT("Builtin - print");

	// function label
	INSTRUCTION("LABEL print");

	// loop label
	INSTRUCTION("LABEL ?print_loop");

	// pop arg and test if type is nil -> if yes jump to end
	INSTRUCTION("POPS GF@rega");
	INSTRUCTION("JUMPIFEQ !print GF@rega nil@nil");

	// if here, print the arg and loop back
	INSTRUCTION("WRITE GF@rega");
	INSTRUCTION("JUMP ?print_loop");

	// end
	INSTRUCTION("LABEL !print");
	INSTRUCTION("RETURN");
}

/**
 * @param fname function name -> one of  {inputi, inputf, inputs, inputb}
 * @param type var type -> one of {int, float, string, bool}
 */
static void builtin_input(const char *fname, const char *type) {
	COMMENT("Builtin - inputs");

	// function label
	INSTRUCTION("LABEL ", fname);

	// initialize return stack
	INSTRUCTION("PUSHS nil@nil");

	// read stdin and push on return stack
	INSTRUCTION("READ GF@rega ", type);
	INSTRUCTION("PUSHS GF@rega");

	// test type, jump to error if nil
	INSTRUCTION("JUMPIFEQ !", fname, " GF@rega nil@nil");

	// on success push return values to stack and return
	INSTRUCTION("PUSHS int@0");
	INSTRUCTION("RETURN");

	// end - error
	INSTRUCTION("LABEL !", fname);
	INSTRUCTION("PUSHS int@1");
	INSTRUCTION("RETURN");
}

static void builtin_int2float() {
	COMMENT("Builtin - int2float");

	// function label
	INSTRUCTION("LABEL int2float");

	// convert the argument (no other stack operation is neccessary)
	INSTRUCTION("INT2FLOATS");

	// end
	INSTRUCTION("RETURN");
}

static void builtin_float2int() {
	COMMENT("Builtin - float2int");

	// function label
	INSTRUCTION("LABEL float2int");

	// convert the argument (no other stack operation is neccessary)
	INSTRUCTION("FLOAT2INTS");

	// end
	INSTRUCTION("RETURN");
}

////// Code segment generation

/// Private

static void header() {
	COMMENT("begin");

	INSTRUCTION(".IFJcode20");

	// some global "registers" to avoid frame creation for simple functions
	// TODO: Remove unused registers
	INSTRUCTION("DEFVAR GF@rega");
	INSTRUCTION("DEFVAR GF@regb");
	INSTRUCTION("DEFVAR GF@regc");

	INSTRUCTION("CREATEFRAME");
	INSTRUCTION("CALL main");

	INSTRUCTION("JUMP !_main");
}

static void builtin_define() {
	COMMENT("Builtin function definitions");

	builtin_print();
	builtin_input("inputi", "int");
	builtin_input("inputf", "float");
	builtin_input("inputs", "string");
	builtin_input("inputb", "bool");
	builtin_int2float();
	builtin_float2int();
}

/// Public

void gen_init() {
	header();
	builtin_define();
}

void gen_func_begin(const char *identifier) {
	COMMENT("Begin function");

	INSTRUCTION("LABEL ", identifier);
	INSTRUCTION("PUSHFRAME");
}

void gen_func_init_frame() {
	INSTRUCTION("CREATEFRAME");
}

void gen_func_init_stack() {
	INSTRUCTION("PUSHS nil@nil");
}

void gen_func_param(const char *identifier) {
	INSTRUCTION("DEFVAR TF@", identifier);
	INSTRUCTION("POPS TF@", identifier);
}

void gen_func_call(const char *identifier) {
	INSTRUCTION("CALL ", identifier);
}

void gen_func_callarg_literal(token_t token) {
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
			encode_string_literal(token->param.s);
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

void gen_func_end() {
	COMMENT("End funtion");

	INSTRUCTION("POPFRAME");
	INSTRUCTION("RETURN");
}

void gen_finish() {
	INSTRUCTION("LABEL !_main");

	// just for the sake of completeness
	fflush(stdout);
}


