#include "test_syntax.h"

TEST_F(SyntaxTest, prolog) {
	fprintf(stream, "package main\n");
	rewind(stream);

	int res = analyse_syntax(stream);

	EXPECT_EQ(res, EXIT_SUCCESS);
}

TEST_F(SyntaxTest, prolog_fail) {
	fprintf(stream, "package main\n l");
	rewind(stream);

	int res = analyse_syntax(stream);

	EXPECT_EQ(res, ERROR_SYN);
}
