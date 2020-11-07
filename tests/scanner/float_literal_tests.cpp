#include "scanner_tests.h"

TEST_F(ScannerTest, float_literal_zero) {
	fprintf(stream, "0.25");

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 0.25);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, float_literal_nonzero) {
	fprintf(stream, "2.25");

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 2.25);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, float_literal_exp_zero) {
	fprintf(stream, "0e3");

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 0.0);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, float_literal_exp_zero_altchar) {
	fprintf(stream, "0E3");

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 0.0);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, float_literal_exp_nonzero) {
	fprintf(stream, "12e3");

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 12000.0);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, float_literal_exp_altchar) {
	fprintf(stream, "12E3");

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 12000.0);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, float_literal_exp_zeroexp) {
	fprintf(stream, "0.1e0");

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 0.1);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, float_literal_exp_floating_zero) {
	fprintf(stream, "0.1e3");

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 100.0);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, float_literal_exp_floating_nonzero) {
	fprintf(stream, "12.1e3");

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 12100.0);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, float_literal_exp_plus) {
	fprintf(stream, "12.1e+3");

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 12100.0);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, float_literal_exp_minus) {
	fprintf(stream, "12.5e-2");

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 0.125);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

/**
 * Lexical error tests
 */

TEST_F(ScannerTest, float_literal_lex_error_invalid_char) {
	fprintf(stream, "0.a25");

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_ERROR);
}

TEST_F(ScannerTest, float_literal_lex_error_invalid_exp) {
	fprintf(stream, "0.e25");

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_ERROR);
}

TEST_F(ScannerTest, float_literal_lex_error_exp_eps) {
	fprintf(stream, "0.2e");

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_ERROR);
}
