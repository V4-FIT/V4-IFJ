#ifndef IFJ_TEST_TOKEN_LIST_H
#define IFJ_TEST_TOKEN_LIST_H

#include <cstdio>
#include <gtest/gtest.h>
#include <filesystem>

extern "C" {
#include "error.h"
#include "tokens.h"
#include "scanner.h"
}

class TokenListTestFile : public testing::Test
{
public:
	FILE *stream;
	tklist_t tklist;

	void SetUp(const std::string &path, const std::string &filename) {
		std::string filepath = path + "/" + filename;
		stream = fopen(filepath.c_str(), "r");
		if (stream == nullptr) {
			std::cout << "Non-existent file: " << std::filesystem::current_path().c_str() << "/" << filepath << std::endl;
		}
		tklist = tklist_init();
		scanner_scan(stream, tklist);
	}

	virtual void TearDown() {
		if (stream != nullptr) {
			fclose(stream);
			stream = nullptr;
		}
		tklist_free(tklist);
	}
};

#endif // !IFJ_TEST_TOKEN_LIST_H