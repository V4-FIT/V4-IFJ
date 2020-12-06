#ifndef IFJ_TEST_PRECEDENCE_H
#define IFJ_TEST_PRECEDENCE_H

#include <cstdio>
#include <gtest/gtest.h>

extern "C" {
#include "error.h"
#include "tokens.h"
#include "precedence.h"
#include "scanner.h"
#include "parser.h"
}


#define TESTVAL(val)                                      \
	do {                                                  \
		rewind(stream);                                   \
		if (scanner_scan(stream, tklist)) {               \
			fprintf(stderr, "ERROR: Lexical analysis\n"); \
		} else {                                          \
			s_parser = parser_init(tklist);               \
			parser_setup(s_parser);                       \
			EXPECT_EQ(parse_expr(s_parser), val);         \
		}                                                 \
	} while (0)

class DISABLED_Precedence : public testing::Test
{
public:
	FILE *stream;
	tklist_t tklist;
	parser_t s_parser;

	virtual void SetUp() {
		stream = tmpfile();
		tklist = tklist_init();
	}

	virtual void TearDown() {
		parser_free(s_parser);
		tklist_free(tklist);
		fclose(stream);
	}
};

#endif // !IFJ_TEST_PRECEDENCE_H
