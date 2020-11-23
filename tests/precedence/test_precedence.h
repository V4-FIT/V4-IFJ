#ifndef IFJ_TEST_PRECEDENCE_H
#define IFJ_TEST_PRECEDENCE_H

#include <cstdio>
#include <gtest/gtest.h>

extern "C" {
#include "error.h"
#include "tokens.h"
#include "parser.h"
}


#define TESTVAL(val) 	fscanf(stream, "\n"); \
						rewind(stream);		\
						EXPECT_EQ(parse(stream), val)

class Precedence : public testing::Test
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

#endif // !IFJ_TEST_PRECEDENCE_H
