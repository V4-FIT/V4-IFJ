#ifndef IFJ_TEST_SYNTAX_FILE_H
#define IFJ_TEST_SYNTAX_FILE_H

#include <cstdio>
#include <gtest/gtest.h>

extern "C" {
#include "error.h"
#include "tokens.h"
#include "parser.h"
}

class SyntaxTestFile : public testing::Test
{
public:
	FILE *stream;
	char *pwd = get_current_dir_name();
	bool in_subdir;

	void SetUp(const std::string &path, const std::string &filename) {
		ASSERT_NE(pwd, nullptr); // <- this shouldn't happen
		in_subdir = chdir(path.c_str()) == 0;

		stream = fopen(filename.c_str(), "r");
		if (stream == nullptr) {
			std::cout << "Non-existent file: " << pwd << "/" << path << "/" << filename << std::endl;
		}
	}

	virtual void TearDown() {
		if (stream != nullptr) {
			fclose(stream);
			stream = nullptr;
		}
		if (pwd != nullptr && in_subdir) {
			in_subdir = chdir(pwd) != 0;
		}
	}
};

#endif // !IFJ_TEST_SYNTAX_FILE_H
