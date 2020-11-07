#include <stdio.h>
#include <ctype.h>

#include "scanner_states.h"
#include "char_sequence.h"

// TODO: Underscores for numbers

// defined in scanner.c
FILE *get_stream(scanner_t scanner);
token_t get_tok(scanner_t scanner);
charseq_t get_charseq(scanner_t scanner);
char *get_buf_escape(scanner_t scanner);

state_fun_ptr_t state_map[] = {
		[S_START] = &s_start,
		[S_PIPE] = &s_pipe,
		[S_AMPERSAND] = &s_ampersand,
		[S_COLON] = &s_colon,
		[S_ASSIGN] = &s_assign,
		[S_LESS] = &s_less,
		[S_GREATER] = &s_greater,
		[S_NOT] = &s_not,
		[S_PLUS] = &s_plus,
		[S_MINUS] = &s_minus,
		[S_STAR] = &s_star,
		[S_SLASH] = &s_slash,
		[S_SL_COMMENT] = &s_sl_comment,
		[S_ML_COMMENT1] = &s_ml_comment1,
		[S_ML_COMMENT2] = &s_ml_comment2,
		[S_STR_LIT] = &s_str_lit,
		[S_ESCAPE_SEQ] = &s_escape_seq,
		[S_HEX1] = &s_hex1,
		[S_HEX2] = &s_hex2,
		[S_UNDERSCORE] = &s_underscore,
		[S_ZERO] = &s_zero,
		[S_DEC_LIT] = &s_dec_lit,
		[S_FLOAT_SCI_LIT] = &s_float_sci_lit,
		[S_FLOAT_LIT] = &s_float_lit,
		[S_FLOAT_EXP1] = &s_float_exp1,
		[S_FLOAT_EXP2] = &s_float_exp2,
		[S_FLOAT_POINT] = &s_float_point,
		[S_HEX_LIT1] = &s_hex_lit1,
		[S_HEX_LIT2] = &s_hex_lit2,
		[S_OCT_LIT1] = &s_oct_lit1,
		[S_OCT_LIT2] = &s_oct_lit2,
		[S_BIN_LIT1] = &s_bin_lit1,
		[S_BIN_LIT2] = &s_bin_lit2,
		[S_IDENTIF] = &s_identif
};

scanner_state_t s_start(scanner_t scanner, int c) {
	switch (c) {
		// whitespace skip
		case ' ':
		case '\t':
			return S_START;

		// end cases
		case '\n':
			get_tok(scanner)->type = TK_EOL;
			return S_END;
		case ')':
			get_tok(scanner)->type = TK_R_PARANTHESIS;
			return S_END;
		case '(':
			get_tok(scanner)->type = TK_L_PARANTHESIS;
			return S_END;
		case EOF:
			get_tok(scanner)->type = TK_EOF;
			return S_END;
		case ',':
			get_tok(scanner)->type = TK_SEPARATOR;
			return S_END;
		case ';':
			get_tok(scanner)->type = TK_SEMICOLON;
			return S_END;
		case '}':
			get_tok(scanner)->type = TK_R_CURLY;
			return S_END;
		case '{':
			get_tok(scanner)->type = TK_L_CURLY;
			return S_END;

		// intermediate cases
		case '|':
			return S_PIPE;
		case '&':
			return S_AMPERSAND;
		case ':':
			return S_COLON;
		case '=':
			return S_ASSIGN;
		case '<':
			return S_LESS;
		case '>':
			return S_GREATER;
		case '!':
			return S_NOT;
		case '+':
			return S_PLUS;
		case '-':
			return S_MINUS;
		case '*':
			return S_STAR;
		case '/':
			return S_SLASH;
		case '\"':
			return S_STR_LIT;
		case '0':
			return S_ZERO;

		case '_':
			if (charseq_push_back(get_charseq(scanner), c)) {
				return S_UNDERSCORE;
			} else {
				get_tok(scanner)->type = TK_INTERNAL_ERROR;
				return S_END;
			}

		default:
			if (isalpha(c)) {
				return S_IDENTIF;
			}
			if (isdigit(c)) { //special case for 0 handled above
				if (!charseq_push_back(get_charseq(scanner), c)) {
					get_tok(scanner)->type = TK_INTERNAL_ERROR;
					return S_END;
				}
				return S_DEC_LIT;
			}
			get_tok(scanner)->type = TK_ERROR;
			return S_END;
	}
}

scanner_state_t s_pipe(scanner_t scanner, int c) {
	if (c == '|') {
		get_tok(scanner)->type = TK_OR;
	} else {
		get_tok(scanner)->type = TK_ERROR;
	}
	return S_END;
}

scanner_state_t s_ampersand(scanner_t scanner, int c) {
	if (c == '&') {
		get_tok(scanner)->type = TK_AND;
	} else {
		get_tok(scanner)->type = TK_ERROR;
	}
	return S_END;
}

scanner_state_t s_colon(scanner_t scanner, int c) {
	if (c == '=') {
		get_tok(scanner)->type = TK_VAR_INNIT;
	} else {
		get_tok(scanner)->type = TK_ERROR;
	}
	return S_END;
}

scanner_state_t s_assign(scanner_t scanner, int c) {
	if (c == '=') {
		get_tok(scanner)->type = TK_EQUAL;
	} else {
		get_tok(scanner)->type = TK_ASSIGN;
		ungetc(c, get_stream(scanner));
	}
	return S_END;
}

scanner_state_t s_less(scanner_t scanner, int c) {
	if (c == '=') {
		get_tok(scanner)->type = TK_LESS_EQUAL;
	} else {
		get_tok(scanner)->type = TK_LESS;
		ungetc(c, get_stream(scanner));
	}
	return S_END;
}

scanner_state_t s_greater(scanner_t scanner, int c) {
	if (c == '=') {
		get_tok(scanner)->type = TK_GREATER_EQUAL;
	} else {
		get_tok(scanner)->type = TK_GREATER;
		ungetc(c, get_stream(scanner));
	}
	return S_END;
}

scanner_state_t s_not(scanner_t scanner, int c) {
	if (c == '=') {
		get_tok(scanner)->type = TK_NOT_EQUAL;
	} else {
		get_tok(scanner)->type = TK_NOT;
		ungetc(c, get_stream(scanner));
	}
	return S_END;
}

scanner_state_t s_plus(scanner_t scanner, int c) {
	if (c == '=') {
		get_tok(scanner)->type = TK_INCREMENT;
	} else {
		get_tok(scanner)->type = TK_PLUS;
		ungetc(c, get_stream(scanner));
	}
	return S_END;
}

scanner_state_t s_minus(scanner_t scanner, int c) {
	if (c == '=') {
		get_tok(scanner)->type = TK_DECREMENT;
	} else {
		get_tok(scanner)->type = TK_MINUS;
		ungetc(c, get_stream(scanner));
	}
	return S_END;
}

scanner_state_t s_star(scanner_t scanner, int c) {
	if (c == '=') {
		get_tok(scanner)->type = TK_TIMES;
	} else {
		get_tok(scanner)->type = TK_STAR;
		ungetc(c, get_stream(scanner));
	}
	return S_END;
}

scanner_state_t s_slash(scanner_t scanner, int c) {
	switch (c) {
		case '=':
			get_tok(scanner)->type = TK_DIVIDE;
			return S_END;
		case '/':
			return S_SL_COMMENT;
		case '*':
			return S_ML_COMMENT1;
		default:
			get_tok(scanner)->type = TK_SLASH;
			ungetc(c, get_stream(scanner));
			return S_END;
	}
}

scanner_state_t s_sl_comment(scanner_t scanner, int c) {
	switch (c) {
		case EOF:
		case '\n':
			ungetc(c, get_stream(scanner));
			return S_START;
		default:
			return S_SL_COMMENT;
	}
}

scanner_state_t s_ml_comment1(scanner_t scanner, int c) {
	if (c == EOF) {
		get_tok(scanner)->type = TK_ERROR;
		return S_END;
	} else if (c == '*') {
		return S_ML_COMMENT2;
	}
	return S_ML_COMMENT1;
}

scanner_state_t s_ml_comment2(scanner_t scanner, int c) {
	if (c == EOF) {
		get_tok(scanner)->type = TK_ERROR;
		return S_END;
	} else if (c == '*') {
		return S_ML_COMMENT2;
	} else if (c == '/') {
		// do not send token for comment
		return S_START;
	} else {
		return S_ML_COMMENT1;
	}
}

scanner_state_t s_str_lit(scanner_t scanner, int c) {
	switch (c) {
		case '\\':
			return S_ESCAPE_SEQ;
		case '\"':
			get_tok(scanner)->type = TK_STR_LIT;
			get_tok(scanner)->param.c = charseq_data(get_charseq(scanner));
			return S_END;
		default:
			if (c < ' ') { //includes all unprintables, EOL and EOF
				get_tok(scanner)->type = TK_ERROR;
				return S_END;
			} else if (charseq_push_back(get_charseq(scanner), c)) {
				return S_STR_LIT;
			} else {
				get_tok(scanner)->type = TK_INTERNAL_ERROR;
				return S_END;
			}
	}
}

scanner_state_t s_escape_seq(scanner_t scanner, int c) {
	switch (c) {
		case 'n':
		case 't':
		case '\\':
		case '\"':
			// add backslash to make these escape sequences valid
			if (!charseq_push_back(get_charseq(scanner), '\\')) {
				get_tok(scanner)->type = TK_INTERNAL_ERROR;
				return S_END;
			}
			if (charseq_push_back(get_charseq(scanner), c)) {
				return S_STR_LIT;
			} else {
				get_tok(scanner)->type = TK_INTERNAL_ERROR;
				return S_END;
			}
		case 'x':
			return S_HEX1;
		default:
			get_tok(scanner)->type = TK_ERROR;
			return S_END;
	}
}

scanner_state_t s_hex1(scanner_t scanner, int c) {
	if (isxdigit(c)) {
		get_buf_escape(scanner)[0] = (char)c;
		return S_HEX2;
	} else {
		get_tok(scanner)->type = TK_ERROR;
		return S_END;
	}
}

scanner_state_t s_hex2(scanner_t scanner, int c) {
	if (isxdigit(c)) {
		get_buf_escape(scanner)[1] = (char)c;
	} else {
		get_tok(scanner)->type = TK_ERROR;
		return S_END;
	}

	char subst = (char)strtol(get_buf_escape(scanner), NULL, 16);
	if (!charseq_push_back(get_charseq(scanner), subst)) {
		get_tok(scanner)->type = TK_INTERNAL_ERROR;
		return S_END;
	}

	return S_STR_LIT;
}

scanner_state_t s_underscore(scanner_t scanner, int c) {
	if (isalnum(c) || (c == '_')) {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_IDENTIF;
		} else {
			get_tok(scanner)->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else {
		ungetc(c, get_stream(scanner));
		get_tok(scanner)->type = TK_UNDERSCORE;
		return S_END;
	}
}

scanner_state_t s_zero(scanner_t scanner, int c) {
	switch (c) {
		case '.':
			if (charseq_push_back(get_charseq(scanner), c)) {
				return S_FLOAT_POINT;
			} else {
				get_tok(scanner)->type = TK_INTERNAL_ERROR;
				return S_END;
			}
		case 'b':
		case 'B':
			return S_BIN_LIT1;
		case 'o':
		case 'O':
			return S_OCT_LIT1;
		case 'x':
		case 'X':
			return S_HEX_LIT2;
		case 'e':
		case 'E':
			// prepend a zero to be a valid float 0e123
			if (!charseq_push_back(get_charseq(scanner), '0')) {
				get_tok(scanner)->type = TK_INTERNAL_ERROR;
				return S_END;
			}
			if (charseq_push_back(get_charseq(scanner), c)) {
				return S_FLOAT_EXP1;
			} else {
				get_tok(scanner)->type = TK_INTERNAL_ERROR;
				return S_END;
			}
		default:
			if (isdigit(c)) {
				get_tok(scanner)->type = TK_ERROR;
				return S_END;
			} else {
				ungetc(c, get_stream(scanner));
				get_tok(scanner)->type = TK_INT_LIT;
				get_tok(scanner)->param.i = 0;
				return S_END;
			}
	}
}

scanner_state_t s_dec_lit(scanner_t scanner, int c) {
	switch (c) {
		case '.':
			if (charseq_push_back(get_charseq(scanner), c)) {
				return S_FLOAT_POINT;
			} else {
				get_tok(scanner)->type = TK_INTERNAL_ERROR;
				return S_END;
			}
		case 'e':
		case 'E':
			if (charseq_push_back(get_charseq(scanner), c)) {
				return S_FLOAT_EXP1;
			} else {
				get_tok(scanner)->type = TK_INTERNAL_ERROR;
				return S_END;
			}
		default:
			if (isdigit(c)) {
				if (!charseq_push_back(get_charseq(scanner), c)) {
					get_tok(scanner)->type = TK_INTERNAL_ERROR;
					return S_END;
				}
				return S_DEC_LIT;
			} else {
				ungetc(c, get_stream(scanner));
				get_tok(scanner)->type = TK_INT_LIT;
				char *endptr;
				get_tok(scanner)->param.i = (int64_t)strtoll(charseq_data(get_charseq(scanner)), &endptr, 10);
				if (*endptr != '\0') {
					get_tok(scanner)->type = TK_INTERNAL_ERROR;
				}
				return S_END;
			}
	}
}

scanner_state_t s_float_sci_lit(scanner_t scanner, int c) {
	if (isdigit(c)) {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_FLOAT_SCI_LIT;
		} else {
			get_tok(scanner)->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else {
		ungetc(c, get_stream(scanner));
		get_tok(scanner)->type = TK_FLOAT_LIT;
		char *endptr;
		get_tok(scanner)->param.f = strtod(charseq_data(get_charseq(scanner)), &endptr);
		if (*endptr != '\0') {
			get_tok(scanner)->type = TK_INTERNAL_ERROR;
		}
		return S_END;
	}
}

scanner_state_t s_float_lit(scanner_t scanner, int c) {
	switch (c) {
		case 'e':
		case 'E':
			if (charseq_push_back(get_charseq(scanner), c)) {
				return S_FLOAT_EXP1;
			} else {
				get_tok(scanner)->type = TK_INTERNAL_ERROR;
				return S_END;
			}
		default:
			if (isdigit(c)) {
				if (charseq_push_back(get_charseq(scanner), c)) {
					return S_FLOAT_LIT;
				} else {
					get_tok(scanner)->type = TK_INTERNAL_ERROR;
					return S_END;
				}
			} else {
				ungetc(c, get_stream(scanner));
				get_tok(scanner)->type = TK_FLOAT_LIT;
				char *endptr;
				get_tok(scanner)->param.f = strtod(charseq_data(get_charseq(scanner)), &endptr);
				if (*endptr != '\0') {
					get_tok(scanner)->type = TK_INTERNAL_ERROR;
				}
				return S_END;
			}
	}
}

scanner_state_t s_float_exp1(scanner_t scanner, int c) {
	if (isdigit(c)) {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_FLOAT_SCI_LIT;
		} else {
			get_tok(scanner)->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else if (c == '-' || c == '+') {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_FLOAT_EXP2;
		} else {
			get_tok(scanner)->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else {
		get_tok(scanner)->type = TK_ERROR;
		return S_END;
	}
}

scanner_state_t s_float_exp2(scanner_t scanner, int c) {
	if (isdigit(c)) {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_FLOAT_SCI_LIT;
		} else {
			get_tok(scanner)->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else {
		get_tok(scanner)->type = TK_ERROR;
		return S_END;
	}
}

scanner_state_t s_float_point(scanner_t scanner, int c) {
	if (isdigit(c)) {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_FLOAT_LIT;
		} else {
			get_tok(scanner)->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else {
		get_tok(scanner)->type = TK_ERROR;
		return S_END;
	}
}

scanner_state_t s_hex_lit1(scanner_t scanner, int c) {
	if (isxdigit(c)) {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_HEX_LIT2;
		} else {
			get_tok(scanner)->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else {
		get_tok(scanner)->type = TK_ERROR;
		return S_END;
	}
}

scanner_state_t s_hex_lit2(scanner_t scanner, int c) { //TODO: refactor when tested
	if (isxdigit(c)) {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_HEX_LIT2;
		} else {
			get_tok(scanner)->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else {
		ungetc(c, get_stream(scanner));
		get_tok(scanner)->type = TK_INT_LIT;
		char *endptr;
		get_tok(scanner)->param.i = (int64_t)strtoll(charseq_data(get_charseq(scanner)), &endptr, 16);
		if (*endptr != '\0') {
			get_tok(scanner)->type = TK_INTERNAL_ERROR;
		}
		return S_END;
	}
}

scanner_state_t s_oct_lit1(scanner_t scanner, int c) {
	if (c >= '0' && c <= '7') {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_OCT_LIT2;
		} else {
			get_tok(scanner)->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else {
		get_tok(scanner)->type = TK_ERROR;
		return S_END;
	}
}

scanner_state_t s_oct_lit2(scanner_t scanner, int c) { //TODO: refactor when tested
	if (c >= '0' && c <= '7') {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_OCT_LIT2;
		} else {
			get_tok(scanner)->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else {
		ungetc(c, get_stream(scanner));
		get_tok(scanner)->type = TK_INT_LIT;
		char *endptr;
		get_tok(scanner)->param.i = (int64_t)strtoll(charseq_data(get_charseq(scanner)), &endptr, 8);
		if (*endptr != '\0') {
			get_tok(scanner)->type = TK_INTERNAL_ERROR;
		}
		return S_END;
	}
}

scanner_state_t s_bin_lit1(scanner_t scanner, int c) {
	if (c == '0' || c == '1') {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_BIN_LIT2;
		} else {
			get_tok(scanner)->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else {
		get_tok(scanner)->type = TK_ERROR;
		return S_END;
	}
}

scanner_state_t s_bin_lit2(scanner_t scanner, int c) { //TODO: refactor when tested
	if (c == '0' || c == '1') {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_BIN_LIT2;
		} else {
			get_tok(scanner)->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else {
		ungetc(c, get_stream(scanner));
		get_tok(scanner)->type = TK_INT_LIT;
		char *endptr;
		get_tok(scanner)->param.i = (int64_t)strtoll(charseq_data(get_charseq(scanner)), &endptr, 2);
		if (*endptr != '\0') {
			get_tok(scanner)->type = TK_INTERNAL_ERROR;
		}
		return S_END;
	}
}

scanner_state_t s_identif(scanner_t scanner, int c) {
	if (isalnum(c) || (c == '_')) {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_IDENTIF;
		} else {
			get_tok(scanner)->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else {
		ungetc(c, get_stream(scanner));
		// TODO
		// get_tok(scanner)->param->somenewtypeprobably = hash table magic
		return S_END;
	}

}
