#include "test_scanner.h"

TEST_F(ScannerTest, dec_zero) {
	fprintf(stream, "0");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 0);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, dec_literal) {
	fprintf(stream, "123");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 123);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, bin_literal) {
	fprintf(stream, "0b11");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 3);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, bin_literal_alt_char) {
	fprintf(stream, "0B11");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 3);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, bin_literal_prefix) {
	fprintf(stream, "0b00000011");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 3);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, oct_literal) {
	fprintf(stream, "0o77");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 63);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, oct_literal_alt_char) {
	fprintf(stream, "0O77");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 63);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, oct_literal_prefix) {
	fprintf(stream, "0o0077");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 63);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, hex_literal) {
	fprintf(stream, "0xFF");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 255);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, hex_literal_alt_char) {
	fprintf(stream, "0XFF");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 255);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, hex_literal_prefix) {
	fprintf(stream, "0x00FF");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 255);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

TEST_F(ScannerTest, hex_literal_lowercase) {
	fprintf(stream, "0x00ff");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_INT_LIT);
	EXPECT_EQ(token->param.i, 255);

	scanner_retrieve_token(scanner, token);
	EXPECT_EQ(token->type, TK_EOF);
}

/**
 * Lexical error tests
 */

TEST_F(ScannerTest, bin_literal_lex_error_offbyone) {
	fprintf(stream, "0b2");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_ERROR);
}

TEST_F(ScannerTest, oct_literal_lex_error_offbyone) {
	fprintf(stream, "0o8");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_ERROR);
}

TEST_F(ScannerTest, hex_literal_lex_error_offbyone) {
	fprintf(stream, "0xG");
	rewind(stream);

	scanner_retrieve_token(scanner, token);
	ASSERT_EQ(token->type, TK_ERROR);
}
