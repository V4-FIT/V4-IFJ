/**
 * @file generator_static.h
 * @author Adrián Kálazi
 * @brief Private helper functions and macros for the generator
 * @date 2020-12-08
 *
 */

#ifndef IFJ_GENERATOR_STATIC_H
#define IFJ_GENERATOR_STATIC_H

////// Macros

/**
 * @brief Single line comment macro
 *
 */
#define COMMENT(...)                   \
	do {                               \
		fputs("# ", stdout);           \
		INSTRUCTION_PART(__VA_ARGS__); \
		fputs("\n", stdout);           \
	} while (0)

/**
 * @brief Print variadic arguments macro
 *
 */
#define INSTRUCTION_PART(...)                                \
	do {                                                     \
		const char *_code[] = {__VA_ARGS__};                 \
		size_t count = sizeof(_code) / sizeof(const char *); \
		for (int i = 0; i < count; ++i) {                    \
			fputs(_code[i], stdout);                         \
		}                                                    \
	} while (0)

/**
 * @brief Insert a newline macro
 *
 */
#define INSTRUCTION_END() \
	do { fputs("\n", stdout); } while (0)

/**
 * @brief Print variadic arguments and insert a newline
 *
 */
#define INSTRUCTION(...)               \
	do {                               \
		INSTRUCTION_PART(__VA_ARGS__); \
		fputs("\n", stdout);           \
	} while (0)


////// Builtin function definitions (private)
////// These functions generate static code which does not depend on the input file

/**
 * @brief Builtin print function - print arguments until argument type is nil
 *
 */
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
	INSTRUCTION("PUSHS nil@nil");
	INSTRUCTION("RETURN");
}

/**
 * @brief
 * @param fname function name -> one of  {inputi, inputf, inputs, inputb}
 * @param type var type -> one of {int, float, string, bool}
 */
static void builtin_input(const char *fname, const char *type, const char *defvalue) {
	COMMENT("Builtin - inputs");

	// function label
	INSTRUCTION("LABEL ", fname);

	// read stdin and push on return stack
	INSTRUCTION("READ GF@rega ", type);

	// test READ result, jump to error if nil
	INSTRUCTION("JUMPIFEQ !", fname, " GF@rega nil@nil");

	// on success push return values to stack and return
	INSTRUCTION("PUSHS GF@rega");
	INSTRUCTION("PUSHS int@0");
	INSTRUCTION("RETURN");

	// end - error
	INSTRUCTION("LABEL !", fname);
	INSTRUCTION("PUSHS ", type, "@", defvalue);
	INSTRUCTION("PUSHS int@1");
	INSTRUCTION("RETURN");
}

/**
 * @brief Builtin int to float conversion function
 *
 */
static inline void builtin_int2float() {
	COMMENT("Builtin - int2float");

	// function label
	INSTRUCTION("LABEL int2float");

	// convert the argument (no other stack operation is neccessary)
	INSTRUCTION("INT2FLOATS");

	// end
	INSTRUCTION("RETURN");
}

/**
 * @brief Builtin float to int conversion function
 *
 */
static inline void builtin_float2int() {
	COMMENT("Builtin - float2int");

	// function label
	INSTRUCTION("LABEL float2int");

	// convert the argument (no other stack operation is neccessary)
	INSTRUCTION("FLOAT2INTS");

	// end
	INSTRUCTION("RETURN");
}

/**
 * @brief Builtin string length function
 *
 */
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

/**
 * @brief Builtin substring function
 *
 */
static inline void builtin_substr() {
	COMMENT("Builtin - substr");

	// function label
	INSTRUCTION("LABEL substr");
	INSTRUCTION("PUSHFRAME");
	INSTRUCTION("CREATEFRAME");

	// define arg vars
	INSTRUCTION("DEFVAR TF@s");
	INSTRUCTION("DEFVAR TF@i");
	INSTRUCTION("DEFVAR TF@n");

	// pop arguments
	// s - string
	INSTRUCTION("POPS TF@s");
	// i - begin pos
	INSTRUCTION("POPS TF@i");
	// n - length
	INSTRUCTION("POPS TF@n");

	// helper vars
	INSTRUCTION("DEFVAR TF@len");
	INSTRUCTION("STRLEN TF@len TF@s");

	// test i < 0 => err
	INSTRUCTION("LT GF@rega TF@i int@0");
	INSTRUCTION("JUMPIFEQ !substr_err GF@rega bool@true");

	// test i >= len(s) => err
	INSTRUCTION("JUMPIFEQ !substr_err TF@i TF@len"); // if i == len(s)
	INSTRUCTION("GT GF@rega TF@i TF@len");
	INSTRUCTION("JUMPIFEQ !substr_err GF@rega bool@true");

	// test n < 0 => err
	INSTRUCTION("LT GF@rega TF@n int@0");
	INSTRUCTION("JUMPIFEQ !substr_err GF@rega bool@true");

	// actual substr
	INSTRUCTION("MOVE GF@regb string@"); // resulting string
	INSTRUCTION("LABEL ?substr_loop");
	INSTRUCTION("JUMPIFEQ !substr_succ TF@n int@0");
	INSTRUCTION("JUMPIFEQ !substr_succ TF@i TF@len");
	INSTRUCTION("GETCHAR GF@rega TF@s TF@i");
	INSTRUCTION("CONCAT GF@regb GF@regb GF@rega");
	INSTRUCTION("SUB TF@n TF@n int@1");
	INSTRUCTION("ADD TF@i TF@i int@1");
	INSTRUCTION("JUMP ?substr_loop");

	// success
	INSTRUCTION("LABEL !substr_succ");
	INSTRUCTION("PUSHS GF@regb");
	INSTRUCTION("PUSHS int@0");
	INSTRUCTION("JUMP !substr");

	// error
	INSTRUCTION("LABEL !substr_err");
	INSTRUCTION("PUSHS string@");
	INSTRUCTION("PUSHS int@1");

	// end
	INSTRUCTION("LABEL !substr");
	INSTRUCTION("POPFRAME");
	INSTRUCTION("RETURN");
}

/**
 * @brief Buildin ordinal function
 *
 */
static inline void builtin_ord() {
	COMMENT("Builtin - ord");

	// function label
	INSTRUCTION("LABEL ord");
	INSTRUCTION("PUSHFRAME");
	INSTRUCTION("CREATEFRAME");

	// define arg vars
	INSTRUCTION("DEFVAR TF@s");
	INSTRUCTION("DEFVAR TF@i");

	// pop argument
	// s
	INSTRUCTION("POPS TF@s");
	// i
	INSTRUCTION("POPS TF@i");

	// test i < 0
	INSTRUCTION("LT GF@rega TF@i int@0");
	INSTRUCTION("JUMPIFEQ !ord_err GF@rega bool@true");

	// test i >= len(s)
	INSTRUCTION("STRLEN GF@rega TF@s");
	INSTRUCTION("JUMPIFEQ !ord_err TF@i GF@rega"); // if i == len(s)
	INSTRUCTION("GT GF@rega TF@i GF@rega");
	INSTRUCTION("JUMPIFEQ !ord_err GF@rega bool@true");

	// actual ord
	INSTRUCTION("STRI2INT TF@s TF@s TF@i");
	INSTRUCTION("PUSHS TF@s");
	INSTRUCTION("PUSHS int@0");
	INSTRUCTION("JUMP !ord");

	// error
	INSTRUCTION("LABEL !ord_err");
	INSTRUCTION("PUSHS int@0");
	INSTRUCTION("PUSHS int@1");

	// end
	INSTRUCTION("LABEL !ord");
	INSTRUCTION("POPFRAME");
	INSTRUCTION("RETURN");
}

/**
 * @brief builtin character function
 *
 */
static inline void builtin_chr() {
	COMMENT("Builtin - chr");

	// function label
	INSTRUCTION("LABEL chr");

	// pop argument
	INSTRUCTION("POPS GF@regb");

	// test i < 0
	INSTRUCTION("LT GF@rega GF@regb int@0");
	INSTRUCTION("JUMPIFEQ !chr_err GF@rega bool@true");

	// test i > 255
	INSTRUCTION("GT GF@rega GF@regb int@255");
	INSTRUCTION("JUMPIFEQ !chr_err GF@rega bool@true");

	// actual chr
	INSTRUCTION("PUSHS GF@regb");
	INSTRUCTION("INT2CHARS");
	INSTRUCTION("PUSHS int@0");
	INSTRUCTION("JUMP !chr");

	// error
	INSTRUCTION("LABEL !chr_err");
	INSTRUCTION("PUSHS string@");
	INSTRUCTION("PUSHS int@1");

	// end
	INSTRUCTION("LABEL !chr");
	INSTRUCTION("RETURN");
}

/**
 * @brief Create header
 *
 */
static inline void header() {
	COMMENT("begin");

	INSTRUCTION(".IFJcode20");

	INSTRUCTION("DEFVAR GF@rega");
	INSTRUCTION("DEFVAR GF@regb");

	INSTRUCTION("PUSHS nil@nil");
	INSTRUCTION("CREATEFRAME");
	INSTRUCTION("CALL main");

	INSTRUCTION("JUMP !_main");
}

/**
 * @brief Define builtin functions
 *
 */
static inline void builtin_define() {
	COMMENT("Builtin function definitions - start");

	builtin_print();
	builtin_input("inputi", "int", "0");
	builtin_input("inputf", "float", "0x0p+0");
	builtin_input("inputs", "string", "");
	builtin_input("inputb", "bool", "false");
	builtin_int2float();
	builtin_float2int();
	builtin_len();
	builtin_substr();
	builtin_ord();
	builtin_chr();

	COMMENT("Builtin function definitions - end");
}

#endif // !IFJ_GENERATOR_BUILTIN_H
