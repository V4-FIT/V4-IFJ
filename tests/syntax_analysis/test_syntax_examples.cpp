#include "test_syntax_file.h"

#define EXAMPLES_PATH "../../tests/examples"

TEST_F(SyntaxTestFile, example_eols) {
	SetUp(EXAMPLES_PATH, "eols.go");

	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, example_example1) {
	SetUp(EXAMPLES_PATH, "example1.go");

	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, example_example2) {
	SetUp(EXAMPLES_PATH, "example2.go");

	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, example_example3) {
	SetUp(EXAMPLES_PATH, "example3.go");

	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, example_hello) {
	SetUp(EXAMPLES_PATH, "hello.go");

	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, example_input) {
	SetUp(EXAMPLES_PATH, "input.go");

	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, example_inputs) {
	SetUp(EXAMPLES_PATH, "inputs.go");

	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, example_scope) {
	SetUp(EXAMPLES_PATH, "scope.go");

	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, example_two_pass) {
	SetUp(EXAMPLES_PATH, "two-pass.go");

	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, example_varfun) {
	SetUp(EXAMPLES_PATH, "varfun.go");

	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}
