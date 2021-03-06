#include "test_scanner.h"

TEST_F(ScannerTest, float_literal_zero) {
	fprintf(stream, "0.25");
	rewind(stream);

	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 0.25);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, float_literal_nonzero) {
	fprintf(stream, "2.25");
	rewind(stream);

	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 2.25);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, float_literal_floating_zeroes) {
	fprintf(stream, "0.0625");
	rewind(stream);

	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 0.0625);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, float_literal_exp_zero) {
	fprintf(stream, "0e3");
	rewind(stream);

	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 0.0);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, float_literal_exp_zero_altchar) {
	fprintf(stream, "0E3");
	rewind(stream);

	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 0.0);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, float_literal_exp_nonzero) {
	fprintf(stream, "12e3");
	rewind(stream);

	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 12000.0);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, float_literal_exp_altchar) {
	fprintf(stream, "12E3");
	rewind(stream);

	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 12000.0);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, float_literal_exp_zeroexp) {
	fprintf(stream, "0.1e0");
	rewind(stream);

	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 0.1);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, float_literal_exp_floating_zero) {
	fprintf(stream, "0.1e3");
	rewind(stream);

	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 100.0);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, float_literal_exp_floating_nonzero) {
	fprintf(stream, "12.1e3");
	rewind(stream);

	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 12100.0);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, float_literal_exp_plus) {
	fprintf(stream, "12.1e+3");
	rewind(stream);

	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 12100.0);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, float_literal_exp_minus) {
	fprintf(stream, "12.5e-2");
	rewind(stream);

	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 0.125);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, float_literal_ignore_exp_zeroes) {
	fprintf(stream, "12.5e-002");
	rewind(stream);

	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 0.125);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

/**
 * Lexical error tests
 */

TEST_F(ScannerTest, float_literal_lex_error_invalid_char) {
	fprintf(stream, "0.a25");
	rewind(stream);

	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_ERROR);
}

TEST_F(ScannerTest, float_literal_lex_error_invalid_exp) {
	fprintf(stream, "0.e25");
	rewind(stream);

	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_ERROR);
}

TEST_F(ScannerTest, float_literal_lex_error_exp_eps) {
	fprintf(stream, "0.2e");
	rewind(stream);

	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_ERROR);
}

TEST_F(ScannerTest, float_literal_lex_error_exp_invalid_char) {
	fprintf(stream, "0.2ea");
	rewind(stream);

	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_ERROR);
}

TEST_F(ScannerTest, float_literal_lex_error_exp_invalid_char2) {
	fprintf(stream, "0.2e!");
	rewind(stream);

	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_ERROR);
}
