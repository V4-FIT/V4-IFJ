#include "test_syntax_file.h"

#define EXAMPLES_PATH "go_files/examples"

TEST_F(SyntaxTestFile, eols) {
	SetUp(EXAMPLES_PATH, "eols.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, example1) {
	SetUp(EXAMPLES_PATH, "example1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, example2) {
	SetUp(EXAMPLES_PATH, "example2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, example3) {
	SetUp(EXAMPLES_PATH, "example3.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, hello) {
	SetUp(EXAMPLES_PATH, "hello.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, input) {
	SetUp(EXAMPLES_PATH, "input.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, inputs) {
	SetUp(EXAMPLES_PATH, "inputs.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, scope) {
	SetUp(EXAMPLES_PATH, "scope.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, two_pass) {
	SetUp(EXAMPLES_PATH, "two_pass.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, varfun) {
	SetUp(EXAMPLES_PATH, "varfun.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}
