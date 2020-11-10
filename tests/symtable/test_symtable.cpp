#include <gtest/gtest.h>

#include "test_symtable.h"

struct symtable
{
	flist_t tables;
};

TEST_F(SymtableTest, init) {
	ASSERT_NE(symtable, nullptr);
	ASSERT_NE(symtable->tables, nullptr);
}

TEST_F(SymtableDeathTest, exit_scope) {
	ASSERT_DEATH({
		symtable_exit_scope(symtable);
		symtable_exit_scope(symtable);
	},
				 "");
}

TEST_F(SymtableTest, enter_scope) {
	EXPECT_TRUE(symtable_enter_scope(symtable));
	EXPECT_TRUE(symtable_enter_scope(symtable));
}

TEST_F(SymtableTest, empty_has_functions) {
	TK_ID(token, "x");
	EXPECT_FALSE(symtable_has_symbol(symtable, token));
	EXPECT_FALSE(symtable_has_func(symtable, token));
	EXPECT_FALSE(symtable_has_var(symtable, token));

	delete token;
}

TEST_F(SymtableTest, insert) {
	TK_ID(token, "x");
	symbol_ref_t s_ref = symtable_insert(symtable, token, ST_VAR);
	ASSERT_TRUE(symbol_valid(s_ref));
	ASSERT_NE(s_ref.symbol, nullptr);

	EXPECT_TRUE(symtable_has_symbol(symtable, token));
	EXPECT_TRUE(symtable_has_var(symtable, token));
	EXPECT_FALSE(symtable_has_func(symtable, token));

	delete token;
}

TEST_F(SymtableTest, var_set_data) {
	TK_ID(token, "x");
	symbol_ref_t s_ref = symtable_insert(symtable, token, ST_VAR);
	ASSERT_TRUE(symbol_valid(s_ref));

	symbol_var_set_data(s_ref, {DT_INTEGER});
	EXPECT_EQ(s_ref.symbol->var.data_type, DT_INTEGER);

	delete token;
}

TEST_F(SymtableDeathTest, var_set_data) {
	TK_ID(token, "x");
	symbol_ref_t s_ref = symtable_insert(symtable, token, ST_FUNC);
	ASSERT_TRUE(symbol_valid(s_ref));

	ASSERT_DEBUG_DEATH(symbol_var_set_data(s_ref, {DT_INTEGER}), "");

	delete token;
}

TEST_F(SymtableTest, func_add_param_and_return) {
	TK_ID(token, "x");
	symbol_ref_t s_ref = symtable_insert(symtable, token, ST_FUNC);
	ASSERT_TRUE(symbol_valid(s_ref));

	ASSERT_TRUE(symbol_func_add_param(s_ref, DT_INTEGER));
	ASSERT_EQ(s_ref.symbol->func.param_count, 1);
	data_type_t *t = (data_type_t *)flist_front(s_ref.symbol->func.param_list);
	EXPECT_EQ(*t, DT_INTEGER);

	ASSERT_TRUE(symbol_func_add_return(s_ref, DT_INTEGER));
	ASSERT_EQ(s_ref.symbol->func.return_count, 1);
	t = (data_type_t *)flist_front(s_ref.symbol->func.return_list);
	EXPECT_EQ(*t, DT_INTEGER);

	delete token;
}

#ifndef NDEBUG
TEST_F(SymtableDeathTest, func_add_param_and_return) {
	TK_ID(token, "x");
	symbol_ref_t s_ref = symtable_insert(symtable, token, ST_VAR);
	ASSERT_TRUE(symbol_valid(s_ref));

	ASSERT_DEATH(symbol_func_add_param(s_ref, {DT_INTEGER}), "");
	ASSERT_DEATH(symbol_func_add_return(s_ref, {DT_INTEGER}), "");

	delete token;
}
#endif

TEST_F(SymtableTest, advanced) {
	TK_ID(tx, "x");
	TK_ID(ty, "y");
	TK_ID(tz, "z");
	symbol_ref_t ref;

	symbol_ref_t sx = symtable_insert(symtable, tx, ST_VAR);
	ASSERT_TRUE(symbol_valid(sx));

	symbol_ref_t sy = symtable_insert(symtable, ty, ST_VAR);
	ASSERT_TRUE(symbol_valid(sy));

	EXPECT_TRUE(symtable_enter_scope(symtable));
	{
		symbol_ref_t sxx = symtable_insert(symtable, tx, ST_VAR);
		ASSERT_TRUE(symbol_valid(sxx));

		ref = symtable_find(symtable, tx);
		ASSERT_TRUE(symbol_valid(ref));
		EXPECT_TRUE(hmap_it_eq(ref.it, sxx.it));
		EXPECT_FALSE(hmap_it_eq(ref.it, sx.it));
		EXPECT_TRUE(symbol_current_scope(ref));

		EXPECT_TRUE(symtable_enter_scope(symtable));
		{
			ref = symtable_find(symtable, tx);
			ASSERT_TRUE(symbol_valid(ref));
			EXPECT_TRUE(hmap_it_eq(ref.it, sxx.it));
			EXPECT_FALSE(hmap_it_eq(ref.it, sx.it));
			EXPECT_FALSE(symbol_current_scope(ref));

			ref = symtable_find(symtable, ty);
			ASSERT_TRUE(symbol_valid(ref));
			EXPECT_TRUE(hmap_it_eq(ref.it, sy.it));
			EXPECT_FALSE(symbol_current_scope(ref));

			symbol_ref_t syy = symtable_insert(symtable, ty, ST_VAR);
			ASSERT_TRUE(symbol_valid(syy));

			ref = symtable_find(symtable, ty);
			ASSERT_TRUE(symbol_valid(ref));
			EXPECT_TRUE(hmap_it_eq(ref.it, syy.it));
			EXPECT_FALSE(hmap_it_eq(ref.it, sy.it));
			EXPECT_TRUE(symbol_current_scope(ref));
		}
		symtable_exit_scope(symtable);

		ref = symtable_find(symtable, ty);
		ASSERT_TRUE(symbol_valid(ref));
		EXPECT_TRUE(hmap_it_eq(ref.it, sy.it));
		EXPECT_FALSE(symbol_current_scope(ref));

		symbol_ref_t sz = symtable_insert(symtable, tz, ST_VAR);
		ASSERT_TRUE(symbol_valid(ref));
	}
	symtable_exit_scope(symtable);

	ref = symtable_find(symtable, tx);
	ASSERT_TRUE(symbol_valid(ref));
	EXPECT_TRUE(hmap_it_eq(ref.it, sx.it));
	EXPECT_TRUE(symbol_current_scope(ref));

	ref = symtable_find(symtable, tz);
	ASSERT_FALSE(symbol_valid(ref));

	delete tx;
	delete ty;
	delete tz;
}
