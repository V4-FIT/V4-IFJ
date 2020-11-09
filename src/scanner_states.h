/**
 * scanner_states.h
 * Private header used by scanner.c and scanner_states.c
 */

#ifndef IFJ_SCANNER_STATES_H
#define IFJ_SCANNER_STATES_H

#include "tokens.h"
#include "scanner.h"

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
	S_DEC_LIT_UNDERSCORE,
	S_FLOAT_SCI_LIT,
	S_FLOAT_LIT,
	S_FLOAT_EXP,
	S_FLOAT_POINT,
	S_HEX_LIT1,
	S_HEX_LIT2,
	S_HEX_LIT_UNDERSCORE,
	S_OCT_LIT1,
	S_OCT_LIT2,
	S_OCT_LIT_UNDERSCORE,
	S_BIN_LIT1,
	S_BIN_LIT2,
	S_BIN_LIT_UNDERSCORE,
	S_IDENTIF,
	S_END // special case -> returns the completed token
} scanner_state_t;

typedef scanner_state_t (*state_fun_ptr_t)(scanner_t, int);

scanner_state_t s_start(scanner_t scanner, int c);
scanner_state_t s_pipe(scanner_t scanner, int c);
scanner_state_t s_ampersand(scanner_t scanner, int c);
scanner_state_t s_colon(scanner_t scanner, int c);
scanner_state_t s_assign(scanner_t scanner, int c);
scanner_state_t s_less(scanner_t scanner, int c);
scanner_state_t s_greater(scanner_t scanner, int c);
scanner_state_t s_not(scanner_t scanner, int c);
scanner_state_t s_plus(scanner_t scanner, int c);
scanner_state_t s_minus(scanner_t scanner, int c);
scanner_state_t s_star(scanner_t scanner, int c);
scanner_state_t s_slash(scanner_t scanner, int c);
scanner_state_t s_sl_comment(scanner_t scanner, int c);
scanner_state_t s_ml_comment1(scanner_t scanner, int c);
scanner_state_t s_ml_comment2(scanner_t scanner, int c);
scanner_state_t s_str_lit(scanner_t scanner, int c);
scanner_state_t s_escape_seq(scanner_t scanner, int c);
scanner_state_t s_hex1(scanner_t scanner, int c);
scanner_state_t s_hex2(scanner_t scanner, int c);
scanner_state_t s_underscore(scanner_t scanner, int c);
scanner_state_t s_zero(scanner_t scanner, int c);
scanner_state_t s_dec_lit(scanner_t scanner, int c);
scanner_state_t s_dec_lit_underscore(scanner_t scanner, int c);
scanner_state_t s_float_sci_lit(scanner_t scanner, int c);
scanner_state_t s_float_lit(scanner_t scanner, int c);
scanner_state_t s_float_exp(scanner_t scanner, int c);
scanner_state_t s_float_point(scanner_t scanner, int c);
scanner_state_t s_hex_lit1(scanner_t scanner, int c);
scanner_state_t s_hex_lit2(scanner_t scanner, int c);
scanner_state_t s_hex_lit_underscore(scanner_t scanner, int c);
scanner_state_t s_oct_lit1(scanner_t scanner, int c);
scanner_state_t s_oct_lit2(scanner_t scanner, int c);
scanner_state_t s_oct_lit_underscore(scanner_t scanner, int c);
scanner_state_t s_bin_lit1(scanner_t scanner, int c);
scanner_state_t s_bin_lit2(scanner_t scanner, int c);
scanner_state_t s_bin_lit_underscore(scanner_t scanner, int c);
scanner_state_t s_identif(scanner_t scanner, int c);

extern state_fun_ptr_t state_map[];

#endif //IFJ_SCANNER_STATES_H
