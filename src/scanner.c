#include <stdio.h>
#include "tokens.h"


/*states of the scanner finite state machine*/
typedef enum
{
	S_START,
	S_PIPE,
	S_AMPERSAND,
	S_COLON,
	S_ASSIGN,
	S_LESS,
	S_GREATER,
	S_NOT,
	S_PLUS,
	S_MINUS,
	S_STAR,
	S_SLASH,
	S_ML_COMMENT1,
	S_ML_COMMENT2,
	S_SL_COMMENT,
	S_STR_LIT,
	S_ESCAPE_SEQ,
	S_HEX1,
	S_HEX2,
	S_UNDERSCORE,
	S_ZERO,
	S_DEC_LIT,
	S_FLOAT_SCI_LIT,
	S_FLOAT_LIT,
	S_FLOAT_EXP,
	S_FLOAT_POINT,
	S_HEX_LIT,
	S_OCT_LIT,
	S_BIN_LIT
} scanner_state_t;


void retrieve_token(token_t token) {
	scanner_state_t state = S_START;

	int c;
	while ((c = getc(stdin))) {
		switch (state) {
			case S_START:
				switch (c) {
					// end cases
					case '\n':
						token->type = TK_EOL;
						return;
					case ')':
						token->type = TK_R_PARANTHESIS;
						return;
					case '(':
						token->type = TK_L_PARANTHESIS;
						return;
					case EOF:
						token->type = TK_EOF;
						return;
					case ',':
						token->type = TK_SEPARATOR;
						return;
					case ';':
						token->type = TK_SEMICOLON;
						return;
					case '}':
						token->type = TK_R_CURLY;
						return;
					case '{':
						token->type = TK_L_CURLY;
						return;

					// intermediate cases
					case '|':
						state = S_PIPE;
						break;
					case '&':
						state = S_AMPERSAND;
						break;
					case ':':
						state = S_COLON;
						break;
					case '=':
						state = S_ASSIGN;
						break;
					case '<':
						state = S_LESS;
						break;
					case '>':
						state = S_GREATER;
						break;
					case '!':
						state = S_NOT;
						break;
					case '+':
						state = S_PLUS;
						break;
					case '-':
						state = S_MINUS;
						break;
					case '*':
						state = S_STAR;
						break;
					case '/':
						state = S_SLASH;
						break;

					default:
						break; // TODO: cover default path
				}
				break;

			case S_PIPE:
				if (c == '|') {
					token->type = TK_OR;
				} else {
					token->type = TK_ERROR;
				}
				return;

			case S_AMPERSAND:
				if (c == '&') {
					token->type = TK_AND;
				} else {
					token->type = TK_ERROR;
				}
				return;

			case S_COLON:
				if (c == '=') {
					token->type = TK_VAR_INNIT;
				} else {
					token->type = TK_ERROR;
				}
				return;

			case S_ASSIGN:
				if (c == '=') {
					token->type = TK_EQUAL;
				} else {
					token->type = TK_ASSIGN;
					ungetc(c, stdin);
				}
				return;

			case S_LESS:
				if (c == '=') {
					token->type = TK_LESS_EQUAL;
				} else {
					token->type = TK_LESS;
					ungetc(c, stdin);
				}
				return;

			case S_GREATER:
				if (c == '=') {
					token->type = TK_GREATER_EQUAL;
				} else {
					token->type = TK_GREATER;
					ungetc(c, stdin);
				}
				return;

			case S_NOT:
				if (c == '=') {
					token->type = TK_NOT_EQUAL;
				} else {
					token->type = TK_NOT;
					ungetc(c, stdin);
				}
				return;

			case S_PLUS:
				if (c == '=') {
					token->type = TK_INC;
				} else {
					token->type = TK_PLUS;
					ungetc(c, stdin);
				}
				return;

			case S_MINUS:
				if (c == '=') {
					token->type = TK_DEC;
				} else {
					token->type = TK_MINUS;
					ungetc(c, stdin);
				}
				return;

			case S_STAR:
				if (c == '=') {
					token->type = TK_TIMES;
				} else {
					token->type = TK_STAR;
					ungetc(c, stdin);
				}
				return;

			case S_SLASH:
				switch (c) {
					case '=':
						token->type = TK_DIVIDE;
						return;
					case '/':
						state = S_SL_COMMENT;
						break;
					case '*':
						state = S_ML_COMMENT1;
						break;
					default:
						token->type = TK_SLASH;
						ungetc(c, stdin);
						break;
				}
				break; // TODO: This was a return, yet the next state is set to something..

			case S_SL_COMMENT:
				switch (c) {
					case EOF:
						ungetc(c, stdin);
					case '\n': // don't want to send EOL token here since it belongs to comment
						state = S_START;
						break;
					default:
						break; // TODO: cover default path
				}

			case S_ML_COMMENT1:
				if (c == EOF) {
					token->type = TK_ERROR;
					return;
				} else if (c == '*') {
					state = S_ML_COMMENT2;
				}
				break; // NOTE: This wasn't here, but it probably shouldn't fall trough

			case S_ML_COMMENT2:
				if (c == EOF) {
					token->type = TK_ERROR;
					return;
				} else if (c == '/') {
					state = S_START;
					//not sending tokens for comment
				} else {
					state = S_ML_COMMENT1;
				}
				break; // NOTE: This wasn't here, but it probably shouldn't fall trough

			// TODO: Implement remaining enumerations
		}
	}
}
