#ifndef IFJ_TOKENS_H
#define IFJ_TOKENS_H

#include "char_sequence.h"

/*tokens produced by scanner*/
typedef enum
{
	TK_ERROR, //lexical error
	TK_INTERNAL_ERROR, //malloc errors
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
	TK_VAR_INNIT,
	TK_ASSIGN,
	TK_EQUAL,
	TK_LESS,
	TK_LESS_EQUAL,
	TK_GREATER,
	TK_GREATER_EQUAL,
	TK_NOT,
	TK_NOT_EQUAL,
	TK_PLUS,
	TK_INC,
	TK_MINUS,
	TK_DEC,
	TK_STAR,
	TK_TIMES,
	TK_SLASH,
	TK_DIVIDE,
	TK_DEC_LIT,
	TK_FLOAT_SCI_LIT,
	TK_FLOAT_LIT,
	TK_HEX_LIT,
	TK_OCT_LIT,
	TK_BIN_LIT,
	TK_UNDERSCORE,
	TK_ZERO,
	//keywords:
	TK_PACKAGE,
	TK_FUNC,
	TK_MAIN,
	TK_RETURN,
	TK_IF,
	TK_ELSE,
	TK_FOR,
	TK_TRUE,
	TK_FALSE,
	TK_INT,
	TK_FLOAT64,
	TK_STRING,
	TK_BOOL
} token_type_t;


/*data type to represent tokens created by scanner*/
typedef struct Token
{
	token_type_t type;
	charseq_t value;
} *token_t;


#endif // IFJ_TOKENS_H
