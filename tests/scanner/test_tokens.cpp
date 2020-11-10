#include "test_scanner.h"

// --------------- Separators ---------------

TEST_F(ScannerTest, tk_l_parenthesis) {
	fprintf(stream, "(");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_L_PARENTHESIS);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, tk_r_parenthesis) {
	fprintf(stream, ")");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_R_PARENTHESIS);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, tk_l_curly) {
	fprintf(stream, "{");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_L_CURLY);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, tk_r_curly) {
	fprintf(stream, "}");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_R_CURLY);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, tk_comma) {
	fprintf(stream, ",");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_COMMA);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, tk_semicolon) {
	fprintf(stream, ";");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_SEMICOLON);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, tk_eol) {
	fprintf(stream, "\n");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_EOL);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

// ---------- Arithmetic operators ----------

TEST_F(ScannerTest, tk_plus) {
	fprintf(stream, "+");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_PLUS);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, tk_minus) {
	fprintf(stream, "-");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_MINUS);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, tk_multiply) {
	fprintf(stream, "*");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_MULTIPLY);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, tk_divide) {
	fprintf(stream, "/");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_DIVIDE);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

// ---------- Relational operators ----------

TEST_F(ScannerTest, tk_equal) {
	fprintf(stream, "==");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_EQUAL);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, tk_not_equal) {
	fprintf(stream, "!=");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_NOT_EQUAL);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, tk_less) {
	fprintf(stream, "<");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_LESS);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, tk_less_equal) {
	fprintf(stream, "<=");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_LESS_EQUAL);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, tk_greater) {
	fprintf(stream, ">");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_GREATER);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, tk_greater_equal) {
	fprintf(stream, ">=");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_GREATER_EQUAL);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

// ----------- Logical operators ------------

TEST_F(ScannerTest, tk_or) {
	fprintf(stream, "||");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_OR);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, tk_and) {
	fprintf(stream, "&&");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_AND);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, tk_not) {
	fprintf(stream, "!");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_NOT);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

// ---------- Assignment operators ----------

TEST_F(ScannerTest, tk_var_init) {
	fprintf(stream, ":=");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_VAR_INIT);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, tk_assign) {
	fprintf(stream, "=");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_ASSIGN);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, tk_plus_assign) {
	fprintf(stream, "+=");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_PLUS_ASSIGN);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, tk_minus_assign) {
	fprintf(stream, "-=");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_MINUS_ASSIGN);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, tk_multiply_assign) {
	fprintf(stream, "*=");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_MULTIPLY_ASSIGN);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, tk_divide_assign) {
	fprintf(stream, "/=");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_DIVIDE_ASSIGN);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

// --------------- Identifier ---------------
TEST_F(ScannerTest, tk_underscore) {
	fprintf(stream, "_");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_UNDERSCORE);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}




