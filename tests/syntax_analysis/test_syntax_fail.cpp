#include "test_syntax_file.h"

#define EXAMPLES_PATH "go_files/syntactic_fail"

TEST_F(SyntaxTestFile, func_type_param_double) {
	SetUp(EXAMPLES_PATH, "func_type_param_double.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(SyntaxTestFile, func_type_param_invalid) {
	SetUp(EXAMPLES_PATH, "func_type_param_invalid.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(SyntaxTestFile, func_type_return_double) {
	SetUp(EXAMPLES_PATH, "func_type_return_double.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(SyntaxTestFile, func_type_return_invalid) {
	SetUp(EXAMPLES_PATH, "func_type_return_invalid.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(SyntaxTestFile, func_type_return_invalid_2) {
	SetUp(EXAMPLES_PATH, "func_type_return_invalid_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(SyntaxTestFile, prolog_double) {
	SetUp(EXAMPLES_PATH, "prolog_double.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(SyntaxTestFile, prolog_invalid) {
	SetUp(EXAMPLES_PATH, "prolog_invalid.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(SyntaxTestFile, prolog_noeol) {
	SetUp(EXAMPLES_PATH, "prolog_noeol.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}
