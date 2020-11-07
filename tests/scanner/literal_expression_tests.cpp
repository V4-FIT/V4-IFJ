#include "scanner_tests.h"

TEST_F(ScannerTest, expression_basic) {
	fprintf(stream, "5 + 5");

	// 5
	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 5);

	// +
	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_PLUS);

	// 5
	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 5);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, expression_basic_spaces) {
	fprintf(stream, "\t5 \t     \t+   \t 5\t ");

	// 5
	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 5);

	// +
	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_PLUS);

	// 5
	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 5);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, expression_basic_nospaces) {
	fprintf(stream, "5+5");

	// 5
	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 5);

	// +
	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_PLUS);

	// 5
	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 5);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}
