#include "test_scanner.h"

// ------------- Lexical Errors -------------

TEST_F(ScannerTest, s_unsopported_char) {
	fprintf(stream, "#");
	rewind(stream);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_ERROR);
	ASSERT_EQ(token->param.i, 1);
}

TEST_F(ScannerTest, s_pipe_lex_error) {
	fprintf(stream, "|#");
	rewind(stream);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_ERROR);
	ASSERT_EQ(token->param.i, 1);
}

TEST_F(ScannerTest, s_ampersand_lex_error) {
	fprintf(stream, "&#");
	rewind(stream);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_ERROR);
	ASSERT_EQ(token->param.i, 1);
}

TEST_F(ScannerTest, s_colon_lex_error) {
	fprintf(stream, ":#");
	rewind(stream);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_ERROR);
	ASSERT_EQ(token->param.i, 1);
}

TEST_F(ScannerTest, s_escape_seq_lex_error) {
	fprintf(stream, R"("\#")");
	rewind(stream);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_ERROR);
	ASSERT_EQ(token->param.i, 1);
}

// ---------------- Comments ----------------

TEST_F(ScannerTest, s_sl_comment_1) {
	fprintf(stream, "// this is a sinngle line comment");
	rewind(stream);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, s_sl_comment_2) {
	fprintf(stream, " { // this is a single line comment \n } ");
	rewind(stream);

	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_L_CURLY);

	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_EOL);

	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_R_CURLY);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, s_ml_comment_1) {
	fprintf(stream, "/* this is a multi line comment */");
	rewind(stream);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, s_ml_comment_2) {
	fprintf(stream, "/* this is a multi /* line ** comment **/");
	rewind(stream);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, s_ml_comment_3) {
	fprintf(stream, "/* this is a \"mutli line\" comment */");
	rewind(stream);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, s_ml_comment_4) {
	fprintf(stream, " { /* this is a single \n line comment */ } ");
	rewind(stream);

	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_L_CURLY);

	scanner_next_token(scanner);
	ASSERT_EQ(token->type, TK_R_CURLY);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, s_ml_comment_lex_error_1) {
	fprintf(stream, "/* this is a mult-");
	rewind(stream);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_ERROR);
	ASSERT_EQ(token->param.i, 1);
}

TEST_F(ScannerTest, s_ml_comment_lex_error_2) {
	fprintf(stream, "/* this is a mult*");
	rewind(stream);

	scanner_next_token(scanner);
	EXPECT_EQ(token->type, TK_ERROR);
	ASSERT_EQ(token->param.i, 1);
}
