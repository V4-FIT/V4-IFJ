#ifndef IFJ_TEST_SYNTAX_H
#define IFJ_TEST_SYNTAX_H

#include <cstdio>
#include <gtest/gtest.h>

extern "C" {
#include "error.h"
#include "tokens.h"
#include "parser.h"
}

#define PROLOG fprintf(stream, "package main\n")
#define OPENFUN(name)	fprintf(stream, "func " );		\
						fprintf(stream, name);			\
						fprintf(stream, "()() {\n")
#define CLOSEFUN fprintf(stream, "}\n")

#define TESTVAL(val)	rewind(stream);		\
					EXPECT_EQ(parse(stream), val)

class SyntaxTest : public testing::Test
{
public:
	FILE *stream;

	virtual void SetUp() {
		stream = tmpfile();
	}

	virtual void TearDown() {
		fclose(stream);
	}
};

#endif // !IFJ_TEST_SYNTAX_H
