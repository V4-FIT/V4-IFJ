#include "test_testsuite.h"

//// builtin

TEST_F(TestSuiteSem, cast_1) {
	SetUp("go_files/sem/builtin", "cast_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, cast_2) {
	SetUp("go_files/sem/builtin", "cast_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, cast_3) {
	SetUp("go_files/sem/builtin", "cast_3.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, cast_4) {
	SetUp("go_files/sem/builtin", "cast_4.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	int res = parser_parse(tklist);
	EXPECT_TRUE(res == ERROR_TYPE_COMPAT || res == ERROR_PARAM || res == ERROR_SEM);
}

TEST_F(TestSuiteSem, chr_1) {
	SetUp("go_files/sem/builtin", "chr_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_PARAM);
}

TEST_F(TestSuiteSem, chr_2) {
	SetUp("go_files/sem/builtin", "chr_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, inputf_1) {
	SetUp("go_files/sem/builtin", "inputf_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, inputf_2) {
	SetUp("go_files/sem/builtin", "inputf_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	int res = parser_parse(tklist);
	EXPECT_TRUE(res == ERROR_PARAM || res == ERROR_SEM);
}

TEST_F(TestSuiteSem, inputi_1) {
	SetUp("go_files/sem/builtin", "inputi_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, inputi_2) {
	SetUp("go_files/sem/builtin", "inputi_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	int res = parser_parse(tklist);
	EXPECT_TRUE(res == ERROR_TYPE_COMPAT || res == ERROR_PARAM || res == ERROR_SEM);
}

TEST_F(TestSuiteSem, inputs_1) {
	SetUp("go_files/sem/builtin", "inputs_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, inputs_2) {
	SetUp("go_files/sem/builtin", "inputs_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	int res = parser_parse(tklist);
	EXPECT_TRUE(res == ERROR_TYPE_COMPAT || res == ERROR_PARAM || res == ERROR_SEM);
}

TEST_F(TestSuiteSem, len_1) {
	SetUp("go_files/sem/builtin", "len_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, len_2) {
	SetUp("go_files/sem/builtin", "len_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_PARAM);
}

TEST_F(TestSuiteSem, ord_1) {
	SetUp("go_files/sem/builtin", "ord_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, ord_2) {
	SetUp("go_files/sem/builtin", "ord_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_PARAM);
}

TEST_F(TestSuiteSem, ord_3) {
	SetUp("go_files/sem/builtin", "ord_3.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	int res = parser_parse(tklist);
	EXPECT_TRUE(res == ERROR_PARAM || res == ERROR_SEM);
}

TEST_F(TestSuiteSem, print_all) {
	SetUp("go_files/sem/builtin", "print_all.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, print_bad) {
	SetUp("go_files/sem/builtin", "print_bad.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	int res = parser_parse(tklist);
	EXPECT_TRUE(res == ERROR_PARAM || res == ERROR_SEM);
}

TEST_F(TestSuiteSem, print_empty) {
	SetUp("go_files/sem/builtin", "print_empty.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, print_float) {
	SetUp("go_files/sem/builtin", "print_float.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, print_int) {
	SetUp("go_files/sem/builtin", "print_int.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, print_string) {
	SetUp("go_files/sem/builtin", "print_string.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, substr_1) {
	SetUp("go_files/sem/builtin", "substr_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, substr_2) {
	SetUp("go_files/sem/builtin", "substr_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_PARAM);
}

TEST_F(TestSuiteSem, substr_3) {
	SetUp("go_files/sem/builtin", "cast_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_PARAM);
}

//// declaration

TEST_F(TestSuiteSem, multiple_functions) {
	SetUp("go_files/sem/declaration", "multiple_functions.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_DEFINITION);
}

TEST_F(TestSuiteSem, multiple_vars_1) {
	SetUp("go_files/sem/declaration", "multiple_vars_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_DEFINITION);
}

TEST_F(TestSuiteSem, multiple_vars_2) {
	SetUp("go_files/sem/declaration", "multiple_vars_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, multiple_vars_3) {
	SetUp("go_files/sem/declaration", "multiple_vars_3.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, multiple_vars_4) {
	SetUp("go_files/sem/declaration", "multiple_vars_4.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	int res = parser_parse(tklist);
	EXPECT_TRUE(res == ERROR_DEFINITION || res == ERROR_PARAM || res == ERROR_SEM);
}

TEST_F(TestSuiteSem, multiple_vars_5) {
	SetUp("go_files/sem/declaration", "multiple_vars_5.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, nondefined_var_1) {
	SetUp("go_files/sem/declaration", "nondefined_var_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_DEFINITION);
}

TEST_F(TestSuiteSem, nondefined_var_2) {
	SetUp("go_files/sem/declaration", "nondefined_var_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_DEFINITION);
}

TEST_F(TestSuiteSem, nondefined_var_3) {
	SetUp("go_files/sem/declaration", "nondefined_var_3.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_DEFINITION);
}

TEST_F(TestSuiteSem, nondefined_var_4) {
	SetUp("go_files/sem/declaration", "nondefined_var_4.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_DEFINITION);
}

//// expresion

TEST_F(TestSuiteSem, add_1) {
	SetUp("go_files/sem/expresion", "add_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, add_2) {
	SetUp("go_files/sem/expresion", "add_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, add_3) {
	SetUp("go_files/sem/expresion", "add_3.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, add_4) {
	SetUp("go_files/sem/expresion", "add_4.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, add_5) {
	SetUp("go_files/sem/expresion", "add_5.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT); // consider extensions
}

TEST_F(TestSuiteSem, add_6) {
	SetUp("go_files/sem/expresion", "add_6.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT); // consider extensions
}

TEST_F(TestSuiteSem, add_7) {
	SetUp("go_files/sem/expresion", "add_7.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, add_8) {
	SetUp("go_files/sem/expresion", "add_8.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, add_9) {
	SetUp("go_files/sem/expresion", "add_9.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, div_1) {
	SetUp("go_files/sem/expresion", "div_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, div_2) {
	SetUp("go_files/sem/expresion", "div_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, div_3) {
	SetUp("go_files/sem/expresion", "div_3.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, div_4) {
	SetUp("go_files/sem/expresion", "div_4.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, div_5) {
	SetUp("go_files/sem/expresion", "div_5.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, div_6) {
	SetUp("go_files/sem/expresion", "div_6.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, div_7) {
	SetUp("go_files/sem/expresion", "div_7.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, div_zero) {
	SetUp("go_files/sem/expresion", "div_zero.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_ZERO_DIV);
}

TEST_F(TestSuiteSem, div_zero_complex) {
	SetUp("go_files/sem/expresion", "div_zero_complex.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	int res = parser_parse(tklist);
	EXPECT_TRUE(res == EXIT_SUCCESS || res == ERROR_ZERO_DIV);
}

TEST_F(TestSuiteSem, eq_1) {
	SetUp("go_files/sem/expresion", "eq_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, eq_2) {
	SetUp("go_files/sem/expresion", "eq_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, eq_3) {
	SetUp("go_files/sem/expresion", "eq_3.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, eq_4) {
	SetUp("go_files/sem/expresion", "eq_4.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT); // consider extensions
}

TEST_F(TestSuiteSem, eq_5) {
	SetUp("go_files/sem/expresion", "eq_5.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, eq_6) {
	SetUp("go_files/sem/expresion", "eq_6.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, eq_7) {
	SetUp("go_files/sem/expresion", "eq_7.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, gt_1) {
	SetUp("go_files/sem/expresion", "gt_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, gt_2) {
	SetUp("go_files/sem/expresion", "gt_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, gt_3) {
	SetUp("go_files/sem/expresion", "gt_3.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, gt_4) {
	SetUp("go_files/sem/expresion", "gt_4.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT); // consider extensions
}

TEST_F(TestSuiteSem, gt_5) {
	SetUp("go_files/sem/expresion", "gt_5.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, gt_6) {
	SetUp("go_files/sem/expresion", "gt_6.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, gt_7) {
	SetUp("go_files/sem/expresion", "gt_7.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, gte_1) {
	SetUp("go_files/sem/expresion", "gte_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, gte_2) {
	SetUp("go_files/sem/expresion", "gte_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, gte_3) {
	SetUp("go_files/sem/expresion", "gte_3.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, gte_4) {
	SetUp("go_files/sem/expresion", "gte_4.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT); // consider extensions
}

TEST_F(TestSuiteSem, gte_5) {
	SetUp("go_files/sem/expresion", "gte_5.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, gte_6) {
	SetUp("go_files/sem/expresion", "gte_6.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, gte_7) {
	SetUp("go_files/sem/expresion", "gte_7.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, lt_1) {
	SetUp("go_files/sem/expresion", "lt_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, lt_2) {
	SetUp("go_files/sem/expresion", "lt_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, lt_3) {
	SetUp("go_files/sem/expresion", "lt_3.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, lt_4) {
	SetUp("go_files/sem/expresion", "lt_4.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT); // consider extension
}

TEST_F(TestSuiteSem, lt_5) {
	SetUp("go_files/sem/expresion", "lt_5.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, lt_6) {
	SetUp("go_files/sem/expresion", "lt_6.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, lt_7) {
	SetUp("go_files/sem/expresion", "lt_7.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, lte_1) {
	SetUp("go_files/sem/expresion", "lte_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, lte_2) {
	SetUp("go_files/sem/expresion", "lte_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, lte_3) {
	SetUp("go_files/sem/expresion", "lte_3.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, lte_4) {
	SetUp("go_files/sem/expresion", "lte_4.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT); // consider extensions
}

TEST_F(TestSuiteSem, lte_5) {
	SetUp("go_files/sem/expresion", "lte_5.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, lte_6) {
	SetUp("go_files/sem/expresion", "lte_6.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, lte_7) {
	SetUp("go_files/sem/expresion", "lte_7.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, mul_1) {
	SetUp("go_files/sem/expresion", "mul_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, mul_2) {
	SetUp("go_files/sem/expresion", "mul_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, mul_3) {
	SetUp("go_files/sem/expresion", "mul_3.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, mul_4) {
	SetUp("go_files/sem/expresion", "mul_4.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, mul_5) {
	SetUp("go_files/sem/expresion", "mul_5.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, mul_6) {
	SetUp("go_files/sem/expresion", "mul_6.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, mul_7) {
	SetUp("go_files/sem/expresion", "mul_7.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, neq_1) {
	SetUp("go_files/sem/expresion", "neq_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, neq_2) {
	SetUp("go_files/sem/expresion", "neq_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, neq_3) {
	SetUp("go_files/sem/expresion", "neq_3.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, neq_4) {
	SetUp("go_files/sem/expresion", "neq_4.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT); // consider extensions
}

TEST_F(TestSuiteSem, neq_5) {
	SetUp("go_files/sem/expresion", "neq_5.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, neq_6) {
	SetUp("go_files/sem/expresion", "neq_6.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, neq_7) {
	SetUp("go_files/sem/expresion", "neq_7.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, sub_1) {
	SetUp("go_files/sem/expresion", "sub_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, sub_2) {
	SetUp("go_files/sem/expresion", "sub_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, sub_3) {
	SetUp("go_files/sem/expresion", "sub_3.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, sub_4) {
	SetUp("go_files/sem/expresion", "sub_4.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, sub_5) {
	SetUp("go_files/sem/expresion", "sub_5.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, sub_6) {
	SetUp("go_files/sem/expresion", "sub_6.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, sub_7) {
	SetUp("go_files/sem/expresion", "sub_7.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

//// other

TEST_F(TestSuiteSem, bad_prolog_id) {
	SetUp("go_files/sem/other", "bad_prolog_id.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	int res = parser_parse(tklist);
	EXPECT_TRUE(res == ERROR_SYN || res == ERROR_SEM);
}

TEST_F(TestSuiteSem, case_sensitive) {
	SetUp("go_files/sem/other", "case_sensitive.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	int res = parser_parse(tklist);
	EXPECT_TRUE(res == ERROR_DEFINITION || res == ERROR_TYPE_INIT);
}

TEST_F(TestSuiteSem, main_params) {
	SetUp("go_files/sem/other", "main_params.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_PARAM);
}

TEST_F(TestSuiteSem, main_params_returns) {
	SetUp("go_files/sem/other", "main_params_returns.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_PARAM);
}

TEST_F(TestSuiteSem, main_returns) {
	SetUp("go_files/sem/other", "main_returns.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_PARAM);
}

TEST_F(TestSuiteSem, no_main) {
	SetUp("go_files/sem/other", "no_main.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_DEFINITION);
}

//// statement

TEST_F(TestSuiteSem, assign_dont_match_1) {
	SetUp("go_files/sem/statement", "assign_dont_match_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SEM);
}

TEST_F(TestSuiteSem, assign_dont_match_2) {
	SetUp("go_files/sem/statement", "assign_dont_match_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SEM);
}

TEST_F(TestSuiteSem, assign_dont_match_3) {
	SetUp("go_files/sem/statement", "assign_dont_match_3.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	int res = parser_parse(tklist);
	EXPECT_TRUE(res == ERROR_TYPE_COMPAT || res == ERROR_SEM);
}

TEST_F(TestSuiteSem, bad_return_1) {
	SetUp("go_files/sem/statement", "bad_return_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_PARAM);
}

TEST_F(TestSuiteSem, bad_return_2) {
	SetUp("go_files/sem/statement", "bad_return_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_PARAM);
}

TEST_F(TestSuiteSem, bad_return_3) {
	SetUp("go_files/sem/statement", "bad_return_3.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_PARAM);
}

TEST_F(TestSuiteSem, bad_return_4) {
	SetUp("go_files/sem/statement", "bad_return_4.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_PARAM);
}

TEST_F(TestSuiteSem, call_bad_params_1) {
	SetUp("go_files/sem/statement", "call_bad_params_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_PARAM);
}

TEST_F(TestSuiteSem, call_bad_params_2) {
	SetUp("go_files/sem/statement", "call_bad_params_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_PARAM);
}

TEST_F(TestSuiteSem, call_bad_return_1) {
	SetUp("go_files/sem/statement", "call_bad_return_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	int res = parser_parse(tklist);
	EXPECT_TRUE(res == ERROR_PARAM || res == ERROR_SEM);
}

TEST_F(TestSuiteSem, call_bad_return_2) {
	SetUp("go_files/sem/statement", "call_bad_return_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	int res = parser_parse(tklist);
	EXPECT_TRUE(res == ERROR_TYPE_COMPAT || res == ERROR_PARAM || res == ERROR_SEM);
}

TEST_F(TestSuiteSem, call_bad_return_3) {
	SetUp("go_files/sem/statement", "call_bad_return_3.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	int res = parser_parse(tklist);
	EXPECT_TRUE(res == ERROR_PARAM || res == ERROR_SEM);
}

TEST_F(TestSuiteSem, call_undefined) {
	SetUp("go_files/sem/statement", "call_undefined.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_DEFINITION);
}

TEST_F(TestSuiteSem, for_nonbool) {
	SetUp("go_files/sem/statement", "for_nonbool.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, for_scope_1) {
	SetUp("go_files/sem/statement", "for_scope_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, for_scope_2) {
	SetUp("go_files/sem/statement", "for_scope_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_DEFINITION);
}

TEST_F(TestSuiteSem, if_nonbool) {
	SetUp("go_files/sem/statement", "if_nonbool.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, no_return) {
	SetUp("go_files/sem/statement", "no_return.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_PARAM);
}

TEST_F(TestSuiteSem, underscore_usage_1) {
	SetUp("go_files/sem/statement", "underscore_usage_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, underscore_usage_2) {
	SetUp("go_files/sem/statement", "underscore_usage_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, underscore_usage_3) {
	SetUp("go_files/sem/statement", "underscore_usage_3.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	int res = parser_parse(tklist);
	EXPECT_TRUE(res == ERROR_SYN || res == ERROR_DEFINITION);
}

TEST_F(TestSuiteSem, underscore_usage_4) {
	SetUp("go_files/sem/statement", "underscore_usage_4.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	int res = parser_parse(tklist);
	EXPECT_TRUE(res == ERROR_SYN || res == ERROR_DEFINITION);
}

//// extensions

TEST_F(TestSuiteSem, unary_1) {
	SetUp("go_files/sem/extensions", "unary_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, unary_2) {
	SetUp("go_files/sem/extensions", "unary_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, unary_3) {
	SetUp("go_files/sem/extensions", "unary_3.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, unary_4) {
	SetUp("go_files/sem/extensions", "unary_4.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, unary_5) {
	SetUp("go_files/sem/extensions", "unary_5.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, unary_6) {
	SetUp("go_files/sem/extensions", "unary_6.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, unary_7) {
	SetUp("go_files/sem/extensions", "unary_7.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, or_1) {
	SetUp("go_files/sem/extensions", "or_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, or_2) {
	SetUp("go_files/sem/extensions", "or_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}

TEST_F(TestSuiteSem, and_1) {
	SetUp("go_files/sem/extensions", "and_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSem, and_2) {
	SetUp("go_files/sem/extensions", "and_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_TYPE_COMPAT);
}
