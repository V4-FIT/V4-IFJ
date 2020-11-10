#include "test_scanner.h"

/*********************************************************

42
4_2
0o600
0O600       // second character is capital letter 'O'
0xBadFace
0xBad_Face
0x_67_7a_2f_cc_40_c6
170141183460469231731687303715884105727
170_141183_460469_231731_687303_715884_105727

_42         // an identifier, not an integer literal
42_         // invalid: _ must separate successive digits
4__2        // invalid: only one _ at a time
0_xBadFace  // invalid: _ must separate successive digits

*********************************************************/

// Underscore tests

TEST_F(ScannerTest, dec_underscore) {
	fprintf(stream, "4_2");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 42);

	scanner_retrieve_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, dec_underscores) {
	fprintf(stream, "1_7_0_1_4_1_1_8_3");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 170141183);

	scanner_retrieve_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, bin_underscores) {
	fprintf(stream, "0b1_10_1");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 13);

	scanner_retrieve_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, oct_underscores) {
	fprintf(stream, "0o7_7");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 63);

	scanner_retrieve_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, hex_underscores) {
	fprintf(stream, "0xBad_Face");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 0xBadFace);

	scanner_retrieve_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

// Regular tests

TEST_F(ScannerTest, dec_literal) {
	fprintf(stream, "123");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 123);

	scanner_retrieve_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, bin_literal) {
	fprintf(stream, "0b11");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 3);

	scanner_retrieve_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, bin_literal_alt_char) {
	fprintf(stream, "0B11");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 3);

	scanner_retrieve_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, bin_literal_prefix) {
	fprintf(stream, "0b00000011");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 3);

	scanner_retrieve_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, oct_literal) {
	fprintf(stream, "0o77");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 63);

	scanner_retrieve_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, oct_literal_alt_char) {
	fprintf(stream, "0O77");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 63);

	scanner_retrieve_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, oct_literal_prefix) {
	fprintf(stream, "0o0077");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 63);

	scanner_retrieve_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, hex_literal) {
	fprintf(stream, "0xFF");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 255);

	scanner_retrieve_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, hex_literal_alt_char) {
	fprintf(stream, "0XFF");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 255);

	scanner_retrieve_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, hex_literal_prefix) {
	fprintf(stream, "0x00FF");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 255);

	scanner_retrieve_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, hex_literal_lowercase) {
	fprintf(stream, "0x00ff");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 255);

	scanner_retrieve_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

/**
 * Lexical error tests
 */

TEST_F(ScannerTest, int_literal_lex_error_leading_zeroes) {
	fprintf(stream, "02");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_ERROR);
	ASSERT_EQ(token->param.i, ERROR_LEX);
}

TEST_F(ScannerTest, bin_literal_lex_error_offbyone) {
	fprintf(stream, "0b2");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_ERROR);
	ASSERT_EQ(token->param.i, ERROR_LEX);
}

TEST_F(ScannerTest, oct_literal_lex_error_offbyone) {
	fprintf(stream, "0o8");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_ERROR);
	ASSERT_EQ(token->param.i, ERROR_LEX);
}

TEST_F(ScannerTest, hex_literal_lex_error_offbyone) {
	fprintf(stream, "0xG");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_ERROR);
	ASSERT_EQ(token->param.i, ERROR_LEX);
}

// underscore lex errors

TEST_F(ScannerTest, int_literal_lex_error_underscore) {
	fprintf(stream, "0_x");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_ERROR);
	ASSERT_EQ(token->param.i, ERROR_LEX);
}

TEST_F(ScannerTest, dec_literal_lex_error_underscore_infix) {
	fprintf(stream, "4__2");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_ERROR);
	ASSERT_EQ(token->param.i, ERROR_LEX);
}

TEST_F(ScannerTest, dec_literal_lex_error_underscore_postfix) {
	fprintf(stream, "42_");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_ERROR);
	ASSERT_EQ(token->param.i, ERROR_LEX);
}

TEST_F(ScannerTest, bin_literal_lex_error_underscore_infix) {
	fprintf(stream, "0b1__0");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_ERROR);
	ASSERT_EQ(token->param.i, ERROR_LEX);
}

TEST_F(ScannerTest, bin_literal_lex_error_underscore_postfix) {
	fprintf(stream, "0b10_");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_ERROR);
	ASSERT_EQ(token->param.i, ERROR_LEX);
}

TEST_F(ScannerTest, oct_literal_lex_error_underscore_infix) {
	fprintf(stream, "0o7__2");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_ERROR);
	ASSERT_EQ(token->param.i, ERROR_LEX);
}

TEST_F(ScannerTest, oct_literal_lex_error_underscore_postfix) {
	fprintf(stream, "0o72_");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_ERROR);
	ASSERT_EQ(token->param.i, ERROR_LEX);
}

TEST_F(ScannerTest, hex_literal_lex_error_underscore_infix) {
	fprintf(stream, "0xB__B");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_ERROR);
	ASSERT_EQ(token->param.i, ERROR_LEX);
}

TEST_F(ScannerTest, hex_literal_lex_error_underscore_postfix) {
	fprintf(stream, "0xBB_");
	rewind(stream);

	scanner_retrieve_token(scanner);
	ASSERT_EQ(token->type, TK_ERROR);
	ASSERT_EQ(token->param.i, ERROR_LEX);
}
