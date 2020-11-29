#ifndef IFJ_TEST_SYNTAX_H
#define IFJ_TEST_SYNTAX_H

#include <cstdio>
#include <gtest/gtest.h>

extern "C" {
#include "error.h"
#include "tokens.h"
#include "parser.h"
#include "scanner.h"
}

#define PROLOG fprintf(stream, "package main\n")

#define OPENFUN(name)         \
	fprintf(stream, "func "); \
	fprintf(stream, name);    \
	fprintf(stream, "()() {\n")

#define CLOSEFUN fprintf(stream, "\n}\n")

#define TESTVAL(val)                                      \
	do {                                                  \
		rewind(stream);                                   \
		if (scanner_scan(stream, tklist)) {               \
			fprintf(stderr, "ERROR: Lexical analysis\n"); \
		} else {                                          \
			EXPECT_EQ(parser_parse(tklist), val);         \
		}                                                 \
	} while (0)

class SyntaxTest : public testing::Test
{
public:
	FILE *stream;
	tklist_t tklist;

	virtual void SetUp() {
		stream = tmpfile();
		tklist = tklist_init();
	}

	virtual void TearDown() {
		tklist_free(tklist);
		fclose(stream);
	}
};

#endif // !IFJ_TEST_SYNTAX_H
