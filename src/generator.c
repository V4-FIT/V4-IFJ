#include "generator.h"

#include <stdio.h>

/**
 * Some notes on code generation
 *
 * Function parameters are stored on the stack, until nil is met (stack root value must be nil)
 *
 * Label prefixes:
 * ^! -> function end (cleanup)
 * ^? -> loop label
 */

////// Macros

/**
 * Single line comment
 */
#define COMMENT(_txt) \
do { \
    fputs("# " _txt "\n", stdout); \
} while (0)

/**
 * Print variadic arguments and insert a newline
 */
#define INSTRUCTION(...) \
do { \
    const char *_asm[] = {__VA_ARGS__}; \
    size_t count = sizeof(_asm)/sizeof(const char *); \
    for (int i = 0; i < count; ++i) { \
        fputs(_asm[i], stdout); \
    } \
    fputs("\n", stdout); \
} while (0)

////// Builtin functions (private)

static void builtin_print() {
	COMMENT("Builtin - print");

	// function label
	INSTRUCTION("LABEL print");

	// loop label
	INSTRUCTION("LABEL ?print_loop");

	// pop arg and test if type is nil -> if yes jump to end
	INSTRUCTION("POPS GF@rega");
	INSTRUCTION("TYPE GF@regb GF@rega");
	INSTRUCTION("JUMPIFEQ !print GF@regb nil@nil");

	// if here, print the arg and loop back
	INSTRUCTION("WRITE GF@rega");
	INSTRUCTION("JUMP ?print_loop");

	// end
	INSTRUCTION("LABEL !print");
	INSTRUCTION("RETURN");
}

////// Code segment generation

/// Private

static void header() {
	COMMENT("begin");

	INSTRUCTION(".IFJcode20");

	// some global "registers" to avoid frame creation for simple functions
	INSTRUCTION("DEFVAR GF@rega");
	INSTRUCTION("DEFVAR GF@regb");
	INSTRUCTION("DEFVAR GF@regc");

	INSTRUCTION("CREATEFRAME");
	INSTRUCTION("PUSHFRAME");
	INSTRUCTION("JUMP main");
}

static void builtin_define() {
	COMMENT("Builtin function definitions");

	builtin_print();
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

void gen_func_end() {
	COMMENT("End funtion");

	INSTRUCTION("POPFRAME");
	INSTRUCTION("RETURN");
}

void gen_finish() {
	INSTRUCTION("POPFRAME");

	// just for the sake of completeness
	fflush(stdout);
}


