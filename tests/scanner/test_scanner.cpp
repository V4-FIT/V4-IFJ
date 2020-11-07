#include "test_scanner.h"

// ---------------- Comments ----------------

TEST_F(ScannerTest, scanner_single_line_comment_1) {
	fprintf(stream, "// this is a sinngle line comment");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, scanner_single_line_comment_2) {
	fprintf(stream, " { // this is a sinngle line comment \n } ");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_L_CURLY);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_R_CURLY);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, scanner_multi_line_comment_1) {
	fprintf(stream, "/* this is a multi line comment */");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, scanner_multi_line_comment_2) {
	fprintf(stream, "/* this is a multi /* line ** comment **/");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, scanner_multi_line_comment_3) {
	fprintf(stream, "/* this is a \"mutli line\" comment */");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, scanner_multi_line_comment_4) {
	fprintf(stream, " { /* this is a single \n line comment */ } ");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_L_CURLY);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_R_CURLY);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, scanner_error_multi_line_comment_1) {
	fprintf(stream, "/* this is a mult-");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_ERROR);
	ASSERT_EQ(token->param.i, 1);
}

TEST_F(ScannerTest, scanner_error_multi_line_comment_2) {
	fprintf(stream, "/* this is a mult*");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_ERROR);
	ASSERT_EQ(token->param.i, 1);
}