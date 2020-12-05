#include "generator.h"

#include <stdio.h>
#include <ctype.h>

#include "error.h"

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

////// Macros

/**
 * Single line comment
 */
#define COMMENT(...)                   \
	do {                               \
		fputs("# ", stdout);           \
		INSTRUCTION_PART(__VA_ARGS__); \
		fputs("\n", stdout);           \
	} while (0)

/**
 * Print variadic arguments
 */
#define INSTRUCTION_PART(...)                               \
	do {                                                    \
		const char *_asm[] = {__VA_ARGS__};                 \
		size_t count = sizeof(_asm) / sizeof(const char *); \
		for (int i = 0; i < count; ++i) {                   \
			fputs(_asm[i], stdout);                         \
		}                                                   \
	} while (0)

/**
 * Insert a newline
 */
#define INSTRUCTION_END(...) \
	do {                     \
		fputs("\n", stdout); \
	} while (0)

/**
 * Print variadic arguments and insert a newline
 */
#define INSTRUCTION(...)               \
	do {                               \
		INSTRUCTION_PART(__VA_ARGS__); \
		fputs("\n", stdout);           \
	} while (0)

////// Conversion tables and functions (private)

static const char *tk_type2type[] = {
		[TK_INT_LIT] = "int",
		[TK_FLOAT_LIT] = "float",
		[TK_STR_LIT] = "string",
		[TK_KEYW_TRUE] = "bool",
		[TK_KEYW_FALSE] = "bool",
		[TK_KEYW_BOOL] = NULL, // last
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

////// Builtin function definitions (private)

static inline void builtin_print() {
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
static inline void builtin_input(const char *fname, const char *type) {
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

static inline void builtin_int2float() {
	COMMENT("Builtin - int2float");

	// function label
	INSTRUCTION("LABEL int2float");

	// convert the argument (no other stack operation is neccessary)
	INSTRUCTION("INT2FLOATS");

	// end
	INSTRUCTION("RETURN");
}

static inline void builtin_float2int() {
	COMMENT("Builtin - float2int");

	// function label
	INSTRUCTION("LABEL float2int");

	// convert the argument (no other stack operation is neccessary)
	INSTRUCTION("FLOAT2INTS");

	// end
	INSTRUCTION("RETURN");
}

static inline void builtin_len() {
	COMMENT("Builtin - len");

	// function label
	INSTRUCTION("LABEL len");

	// pop argument
	INSTRUCTION("POPS GF@rega");

	// calculate strlen
	INSTRUCTION("STRLEN GF@regb GF@rega");

	// push back the length
	INSTRUCTION("PUSHS GF@regb");

	// end
	INSTRUCTION("RETURN");
}

static inline void builtin_substr();
static inline void builtin_ord();
static inline void builtin_chr();

////// Code segment generation -> private generators

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
	builtin_len();
}

static void push_token(token_t token) {
	switch (token->type) {
		case TK_IDENTIFIER:
			INSTRUCTION("PUSHS TF@", token->param.s);
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
void gen_var_neg(token_type_t type) {
	INSTRUCTION("POPS GF@rega");
	INSTRUCTION("PUSHS ", tk_type2type[type], "@0");
	INSTRUCTION("PUSHS GF@rega");
	INSTRUCTION("SUBS");
}

void gen_var_not() {
	INSTRUCTION("NOTS");
}
