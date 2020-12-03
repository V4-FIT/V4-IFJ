#include "test_testsuite.h"

//// Characters
TEST_F(TestSuiteLex, characters_ampersand) {
	SetUp("go_files/lex/characters", "amp.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), ERROR_LEX);
}

TEST_F(TestSuiteLex, characters_bracket) {
	SetUp("go_files/lex/characters", "bracket.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), ERROR_LEX);
}

TEST_F(TestSuiteLex, characters_dolar) {
	SetUp("go_files/lex/characters", "dolar.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), ERROR_LEX);
}

TEST_F(TestSuiteLex, characters_dot) {
	SetUp("go_files/lex/characters", "dot.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), ERROR_LEX);
}

TEST_F(TestSuiteLex, characters_question) {
	SetUp("go_files/lex/characters", "question.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), ERROR_LEX);
}

TEST_F(TestSuiteLex, characters_sharp) {
	SetUp("go_files/lex/characters", "sharp.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), ERROR_LEX);
}

//// Comments
TEST_F(TestSuiteLex, comments_block_line) {
	SetUp("go_files/lex/comments", "block_line.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteLex, comments_line_block) {
	SetUp("go_files/lex/comments", "line_block.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteLex, comments_line_nested) {
	SetUp("go_files/lex/comments", "line_nested.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteLex, comments_multiline) {
	SetUp("go_files/lex/comments", "multiline.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteLex, comments_unending) {
	SetUp("go_files/lex/comments", "unending.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), ERROR_LEX);
}

//// ids

TEST_F(TestSuiteLex, id_all) {
	SetUp("go_files/lex/ids", "id_all.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteLex, id_numbers) {
	SetUp("go_files/lex/ids", "id_numbers.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteLex, id_starting_underscore) {
	SetUp("go_files/lex/ids", "id_starting_underscore.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteLex, underscore_id) {
	SetUp("go_files/lex/ids", "underscore_id.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
}

//// numbers

TEST_F(TestSuiteLex, float_dot) {
	SetUp("go_files/lex/numbers", "float_dot.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteLex, float_dot_zero) {
	SetUp("go_files/lex/numbers", "float_dot_zero.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteLex, float_dot_zeroes) {
	SetUp("go_files/lex/numbers", "float_dot_zeroes.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteLex, float_exp_1) {
	SetUp("go_files/lex/numbers", "float_exp_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteLex, float_exp_2) {
	SetUp("go_files/lex/numbers", "float_exp_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteLex, float_exp_3) {
	SetUp("go_files/lex/numbers", "float_exp_3.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteLex, float_exp_4) {
	SetUp("go_files/lex/numbers", "float_exp_4.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteLex, float_exp_dot_1) {
	SetUp("go_files/lex/numbers", "float_exp_dot_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteLex, float_exp_dot_2) {
	SetUp("go_files/lex/numbers", "float_exp_dot_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteLex, float_exp_zero_1) {
	SetUp("go_files/lex/numbers", "float_exp_zero_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteLex, float_exp_zero_2) {
	SetUp("go_files/lex/numbers", "float_exp_zero_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteLex, float_exp_zeroes) {
	SetUp("go_files/lex/numbers", "float_exp_zeroes.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteLex, float_space) {
	SetUp("go_files/lex/numbers", "float_space.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), ERROR_LEX);
}

TEST_F(TestSuiteLex, number_int) {
	SetUp("go_files/lex/numbers", "int.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteLex, leading_zeroes_float) {
	SetUp("go_files/lex/numbers", "leading_zeroes_float.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), ERROR_LEX);
}

TEST_F(TestSuiteLex, leading_zeroes_int) {
	SetUp("go_files/lex/numbers", "leading_zeroes_int.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), ERROR_LEX);
}

TEST_F(TestSuiteLex, zero_float) {
	SetUp("go_files/lex/numbers", "zero_float.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteLex, zero_int) {
	SetUp("go_files/lex/numbers", "zero_int.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteLex, zeroes) {
	SetUp("go_files/lex/numbers", "zeroes.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), ERROR_LEX);
}
