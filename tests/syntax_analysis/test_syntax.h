#ifndef IFJ_TEST_SYNTAX_H
#define IFJ_TEST_SYNTAX_H

#include <cstdio>
#include <gtest/gtest.h>

extern "C" {
#include "error.h"
#include "tokens.h"
#include "parser.h"
}

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
