#ifndef IFJ_COMMON_H
#define IFJ_COMMON_H

#include <cstdio>
#include <gtest/gtest.h>

extern "C" {
#include "error.h"
#include "scanner.h"
#include "tokens.h"
}

class ScannerTest : public testing::Test
{
public:
	FILE *stream;
	scanner_t scanner;
	token_t token = NULL;

	virtual void SetUp() {
		stream = tmpfile();
		scanner = scanner_init(stream);
		token = scanner_get_token_ptr(scanner);
	}

	virtual void TearDown() {
		fclose(stream);
		scanner_free(scanner);
	}
};

#endif // !IFJ_COMMON_H
