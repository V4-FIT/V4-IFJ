#include "test_syntax_file.h"

#define EXAMPLES_PATH "../../tests/examples"

TEST_F(SyntaxTestFile, example_eols) {
	SetUp(EXAMPLES_PATH, "eols.go");
	ASSERT_NE(stream, nullptr);

	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, example_example1) {
	SetUp(EXAMPLES_PATH, "example1.go");
	ASSERT_NE(stream, nullptr);

	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, example_example2) {
	SetUp(EXAMPLES_PATH, "example2.go");
	ASSERT_NE(stream, nullptr);

	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, example_example3) {
	SetUp(EXAMPLES_PATH, "example3.go");
	ASSERT_NE(stream, nullptr);

	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, example_hello) {
	SetUp(EXAMPLES_PATH, "hello.go");
	ASSERT_NE(stream, nullptr);

	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, example_input) {
	SetUp(EXAMPLES_PATH, "input.go");
	ASSERT_NE(stream, nullptr);

	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, example_inputs) {
	SetUp(EXAMPLES_PATH, "inputs.go");
	ASSERT_NE(stream, nullptr);

	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, example_scope) {
	SetUp(EXAMPLES_PATH, "scope.go");
	ASSERT_NE(stream, nullptr);

	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, example_two_pass) {
	SetUp(EXAMPLES_PATH, "two-pass.go");
	ASSERT_NE(stream, nullptr);

	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

TEST_F(SyntaxTestFile, example_varfun) {
	SetUp(EXAMPLES_PATH, "varfun.go");
	ASSERT_NE(stream, nullptr);

	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}
