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
	FILE *stream = tmpfile();
	scanner_t scanner;
	token_t token;

	~ScannerTest() override {
		fclose(stream);
	}

	virtual void SetUp() {
		scanner = scanner_init(stream);
		token = new struct Token;
	}

	virtual void TearDown() {
		scanner_free(scanner);
		delete token;
	}
};

#endif // !IFJ_COMMON_H
