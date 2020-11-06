#ifndef IFJ_COMMON_H
#define IFJ_COMMON_H

extern "C" {
#include "scanner.h"
#include "tokens.h"
}

class ScannerTest : public testing::Test {
public:
	char* buffer = nullptr;
	size_t bufferSize = 0;
	FILE* stream = open_memstream(&buffer, &bufferSize);

	scanner_t scanner;
	token_t token;

	~ScannerTest() override {
		fclose(stream);
		free(buffer);
	}

	virtual void SetUp() {
		fflush(stream);
		scanner = scanner_init(stream);
		token = new struct Token;
	}

	virtual void TearDown() {
		scanner_free(scanner);
		delete token;
	}
};

#endif //IFJ_COMMON_H
