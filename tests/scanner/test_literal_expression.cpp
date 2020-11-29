#include "test_scanner.h"

TEST_F(ScannerTest, expression_basic) {
	fprintf(stream, "5 + 5");
	rewind(stream);

	// 5
	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 5);

	// +
	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_PLUS);

	// 5
	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 5);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, expression_basic_spaces) {
	fprintf(stream, "\t5 \t     \t+   \t 5\t ");
	rewind(stream);

	// 5
	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 5);

	// +
	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_PLUS);

	// 5
	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 5);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, expression_basic_nospaces) {
	fprintf(stream, "5+5");
	rewind(stream);

	// 5
	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 5);

	// +
	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_PLUS);

	// 5
	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 5);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}
