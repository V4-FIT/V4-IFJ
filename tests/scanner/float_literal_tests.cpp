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


TEST_F(ScannerTest, float_literal_exp_nonzero) {
	fprintf(stream, "1e3");

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 1000.0);

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
	fprintf(stream, "1.1e3");

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_FLOAT_LIT);
	EXPECT_EQ(token->param.f, 1100.0);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}
