#ifndef IFJ_TEST_TESTSUITE_H
#define IFJ_TEST_TESTSUITE_H

#include <cstdio>
#include <gtest/gtest.h>
#include <filesystem>

extern "C" {
#include "error.h"
#include "tokens.h"
#include "scanner.h"
#include "parser.h"
}

class TestSuiteLex : public testing::Test
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
	}

	virtual void TearDown() {
		tklist_free(tklist);
		if (stream != nullptr) {
			fclose(stream);
			stream = nullptr;
		}
	}
};

class TestSuiteSyn : public testing::Test
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
	}

	virtual void TearDown() {
		tklist_free(tklist);
		if (stream != nullptr) {
			fclose(stream);
			stream = nullptr;
		}
	}
};

class TestSuiteSem : public testing::Test
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
	}

	virtual void TearDown() {
		tklist_free(tklist);
		if (stream != nullptr) {
			fclose(stream);
			stream = nullptr;
		}
	}
};

#endif // !IFJ_TEST_TESTSUITE_H
