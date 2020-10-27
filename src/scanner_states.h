/**
 * scanner_states.h
 * Private header used by scanner.c and scanner_states.c
 */

#ifndef IFJ_SCANNER_STATES_H
#define IFJ_SCANNER_STATES_H

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
	S_SL_COMMENT,
	S_ML_COMMENT1,
	S_ML_COMMENT2,
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
	S_BIN_LIT,
	S_END // special case -> returns the completed token
} scanner_state_t;

typedef scanner_state_t (*state_fun_ptr_t)(token_t, int);

scanner_state_t s_start(token_t token, int c);
scanner_state_t s_pipe(token_t token, int c);
scanner_state_t s_ampersand(token_t token, int c);
scanner_state_t s_colon(token_t token, int c);
scanner_state_t s_assign(token_t token, int c);
scanner_state_t s_less(token_t token, int c);
scanner_state_t s_greater(token_t token, int c);
scanner_state_t s_not(token_t token, int c);
scanner_state_t s_plus(token_t token, int c);
scanner_state_t s_minus(token_t token, int c);
scanner_state_t s_star(token_t token, int c);
scanner_state_t s_slash(token_t token, int c);
scanner_state_t s_sl_comment(token_t token, int c);
scanner_state_t s_ml_comment1(token_t token, int c);
scanner_state_t s_ml_comment2(token_t token, int c);
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

extern state_fun_ptr_t state_map[];

#endif //IFJ_SCANNER_STATES_H
