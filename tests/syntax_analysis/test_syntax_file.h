#ifndef IFJ_TEST_SYNTAX_FILE_H
#define IFJ_TEST_SYNTAX_FILE_H

#include <cstdio>
#include <gtest/gtest.h>
#include <filesystem>

extern "C" {
#include "error.h"
#include "tokens.h"
#include "scanner.h"
#include "parser.h"
}

class SyntaxTestFile : public testing::Test
{
public:
	FILE *stream;
	tklist_t tklist;

	void SetUp(const std::string &path, const std::string &filename) {
		tklist = tklist_init();
		std::string filepath = path + "/" + filename;
		stream = fopen(filepath.c_str(), "r");
		if (stream == nullptr) {
			std::cout << "Non-existent file: " << std::filesystem::current_path().c_str() << "/" << filepath << std::endl;
		} else if (scanner_scan(stream, tklist) != EXIT_SUCCESS) {
			std::cout << "ERROR: Lexical analysis\n";
		}
	}

	virtual void TearDown() {
		tklist_free(tklist);
		if (stream != nullptr) {
			fclose(stream);
			stream = nullptr;
		}
	}
};

#endif // !IFJ_TEST_SYNTAX_FILE_H
