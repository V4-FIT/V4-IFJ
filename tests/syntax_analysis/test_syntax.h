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

#define OPENFUN(name)         \
	fprintf(stream, "func "); \
	fprintf(stream, name);    \
	fprintf(stream, "()() {\n")

#define CLOSEFUN fprintf(stream, "}\n")

#define TESTVAL(val) \
	rewind(stream);  \
	EXPECT_EQ(parser_parse(parser), val)

class SyntaxTest : public testing::Test
{
public:
	FILE *stream;
	parser_t parser;

	virtual void SetUp() {
		stream = tmpfile();
		parser = parser_init(stream);
	}

	virtual void TearDown() {
		parser_free(parser);
		fclose(stream);
	}
};

#endif // !IFJ_TEST_SYNTAX_H
