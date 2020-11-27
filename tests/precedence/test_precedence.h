#ifndef IFJ_TEST_PRECEDENCE_H
#define IFJ_TEST_PRECEDENCE_H

#include <cstdio>
#include <gtest/gtest.h>

extern "C" {
#include "error.h"
#include "tokens.h"
#include "precedence.h"
#include "scanner.h"
}


#define TESTVAL(val)                                         \
	do {                                                     \
		rewind(stream);                                      \
		scanner_t scanner = scanner_init(stream);            \
		if (scanner == NULL) {                               \
			fprintf(stderr, "ERROR: scanner aloc failed\n"); \
		} else {                                             \
			scanner_next_token(scanner);                     \
			EXPECT_EQ(parse_expr(scanner), val);             \
		}                                                    \
	} while (0)

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
