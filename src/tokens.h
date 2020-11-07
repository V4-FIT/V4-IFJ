#ifndef IFJ_TOKENS_H
#define IFJ_TOKENS_H

#include <stdint.h>

#include "char_sequence.h"

/*tokens produced by scanner*/
typedef enum
{
	TK_ERROR,
	TK_IDENTIFIER,
	TK_STR_LIT,
	TK_EOL,
	TK_R_PARANTHESIS,
	TK_L_PARANTHESIS,
	TK_EOF,
	TK_SEPARATOR,
	TK_SEMICOLON,
	TK_L_CURLY,
	TK_R_CURLY,
	TK_OR,
	TK_AND,
	TK_VAR_INIT,
	TK_ASSIGN,
	TK_EQUAL,
	TK_LESS,
	TK_LESS_EQUAL,
	TK_GREATER,
	TK_GREATER_EQUAL,
	TK_NOT,
	TK_NOT_EQUAL,
	TK_PLUS,
	TK_INCREMENT,
	TK_MINUS,
	TK_DECREMENT,
	TK_STAR,
	TK_TIMES,
	TK_SLASH,
	TK_DIVIDE,
	TK_INT_LIT,
	TK_FLOAT_LIT,
	TK_UNDERSCORE,
	//keywords:
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
	union TokenParam {
		const char *s;
		uint64_t i;
		double f;
	} param;
} *token_t;


#endif // !IFJ_TOKENS_H
