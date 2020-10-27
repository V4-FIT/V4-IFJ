#include <stdio.h>

#include "scanner_states.h"

state_fun_ptr_t foo_ptr_array[] = {
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
//		[S_STR_LIT] = &s_str_lit,
//		[S_ESCAPE_SEQ] = &s_escape_seq,
//		[S_HEX1] = &s_hex1,
//		[S_HEX2] = &s_hex2,
//		[S_UNDERSCORE] = &s_underscore,
//		[S_ZERO] = &s_zero,
//		[S_DEC_LIT] = &s_dec_lit,
//		[S_FLOAT_SCI_LIT] = &s_float_sci_lit,
//		[S_FLOAT_LIT] = &s_float_lit,
//		[S_FLOAT_EXP] = &s_float_exp,
//		[S_FLOAT_POINT] = &s_float_point,
//		[S_HEX_LIT] = &s_hex_lit,
//		[S_OCT_LIT] = &s_oct_lit,
//		[S_BIN_LIT] = &s_bin_lit
};

scanner_state_t s_start(token_t token, int c) {
	switch (c) {
		// end cases
		case '\n':
			token->type = TK_EOL;
			return S_END;
		case ')':
			token->type = TK_R_PARANTHESIS;
			return S_END;
		case '(':
			token->type = TK_L_PARANTHESIS;
			return S_END;
		case EOF:
			token->type = TK_EOF;
			return S_END;
		case ',':
			token->type = TK_SEPARATOR;
			return S_END;
		case ';':
			token->type = TK_SEMICOLON;
			return S_END;
		case '}':
			token->type = TK_R_CURLY;
			return S_END;
		case '{':
			token->type = TK_L_CURLY;
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

		default:
			break;
	}

	// FIXME: check validity
	token->type = TK_ERROR;
	return S_END;
}

scanner_state_t s_pipe(token_t token, int c) {
	if (c == '|') {
		token->type = TK_OR;
	} else {
		token->type = TK_ERROR;
	}
	return S_END;
}

scanner_state_t s_ampersand(token_t token, int c) {
	if (c == '&') {
		token->type = TK_AND;
	} else {
		token->type = TK_ERROR;
	}
	return S_END;
}

scanner_state_t s_colon(token_t token, int c) {
	if (c == '=') {
		token->type = TK_VAR_INNIT;
	} else {
		token->type = TK_ERROR;
	}
	return S_END;
}

scanner_state_t s_assign(token_t token, int c) {
	if (c == '=') {
		token->type = TK_EQUAL;
	} else {
		token->type = TK_ASSIGN;
		ungetc(c, stdin);
	}
	return S_END;
}

scanner_state_t s_less(token_t token, int c) {
	if (c == '=') {
		token->type = TK_LESS_EQUAL;
	} else {
		token->type = TK_LESS;
		ungetc(c, stdin);
	}
	return S_END;
}

scanner_state_t s_greater(token_t token, int c) {
	if (c == '=') {
		token->type = TK_GREATER_EQUAL;
	} else {
		token->type = TK_GREATER;
		ungetc(c, stdin);
	}
	return S_END;
}

scanner_state_t s_not(token_t token, int c) {
	if (c == '=') {
		token->type = TK_NOT_EQUAL;
	} else {
		token->type = TK_NOT;
		ungetc(c, stdin);
	}
	return S_END;
}

scanner_state_t s_plus(token_t token, int c) {
	if (c == '=') {
		token->type = TK_INC;
	} else {
		token->type = TK_PLUS;
		ungetc(c, stdin);
	}
	return S_END;
}

scanner_state_t s_minus(token_t token, int c) {
	if (c == '=') {
		token->type = TK_DEC;
	} else {
		token->type = TK_MINUS;
		ungetc(c, stdin);
	}
	return S_END;
}

scanner_state_t s_star(token_t token, int c) {
	if (c == '=') {
		token->type = TK_TIMES;
	} else {
		token->type = TK_STAR;
		ungetc(c, stdin);
	}
	return S_END;
}

scanner_state_t s_slash(token_t token, int c) {
	switch (c) {
		case '=':
			token->type = TK_DIVIDE;
			return S_END;
		case '/':
			return S_SL_COMMENT;
		case '*':
			return S_ML_COMMENT1;
		default:
			token->type = TK_SLASH;
			ungetc(c, stdin);
			return S_END; // FIXME: check validity
	}
}

scanner_state_t s_sl_comment(token_t token, int c) {
	switch (c) {
		case EOF:
			ungetc(c, stdin); // this falls trough intentionally
		case '\n':
			// do not send token for comment
			return S_START;
		default:
			return S_SL_COMMENT;
	}
}

scanner_state_t s_ml_comment1(token_t token, int c) {
	if (c == EOF) {
		token->type = TK_ERROR;
		return S_END;
	} else if (c == '*') {
		return S_ML_COMMENT2;
	}
	return S_ML_COMMENT1;
}

scanner_state_t s_ml_comment2(token_t token, int c) {
	if (c == EOF) {
		token->type = TK_ERROR;
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

// TODO

scanner_state_t s_str_lit(token_t token, int c);
scanner_state_t s_escape_seq(token_t token, int c);
scanner_state_t s_hex1(token_t token, int c);
scanner_state_t s_hex2(token_t token, int c);
scanner_state_t s_underscore(token_t token, int c);
scanner_state_t s_zero(token_t token, int c);
scanner_state_t s_dec_lit(token_t token, int c);
scanner_state_t s_float_sci_lit(token_t token, int c);
scanner_state_t s_float_lit(token_t token, int c);
scanner_state_t s_float_exp(token_t token, int c);
scanner_state_t s_float_point(token_t token, int c);
scanner_state_t s_hex_lit(token_t token, int c);
scanner_state_t s_oct_lit(token_t token, int c);
scanner_state_t s_bin_lit(token_t token, int c);
