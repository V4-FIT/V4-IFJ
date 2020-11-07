#ifndef IFJ_COMMON_H
#define IFJ_COMMON_H

#include <cstdio>
#include <gtest/gtest.h>

extern "C" {
#include "scanner.h"
#include "tokens.h"
}

class ScannerTest : public testing::Test {
public:
	FILE *stream;
	scanner_t scanner;
	token_t token;

	virtual void SetUp() {
		stream = tmpfile();
		scanner = scanner_init(stream);
		token = new struct Token;
	}

	virtual void TearDown() {
		fclose(stream);
		scanner_free(scanner);
		delete token;
	}
};

#endif // !IFJ_COMMON_H
