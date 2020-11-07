#include "scanner_tests.h"

TEST_F(ScannerTest, string_literal_basic) {
	fprintf(stream, "\"test\"");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_STR_LIT);
	EXPECT_STREQ(token->param.s, "test");

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, string_literal_escape_replace) {
	fprintf(stream, R"("test\x2B")");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_STR_LIT);
	EXPECT_STREQ(token->param.s, "test+");

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, string_literal_escape_replace_lowercase) {
	fprintf(stream, R"("test\x2b")");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_STR_LIT);
	EXPECT_STREQ(token->param.s, "test+");

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, string_literal_escape_whitespace) {
	fprintf(stream, R"("test\n")");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_STR_LIT);
	EXPECT_STREQ(token->param.s, "test\n");

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, string_literal_escape_backslash) {
	fprintf(stream, R"("test\\")");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_STR_LIT);
	EXPECT_STREQ(token->param.s, "test\\");

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, string_literal_escape_quote) {
	fprintf(stream, R"("test\"")");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_STR_LIT);
	EXPECT_STREQ(token->param.s, "test\"");

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}
