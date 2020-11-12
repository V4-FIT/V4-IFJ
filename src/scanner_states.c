#include <stdio.h>
#include <ctype.h>

#include "scanner_states.h"
#include "hash_map.h"
#include "error.h"

// defined in scanner.c
FILE *get_stream(scanner_t scanner);
token_t get_tok(scanner_t scanner);
charseq_t get_charseq(scanner_t scanner);
hmap_t get_keyw_tok_map(scanner_t scanner);
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
		[S_DEC_LIT_UNDERSCORE] = &s_dec_lit_underscore,
		[S_FLOAT_SCI_LIT] = &s_float_sci_lit,
		[S_FLOAT_LIT] = &s_float_lit,
		[S_FLOAT_EXP] = &s_float_exp,
		[S_FLOAT_POINT] = &s_float_point,
		[S_HEX_LIT1] = &s_hex_lit1,
		[S_HEX_LIT2] = &s_hex_lit2,
		[S_HEX_LIT_UNDERSCORE] = &s_hex_lit_underscore,
		[S_OCT_LIT1] = &s_oct_lit1,
		[S_OCT_LIT2] = &s_oct_lit2,
		[S_OCT_LIT_UNDERSCORE] = &s_oct_lit_underscore,
		[S_BIN_LIT1] = &s_bin_lit1,
		[S_BIN_LIT2] = &s_bin_lit2,
		[S_BIN_LIT_UNDERSCORE] = &s_bin_lit_underscore,
		[S_IDENTIF] = &s_identif,
};

scanner_state_t s_start(scanner_t scanner, int c) {
	switch (c) {
		// whitespace skip
		case ' ':
		case '\r':
		case '\t':
			return S_START;

			// end cases
		case '(':
			get_tok(scanner)->type = TK_L_PARENTHESIS;
			return S_END;
		case ')':
			get_tok(scanner)->type = TK_R_PARENTHESIS;
			return S_END;
		case '{':
			get_tok(scanner)->type = TK_L_CURLY;
			return S_END;
		case '}':
			get_tok(scanner)->type = TK_R_CURLY;
			return S_END;
		case ',':
			get_tok(scanner)->type = TK_COMMA;
			return S_END;
		case ';':
			get_tok(scanner)->type = TK_SEMICOLON;
			return S_END;
		case '\n':
			get_tok(scanner)->type = TK_EOL;
			return S_END;
		case EOF:
			get_tok(scanner)->type = TK_EOF;
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
				get_tok(scanner)->type = TK_ERROR;
				get_tok(scanner)->param.i = ERROR_MISC;
				return S_END;
			}

		default:
			if (isalpha(c)) {
				if (charseq_push_back(get_charseq(scanner), c)) {
					return S_IDENTIF;
				} else {
					get_tok(scanner)->type = TK_ERROR;
					get_tok(scanner)->param.i = ERROR_MISC;
					return S_END;
				}
			}
			if (isdigit(c)) { //special case for 0 handled above
				if (charseq_push_back(get_charseq(scanner), c)) {
					return S_DEC_LIT;
				} else {
					get_tok(scanner)->type = TK_ERROR;
					get_tok(scanner)->param.i = ERROR_MISC;
					return S_END;
				}
			}
			get_tok(scanner)->type = TK_ERROR;
			get_tok(scanner)->param.i = ERROR_LEX;
			return S_END;
	}
}

scanner_state_t s_pipe(scanner_t scanner, int c) {
	if (c == '|') {
		get_tok(scanner)->type = TK_OR;
	} else {
		get_tok(scanner)->type = TK_ERROR;
		get_tok(scanner)->param.i = ERROR_LEX;
	}
	return S_END;
}

scanner_state_t s_ampersand(scanner_t scanner, int c) {
	if (c == '&') {
		get_tok(scanner)->type = TK_AND;
	} else {
		get_tok(scanner)->type = TK_ERROR;
		get_tok(scanner)->param.i = ERROR_LEX;
	}
	return S_END;
}

scanner_state_t s_colon(scanner_t scanner, int c) {
	if (c == '=') {
		get_tok(scanner)->type = TK_VAR_INIT;
	} else {
		get_tok(scanner)->type = TK_ERROR;
		get_tok(scanner)->param.i = ERROR_LEX;
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
		get_tok(scanner)->type = TK_PLUS_ASSIGN;
	} else {
		get_tok(scanner)->type = TK_PLUS;
		ungetc(c, get_stream(scanner));
	}
	return S_END;
}

scanner_state_t s_minus(scanner_t scanner, int c) {
	if (c == '=') {
		get_tok(scanner)->type = TK_MINUS_ASSIGN;
	} else {
		get_tok(scanner)->type = TK_MINUS;
		ungetc(c, get_stream(scanner));
	}
	return S_END;
}

scanner_state_t s_star(scanner_t scanner, int c) {
	if (c == '=') {
		get_tok(scanner)->type = TK_MULTIPLY_ASSIGN;
	} else {
		get_tok(scanner)->type = TK_MULTIPLY;
		ungetc(c, get_stream(scanner));
	}
	return S_END;
}

scanner_state_t s_slash(scanner_t scanner, int c) {
	switch (c) {
		case '=':
			get_tok(scanner)->type = TK_DIVIDE_ASSIGN;
			return S_END;
		case '/':
			return S_SL_COMMENT;
		case '*':
			return S_ML_COMMENT1;
		default:
			get_tok(scanner)->type = TK_DIVIDE;
			ungetc(c, get_stream(scanner));
			return S_END;
	}
}

scanner_state_t s_sl_comment(scanner_t scanner, int c) {
	switch (c) {
		case EOF:
			ungetc(c, get_stream(scanner));
		case '\n':
			return S_START;
		default:
			return S_SL_COMMENT;
	}
}

scanner_state_t s_ml_comment1(scanner_t scanner, int c) {
	if (c == EOF) {
		get_tok(scanner)->type = TK_ERROR;
		get_tok(scanner)->param.i = ERROR_LEX;
		return S_END;
	} else if (c == '*') {
		return S_ML_COMMENT2;
	}
	return S_ML_COMMENT1;
}

scanner_state_t s_ml_comment2(scanner_t scanner, int c) {
	if (c == EOF) {
		get_tok(scanner)->type = TK_ERROR;
		get_tok(scanner)->param.i = ERROR_LEX;
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
			get_tok(scanner)->param.s = charseq_data(get_charseq(scanner));
			return S_END;
		default:
			if (c < ' ') { //includes all unprintables, EOL and EOF
				get_tok(scanner)->type = TK_ERROR;
				get_tok(scanner)->param.i = ERROR_LEX;
				return S_END;
			} else if (charseq_push_back(get_charseq(scanner), c)) {
				return S_STR_LIT;
			} else {
				get_tok(scanner)->type = TK_ERROR;
				get_tok(scanner)->param.i = ERROR_MISC;
				return S_END;
			}
	}
}

scanner_state_t s_escape_seq(scanner_t scanner, int c) {
	switch (c) {
		case 'n':
			if (charseq_push_back(get_charseq(scanner), '\n')) {
				return S_STR_LIT;
			} else {
				get_tok(scanner)->type = TK_ERROR;
				get_tok(scanner)->param.i = ERROR_MISC;
				return S_END;
			}
		case 't':
			if (charseq_push_back(get_charseq(scanner), '\t')) {
				return S_STR_LIT;
			} else {
				get_tok(scanner)->type = TK_ERROR;
				get_tok(scanner)->param.i = ERROR_MISC;
				return S_END;
			}
		case '\\':
		case '\"':
			if (charseq_push_back(get_charseq(scanner), c)) {
				return S_STR_LIT;
			} else {
				get_tok(scanner)->type = TK_ERROR;
				get_tok(scanner)->param.i = ERROR_MISC;
				return S_END;
			}
		case 'x':
			return S_HEX1;
		default:
			get_tok(scanner)->type = TK_ERROR;
			get_tok(scanner)->param.i = ERROR_LEX;
			return S_END;
	}
}

scanner_state_t s_hex1(scanner_t scanner, int c) {
	if (isxdigit(c)) {
		get_buf_escape(scanner)[0] = (char)c;
		return S_HEX2;
	} else {
		get_tok(scanner)->type = TK_ERROR;
		get_tok(scanner)->param.i = ERROR_LEX;
		return S_END;
	}
}

scanner_state_t s_hex2(scanner_t scanner, int c) {
	if (isxdigit(c)) {
		get_buf_escape(scanner)[1] = (char)c;
	} else {
		get_tok(scanner)->type = TK_ERROR;
		get_tok(scanner)->param.i = ERROR_LEX;
		return S_END;
	}

	char subst = (char)strtoul(get_buf_escape(scanner), NULL, 16);
	if (!charseq_push_back(get_charseq(scanner), subst)) {
		get_tok(scanner)->type = TK_ERROR;
		get_tok(scanner)->param.i = ERROR_MISC;
		return S_END;
	}

	return S_STR_LIT;
}

scanner_state_t s_underscore(scanner_t scanner, int c) {
	if (isalnum(c) || (c == '_')) {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_IDENTIF;
		} else {
			get_tok(scanner)->type = TK_ERROR;
			get_tok(scanner)->param.i = ERROR_MISC;
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
				get_tok(scanner)->type = TK_ERROR;
				get_tok(scanner)->param.i = ERROR_MISC;
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
			return S_HEX_LIT1;
		case 'e':
		case 'E':
			// prepend a zero to be a valid float 0e123
			if (!charseq_push_back(get_charseq(scanner), '0')) {
				get_tok(scanner)->type = TK_ERROR;
				get_tok(scanner)->param.i = ERROR_MISC;
				return S_END;
			}
			if (charseq_push_back(get_charseq(scanner), c)) {
				return S_FLOAT_EXP;
			} else {
				get_tok(scanner)->type = TK_ERROR;
				get_tok(scanner)->param.i = ERROR_MISC;
				return S_END;
			}
		case '_':
			// there are no valid tokens starting '0_'
			get_tok(scanner)->type = TK_ERROR;
			get_tok(scanner)->param.i = ERROR_LEX;
			return S_END;
		default:
			if (isdigit(c)) {
				get_tok(scanner)->type = TK_ERROR;
				get_tok(scanner)->param.i = ERROR_LEX;
				return S_END;
			} else {
				ungetc(c, get_stream(scanner));
				get_tok(scanner)->type = TK_INT_LIT;
				get_tok(scanner)->param.i = 0u;
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
				get_tok(scanner)->type = TK_ERROR;
				get_tok(scanner)->param.i = ERROR_MISC;
				return S_END;
			}
		case 'e':
		case 'E':
			if (charseq_push_back(get_charseq(scanner), c)) {
				return S_FLOAT_EXP;
			} else {
				get_tok(scanner)->type = TK_ERROR;
				get_tok(scanner)->param.i = ERROR_MISC;
				return S_END;
			}
		case '_':
			return S_DEC_LIT_UNDERSCORE;
		default:
			if (isdigit(c)) {
				if (!charseq_push_back(get_charseq(scanner), c)) {
					get_tok(scanner)->type = TK_ERROR;
					get_tok(scanner)->param.i = ERROR_MISC;
					return S_END;
				}
				return S_DEC_LIT;
			} else {
				ungetc(c, get_stream(scanner));
				get_tok(scanner)->type = TK_INT_LIT;
				char *endptr;
				get_tok(scanner)->param.i = (uint64_t)strtoull(charseq_data(get_charseq(scanner)), &endptr, 10);
				if (*endptr != '\0') {
					get_tok(scanner)->type = TK_ERROR;
					get_tok(scanner)->param.i = ERROR_MISC;
				}
				return S_END;
			}
	}
}

scanner_state_t s_dec_lit_underscore(scanner_t scanner, int c) {
	if (isdigit(c)) {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_DEC_LIT;
		} else {
			get_tok(scanner)->type = TK_ERROR;
			get_tok(scanner)->param.i = ERROR_MISC;
			return S_END;
		}
	} else {
		get_tok(scanner)->type = TK_ERROR;
		get_tok(scanner)->param.i = ERROR_LEX;
		return S_END;
	}
}

scanner_state_t s_float_sci_lit(scanner_t scanner, int c) {
	if (isdigit(c)) {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_FLOAT_SCI_LIT;
		} else {
			get_tok(scanner)->type = TK_ERROR;
			get_tok(scanner)->param.i = ERROR_MISC;
			return S_END;
		}
	} else {
		ungetc(c, get_stream(scanner));
		get_tok(scanner)->type = TK_FLOAT_LIT;
		char *endptr;
		get_tok(scanner)->param.f = strtod(charseq_data(get_charseq(scanner)), &endptr);
		if (*endptr != '\0') {
			get_tok(scanner)->type = TK_ERROR;
			get_tok(scanner)->param.i = ERROR_MISC;
		}
		return S_END;
	}
}

scanner_state_t s_float_lit(scanner_t scanner, int c) {
	switch (c) {
		case 'e':
		case 'E':
			if (charseq_push_back(get_charseq(scanner), c)) {
				return S_FLOAT_EXP;
			} else {
				get_tok(scanner)->type = TK_ERROR;
				get_tok(scanner)->param.i = ERROR_MISC;
				return S_END;
			}
		default:
			if (isdigit(c)) {
				if (charseq_push_back(get_charseq(scanner), c)) {
					return S_FLOAT_LIT;
				} else {
					get_tok(scanner)->type = TK_ERROR;
					get_tok(scanner)->param.i = ERROR_MISC;
					return S_END;
				}
			} else {
				ungetc(c, get_stream(scanner));
				get_tok(scanner)->type = TK_FLOAT_LIT;
				char *endptr;
				get_tok(scanner)->param.f = strtod(charseq_data(get_charseq(scanner)), &endptr);
				if (*endptr != '\0') {
					get_tok(scanner)->type = TK_ERROR;
					get_tok(scanner)->param.i = ERROR_MISC;
				}
				return S_END;
			}
	}
}

scanner_state_t s_float_exp(scanner_t scanner, int c) {
	if (isdigit(c) || c == '-' || c == '+') {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_FLOAT_SCI_LIT;
		} else {
			get_tok(scanner)->type = TK_ERROR;
			get_tok(scanner)->param.i = ERROR_MISC;
			return S_END;
		}
	} else {
		get_tok(scanner)->type = TK_ERROR;
		get_tok(scanner)->param.i = ERROR_LEX;
		return S_END;
	}
}

scanner_state_t s_float_point(scanner_t scanner, int c) {
	if (isdigit(c)) {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_FLOAT_LIT;
		} else {
			get_tok(scanner)->type = TK_ERROR;
			get_tok(scanner)->param.i = ERROR_MISC;
			return S_END;
		}
	} else {
		get_tok(scanner)->type = TK_ERROR;
		get_tok(scanner)->param.i = ERROR_LEX;
		return S_END;
	}
}

scanner_state_t s_hex_lit1(scanner_t scanner, int c) {
	if (isxdigit(c)) {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_HEX_LIT2;
		} else {
			get_tok(scanner)->type = TK_ERROR;
			get_tok(scanner)->param.i = ERROR_MISC;
			return S_END;
		}
	} else {
		get_tok(scanner)->type = TK_ERROR;
		get_tok(scanner)->param.i = ERROR_LEX;
		return S_END;
	}
}

scanner_state_t s_hex_lit2(scanner_t scanner, int c) {
	if (isxdigit(c)) {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_HEX_LIT2;
		} else {
			get_tok(scanner)->type = TK_ERROR;
			get_tok(scanner)->param.i = ERROR_MISC;
			return S_END;
		}
	} else if (c == '_'){
		return S_HEX_LIT_UNDERSCORE;
	} else {
		ungetc(c, get_stream(scanner));
		get_tok(scanner)->type = TK_INT_LIT;
		char *endptr;
		get_tok(scanner)->param.i = (uint64_t)strtoull(charseq_data(get_charseq(scanner)), &endptr, 16);
		if (*endptr != '\0') {
			get_tok(scanner)->type = TK_ERROR;
			get_tok(scanner)->param.i = ERROR_MISC;
		}
		return S_END;
	}
}

scanner_state_t s_hex_lit_underscore(scanner_t scanner, int c) {
	if (isxdigit(c)) {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_HEX_LIT2;
		} else {
			get_tok(scanner)->type = TK_ERROR;
			get_tok(scanner)->param.i = ERROR_MISC;
			return S_END;
		}
	} else {
		get_tok(scanner)->type = TK_ERROR;
		get_tok(scanner)->param.i = ERROR_LEX;
		return S_END;
	}
}

scanner_state_t s_oct_lit1(scanner_t scanner, int c) {
	if (c >= '0' && c <= '7') {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_OCT_LIT2;
		} else {
			get_tok(scanner)->type = TK_ERROR;
			get_tok(scanner)->param.i = ERROR_MISC;
			return S_END;
		}
	} else {
		get_tok(scanner)->type = TK_ERROR;
		get_tok(scanner)->param.i = ERROR_LEX;
		return S_END;
	}
}

scanner_state_t s_oct_lit2(scanner_t scanner, int c) {
	if (c >= '0' && c <= '7') {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_OCT_LIT2;
		} else {
			get_tok(scanner)->type = TK_ERROR;
			get_tok(scanner)->param.i = ERROR_MISC;
			return S_END;
		}
	} else if (c == '_') {
		return S_OCT_LIT_UNDERSCORE;
	} else {
		ungetc(c, get_stream(scanner));
		get_tok(scanner)->type = TK_INT_LIT;
		char *endptr;
		get_tok(scanner)->param.i = (uint64_t)strtoull(charseq_data(get_charseq(scanner)), &endptr, 8);
		if (*endptr != '\0') {
			get_tok(scanner)->type = TK_ERROR;
			get_tok(scanner)->param.i = ERROR_MISC;
		}
		return S_END;
	}
}

scanner_state_t s_oct_lit_underscore(scanner_t scanner, int c) {
	if (c >= '0' && c <= '7') {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_OCT_LIT2;
		} else {
			get_tok(scanner)->type = TK_ERROR;
			get_tok(scanner)->param.i = ERROR_MISC;
			return S_END;
		}
	} else {
		get_tok(scanner)->type = TK_ERROR;
		get_tok(scanner)->param.i = ERROR_LEX;
		return S_END;
	}
}

scanner_state_t s_bin_lit1(scanner_t scanner, int c) {
	if (c == '0' || c == '1') {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_BIN_LIT2;
		} else {
			get_tok(scanner)->type = TK_ERROR;
			get_tok(scanner)->param.i = ERROR_MISC;
			return S_END;
		}
	} else {
		get_tok(scanner)->type = TK_ERROR;
		get_tok(scanner)->param.i = ERROR_LEX;
		return S_END;
	}
}

scanner_state_t s_bin_lit2(scanner_t scanner, int c) {
	if (c == '0' || c == '1') {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_BIN_LIT2;
		} else {
			get_tok(scanner)->type = TK_ERROR;
			get_tok(scanner)->param.i = ERROR_MISC;
			return S_END;
		}
	} else if (c == '_') {
		return S_BIN_LIT_UNDERSCORE;
	} else {
		ungetc(c, get_stream(scanner));
		get_tok(scanner)->type = TK_INT_LIT;
		char *endptr;
		get_tok(scanner)->param.i = (uint64_t)strtoull(charseq_data(get_charseq(scanner)), &endptr, 2);
		if (*endptr != '\0') {
			get_tok(scanner)->type = TK_ERROR;
			get_tok(scanner)->param.i = ERROR_MISC;
		}
		return S_END;
	}
}

scanner_state_t s_bin_lit_underscore(scanner_t scanner, int c) {
	if (c == '0' || c == '1') {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_BIN_LIT2;
		} else {
			get_tok(scanner)->type = TK_ERROR;
			get_tok(scanner)->param.i = ERROR_MISC;
			return S_END;
		}
	} else {
		get_tok(scanner)->type = TK_ERROR;
		get_tok(scanner)->param.i = ERROR_LEX;
		return S_END;
	}
}

scanner_state_t s_identif(scanner_t scanner, int c) {
	if (isalnum(c) || (c == '_')) {
		if (charseq_push_back(get_charseq(scanner), c)) {
			return S_IDENTIF;
		} else {
			get_tok(scanner)->type = TK_ERROR;
			get_tok(scanner)->param.i = ERROR_MISC;
			return S_END;
		}
	} else {
		ungetc(c, get_stream(scanner));
		hmap_iterator_t it = hmap_find(get_keyw_tok_map(scanner), charseq_data(get_charseq(scanner)));
		if (hmap_it_eq(it, hmap_end(get_keyw_tok_map(scanner)))) {
			get_tok(scanner)->type = TK_IDENTIFIER;
			get_tok(scanner)->param.s = charseq_data(get_charseq(scanner));
		} else {
			get_tok(scanner)->type = *(token_type_t *)hmap_get_value(it);
		}
		return S_END;
	}

}
