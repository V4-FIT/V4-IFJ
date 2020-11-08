#include "test_scanner.h"

TEST_F(ScannerTest, identifier_1) {
	fprintf(stream, "a");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_IDENTIFIER);
	EXPECT_STREQ(token->param.s, "a");

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, identifier_2) {
	fprintf(stream, "_x9 __");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_IDENTIFIER);
	EXPECT_STREQ(token->param.s, "_x9");

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_IDENTIFIER);
	EXPECT_STREQ(token->param.s, "__");

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, identifier_long) {
	fprintf(stream, "This_Is_A_Long_Variable_Name_0123456789");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_IDENTIFIER);
	EXPECT_STREQ(token->param.s, "This_Is_A_Long_Variable_Name_0123456789");

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, keyword_package) {
	fprintf(stream, "package");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_KEYW_PACKAGE);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, keyword_func) {
	fprintf(stream, "func");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_KEYW_FUNC);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, keyword_main) {
	fprintf(stream, "main");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_KEYW_MAIN);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, keyword_return) {
	fprintf(stream, "return");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_KEYW_RETURN);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, keyword_if) {
	fprintf(stream, "if");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_KEYW_IF);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, keyword_ELSE) {
	fprintf(stream, "else");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_KEYW_ELSE);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, keyword_for) {
	fprintf(stream, "for");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_KEYW_FOR);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, keyword_true) {
	fprintf(stream, "true");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_KEYW_TRUE);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, keyword_false) {
	fprintf(stream, "false");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_KEYW_FALSE);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, keyword_int) {
	fprintf(stream, "int");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_KEYW_INT);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, keyword_float64) {
	fprintf(stream, "float64");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_KEYW_FLOAT64);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, keyword_string) {
	fprintf(stream, "string");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_KEYW_STRING);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, keyword_bool) {
	fprintf(stream, "bool");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_KEYW_BOOL);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}
