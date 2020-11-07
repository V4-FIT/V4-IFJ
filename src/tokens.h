#ifndef IFJ_TOKENS_H
#define IFJ_TOKENS_H

#include <stdint.h>

#include "char_sequence.h"

/*tokens produced by scanner*/
typedef enum
{
	// Special purpose
	TK_ERROR,
	TK_EOF,

	// Separators
	TK_L_PARENTHESIS,
	TK_R_PARENTHESIS,
	TK_L_CURLY,
	TK_R_CURLY,
	TK_SEPARATOR,
	TK_SEMICOLON,
	TK_EOL,

	// Literals
	TK_STR_LIT,
	TK_INT_LIT,
	TK_FLOAT_LIT,

	// Arithmetic operators
	TK_PLUS,
	TK_MINUS,
	TK_MULTIPLY,
	TK_DIVIDE,

	// Relational operators
	TK_EQUAL,
	TK_LESS,
	TK_LESS_EQUAL,
	TK_GREATER,
	TK_GREATER_EQUAL,
	TK_NOT_EQUAL,

	// Logical operators
	TK_OR,
	TK_AND,
	TK_NOT,

	// Assignment operators
	TK_VAR_INIT,
	TK_ASSIGN,
	TK_PLUS_ASSIGN,
	TK_MINUS_ASSIGN,
	TK_MULTIPLY_ASSIGN,
	TK_DIVIDE_ASSIGN,

	// Identifiers
	TK_IDENTIFIER,
	TK_UNDERSCORE,

	// Keywords
	TK_KEYW_PACKAGE,
	TK_KEYW_FUNC,
	TK_KEYW_MAIN,
	TK_KEYW_RETURN,
	TK_KEYW_IF,
	TK_KEYW_ELSE,
	TK_KEYW_FOR,
	TK_KEYW_TRUE,
	TK_KEYW_FALSE,
	TK_KEYW_INT,
	TK_KEYW_FLOAT64,
	TK_KEYW_STRING,
	TK_KEYW_BOOL
} token_type_t;


/*data type to represent tokens created by scanner*/
typedef struct Token
{
	token_type_t type;
	union {
		const char *s;
		uint64_t i;
		double f;
	} param;
} *token_t;


#endif // !IFJ_TOKENS_H
