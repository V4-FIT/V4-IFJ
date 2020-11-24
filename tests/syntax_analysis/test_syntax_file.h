#ifndef IFJ_TEST_SYNTAX_FILE_H
#define IFJ_TEST_SYNTAX_FILE_H

#include <cstdio>
#include <gtest/gtest.h>
#include <filesystem>

extern "C" {
#include "error.h"
#include "tokens.h"
#include "parser.h"
}

class SyntaxTestFile : public testing::Test
{
public:
	FILE *stream;
	parser_t parser;

	void SetUp(const std::string &path, const std::string &filename) {
		std::string filepath = path + "/" + filename;
		stream = fopen(filepath.c_str(), "r");
		if (stream == nullptr) {
			std::cout << "Non-existent file: " << std::filesystem::current_path().c_str() << "/" << filepath << std::endl;
		}
		parser = parser_init(stream);
	}

	virtual void TearDown() {
		if (stream != nullptr) {
			fclose(stream);
			stream = nullptr;
		}
		parser_free(parser);
	}
};

#endif // !IFJ_TEST_SYNTAX_FILE_H
