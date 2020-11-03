#include <stdio.h>
#include <ctype.h>

#include "scanner_states.h"
#include "char_sequence.h"

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

scanner_state_t s_start(token_t token, int c) {
	switch (c) {
		// whitespace skip
		case ' ':
		case '\t':
			return S_START;

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
		case '\"':
			return S_STR_LIT;

		case '0':
			if (charseq_push_back(token->value,c)) {
				return S_ZERO;
			} else {
				token->type = TK_INTERNAL_ERROR;
				return S_END;
			}		
		case '_':
			if (charseq_push_back(token->value,c)) {
				return S_UNDERSCORE;
			} else {
				token->type = TK_INTERNAL_ERROR;
				return S_END;
			}

		default:
			if (isalpha(c)){
				return S_IDENTIF;
			}
			if (isdigit(c)) { //special case for 0 handled above
				return S_DEC_LIT;
			}
			token->type = TK_ERROR;
			return S_END;
	}
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
			return S_END;
	}
}

scanner_state_t s_sl_comment(token_t token, int c) {
	switch (c) {
		case EOF:
		case '\n':
			ungetc(c, stdin); 
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

scanner_state_t s_str_lit(token_t token, int c) {
	switch (c) {
		case '\\':
			if (charseq_push_back(token->value,c)) {
				return S_ESCAPE_SEQ;
			} else {
				token->type = TK_INTERNAL_ERROR;
				return S_END;	
			}
		case '\"':
			token->type = TK_STR_LIT;
			token->value = NULL;
			return S_END;
		default:
			if (c < ' ') { //includes all unprintables, EOL and EOF
				token->type = TK_ERROR;
				return S_END;
			} else if (charseq_push_back(token->value,c)) {
				return S_STR_LIT;
			} else {
				token->type = TK_INTERNAL_ERROR;
				return S_END;
			}
	}
}
scanner_state_t s_escape_seq(token_t token, int c) {
	switch (c) {
		case 'n':
		case 't':
		case '\\':
		case '\"':
			if (charseq_push_back(token->value,c)) {
				return S_STR_LIT;
			} else {
				token->type = TK_INTERNAL_ERROR;
				return S_END;
			}
		case 'x':
			if (charseq_push_back(token->value,c)) {
				return S_HEX1;
			} else {
				token->type = TK_INTERNAL_ERROR;
				return S_END;
			}
		default:
			token->type = TK_ERROR;
			return S_END;
	}
}

scanner_state_t s_hex1(token_t token, int c) {
	if (isxdigit(c)){
		if (charseq_push_back(token->value,c)) {
			return S_HEX2;
		} else {
			token->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else {
		token->type = TK_ERROR;
		return S_END;
	}
}
scanner_state_t s_hex2(token_t token, int c) {
	scanner_state_t return_val = s_hex1(token,c); //only difference is in return value so we use logic from s_hex1
	if (return_val == S_HEX2){
		return S_STR_LIT;
	} else {
		return return_val;
	}
}
scanner_state_t s_underscore(token_t token, int c) {
	if (isalnum(c) || (c == '_')) { 
		if (charseq_push_back(token->value,c)) {
			return S_IDENTIF;
		} else {
			token->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else {
		ungetc(c,stdin);
		token->type = TK_UNDERSCORE;
		return S_END;
	}
}

scanner_state_t s_zero(token_t token, int c) {
	switch (c) {
		case '.':
			if (charseq_push_back(token->value,c)) {
				return S_FLOAT_POINT;
			} else {
				token->type = TK_INTERNAL_ERROR;
				return S_END;
			}
		case 'b':
		case 'B':
			if (charseq_push_back(token->value,c)) {
				return S_BIN_LIT1;
			} else {
				token->type = TK_INTERNAL_ERROR;
				return S_END;
			}
		case 'o':
		case 'O':
			if (charseq_push_back(token->value,c)) {
				return S_OCT_LIT1;
			} else {
				token->type = TK_INTERNAL_ERROR;
				return S_END;
			}
		case 'x':
		case 'X':
			if (charseq_push_back(token->value,c)) {
				return S_HEX_LIT2;
			} else {
				token->type = TK_INTERNAL_ERROR;
				return S_END;
			}
		case 'e': //TODO: check if 0e123 is valid
		case 'E':
			if (charseq_push_back(token->value,c)) {
				return S_FLOAT_EXP1;
			} else {
				token->type = TK_INTERNAL_ERROR;
				return S_END;
			}
		case '0':
			token->type = TK_ERROR;
			return S_END;
		default:
			if (isdigit(c)){
				return S_DEC_LIT;
			} else {
				ungetc(c,stdin);
				token->type = TK_ZERO; //TK_INT
				return S_END;
			}
	}
}

scanner_state_t s_dec_lit(token_t token, int c) {
	switch (c) {
		case '.':
			if (charseq_push_back(token->value,c)) {
				return S_FLOAT_POINT;
			} else {
				token->type = TK_INTERNAL_ERROR;
				return S_END;
			}
		case 'e':
		case 'E':
			if (charseq_push_back(token->value,c)) {
				return S_FLOAT_EXP1;
			} else {
				token->type = TK_INTERNAL_ERROR;
				return S_END;
			}
		default:
			if (isdigit(c)){
				return S_DEC_LIT;
			} else {
				ungetc(c,stdin);
				token->type = TK_DEC_LIT; //TK_INT
				return S_END;
			}
	}
}

scanner_state_t s_float_sci_lit(token_t token, int c) {
	if (isdigit(c)) {
		if (charseq_push_back(token->value,c)) {
			return S_FLOAT_SCI_LIT;
		} else {
			token->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else {
		ungetc(c,stdin);
		token->type = TK_FLOAT_LIT; 
		return S_END;
	}
}

scanner_state_t s_float_lit(token_t token, int c) {
	switch (c) {
		case 'e':
		case 'E':
			if (charseq_push_back(token->value,c)) {
					return S_FLOAT_EXP1;
				} else {
					token->type = TK_INTERNAL_ERROR;
					return S_END;
				}
		default:
			if (isdigit(c)) {
				if (charseq_push_back(token->value,c)) {
					return S_FLOAT_LIT;
				} else {
					token->type = TK_INTERNAL_ERROR;
					return S_END;
				}
			} else {
				ungetc(c,stdin);
				token->type = TK_FLOAT_LIT; 
				return S_END;
			}
	}
}

scanner_state_t s_float_exp1(token_t token, int c) { //TODO:FIX
	if (isdigit(c)) {
		if (charseq_push_back(token->value,c)) {
			return S_FLOAT_SCI_LIT;
		} else {
			token->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else if (c == '-' || c == '+') {
		if (charseq_push_back(token->value,c)) {
			return S_FLOAT_EXP2;
		} else {
			token->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else {
		token->type = TK_ERROR;
		return S_END;
	}
}

scanner_state_t s_float_exp2(token_t token, int c) {
	if (isdigit(c)) {
		if (charseq_push_back(token->value,c)) {
			return S_FLOAT_SCI_LIT;
		} else {
			token->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else {
		token->type = TK_ERROR;
		return S_END;
	}
}

scanner_state_t s_float_point(token_t token, int c) {
	if (isdigit(c)){
		if (charseq_push_back(token->value,c)) {
			return S_FLOAT_LIT;
		} else {
			token->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else {
		token->type = TK_ERROR;
		return S_END;
	}
}

scanner_state_t s_hex_lit1(token_t token, int c){
	if (isxdigit(c)){
		if (charseq_push_back(token->value,c)) {
			return S_HEX_LIT2;
		} else {
			token->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else {
		token->type = TK_ERROR;
		return S_END;
	}
}

scanner_state_t s_hex_lit2(token_t token, int c){ //TODO: refactor when tested
	if (isxdigit(c)){
		if (charseq_push_back(token->value,c)) {
			return S_HEX_LIT2;
		} else {
			token->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else {
		ungetc(c,stdin);
		token->type = TK_HEX_LIT; //TK_INT
	}
}

scanner_state_t s_oct_lit1(token_t token, int c) {
	if (c >= '0' && c <= '7'){
		if (charseq_push_back(token->value,c)) {
			return S_OCT_LIT2;
		} else {
			token->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else {
		token->type = TK_ERROR;
		return S_END;
	}
}

scanner_state_t s_oct_lit2(token_t token, int c) { //TODO: refactor when tested
	if (c >= '0' && c <= '7'){
		if (charseq_push_back(token->value,c)) {
			return S_OCT_LIT2;
		} else {
			token->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else {
			ungetc(c,stdin);
			token->type = TK_OCT_LIT; //TK_INT
			return S_END;
	}
}

scanner_state_t s_bin_lit1(token_t token, int c) {
	if (c == '0' || c == '1'){
		if (charseq_push_back(token->value,c)) {
			return S_BIN_LIT2;
		} else {
			token->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else {
		token->type = TK_ERROR;
		return S_END;
	}
}

scanner_state_t s_bin_lit2(token_t token, int c){ //TODO: refactor when tested
	if (c == '0' || c == '1'){
		if (charseq_push_back(token->value,c)) {
			return S_BIN_LIT2;
		} else {
			token->type = TK_INTERNAL_ERROR;
			return S_END;
		}
	} else {
		ungetc(c,stdin);
		token->type = TK_BIN_LIT; //TK_INT
		return S_END;
	}
}

scanner_state_t s_identif(token_t token, int c){
	if (isalnum(c) || (c == '_')) {
		if (charseq_push_back(token->value,c)) {
				return S_IDENTIF;
			} else {
				token->type = TK_INTERNAL_ERROR;
				return S_END;
			}
	} else {
		ungetc(c,stdin);
		//TODO
		//token->type = hash table magic 
		return S_END;
	}

}
