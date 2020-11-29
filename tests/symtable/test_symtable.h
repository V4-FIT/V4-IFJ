#ifndef TEST_SYMTABLE_H
#define TEST_SYMTABLE_H

#include <gtest/gtest.h>

extern "C" {
#include "symtable.h"
}

#define TK(_name, _type)              \
	token_t _name = new struct token; \
	_name->type = _type;

#define TK_ID(_name, _param)          \
	token_t _name = new struct token; \
	_name->type = TK_IDENTIFIER;      \
	_name->param.s = _param;

class SymtableTest : public testing::Test
{
public:
	symtable_t symtable;

	virtual void SetUp() {
		symtable = symtable_init();
	}

	virtual void TearDown() {
		symtable_free(symtable);
	}
};

using SymtableDeathTest = SymtableTest;

#endif // !TEST_SYMTABLE_H
