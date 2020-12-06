#include "test_testsuite.h"

//// expression

TEST_F(TestSuiteSyn, comment_nested) {
	SetUp("go_files/syn/expression", "comment_nested.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, expr_arithm) {
	SetUp("go_files/syn/expression", "expr_arithm.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, expr_assign) {
	SetUp("go_files/syn/expression", "expr_assign.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, expr_bad_encapsulate) {
	SetUp("go_files/syn/expression", "expr_bad_encapsulate.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, expr_comp) {
	SetUp("go_files/syn/expression", "expr_comp.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, expr_encapsulate) {
	SetUp("go_files/syn/expression", "expr_encapsulate.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, expr_float) {
	SetUp("go_files/syn/expression", "expr_float.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, expr_id) {
	SetUp("go_files/syn/expression", "expr_id.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, expr_int) {
	SetUp("go_files/syn/expression", "expr_int.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, expr_invalid_operator) {
	SetUp("go_files/syn/expression", "expr_invalid_operator.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, expr_string) {
	SetUp("go_files/syn/expression", "expr_string.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, expr_unary) {
	SetUp("go_files/syn/expression", "expr_unary.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS); // Extension: UNARY
}

//// function

TEST_F(TestSuiteSyn, bad_function_kw) {
	SetUp("go_files/syn/function", "bad_function_kw.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, double_function_id) {
	SetUp("go_files/syn/function", "double_function_id.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, empty_returns) {
	SetUp("go_files/syn/function", "empty_returns.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, eol_in_function_1) {
	SetUp("go_files/syn/function", "eol_in_function_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, eol_in_function_2) {
	SetUp("go_files/syn/function", "eol_in_function_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, function_missing_spaces) {
	SetUp("go_files/syn/function", "function_missing_spaces.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, function_without_body) {
	SetUp("go_files/syn/function", "function_without_body.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, multiple_blocks) {
	SetUp("go_files/syn/function", "multiple_blocks.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, nested_functions) {
	SetUp("go_files/syn/function", "nested_functions.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, no_param_list) {
	SetUp("go_files/syn/function", "no_param_list.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, no_params_returns) {
	SetUp("go_files/syn/function", "no_params_returns.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, params) {
	SetUp("go_files/syn/function", "params.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, params_returns) {
	SetUp("go_files/syn/function", "params_returns.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, params_with_value) {
	SetUp("go_files/syn/function", "params_with_value.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, params_without_coma) {
	SetUp("go_files/syn/function", "params_without_coma.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, params_without_id) {
	SetUp("go_files/syn/function", "params_without_id.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, params_without_type) {
	SetUp("go_files/syn/function", "params_without_type.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, returns) {
	SetUp("go_files/syn/function", "returns.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, returns_with_id) {
	SetUp("go_files/syn/function", "returns_with_id.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, returns_without_coma) {
	SetUp("go_files/syn/function", "returns_without_coma.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, switched_params_returns) {
	SetUp("go_files/syn/function", "switched_params_returns.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, unending_function) {
	SetUp("go_files/syn/function", "unending_function.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

// statement

TEST_F(TestSuiteSyn, assign) {
	SetUp("go_files/syn/statement", "assign.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, assign_multiple) {
	SetUp("go_files/syn/statement", "assign_multiple.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, assign_without_coma) {
	SetUp("go_files/syn/statement", "assign_without_coma.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, assign_without_expr) {
	SetUp("go_files/syn/statement", "assign_without_expr.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, assign_without_id) {
	SetUp("go_files/syn/statement", "assign_without_id.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, bad_call) {
	SetUp("go_files/syn/statement", "bad_call.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, bad_call_assign) {
	SetUp("go_files/syn/statement", "bad_call_assign.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, call) {
	SetUp("go_files/syn/statement", "call.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, call_assign) {
	SetUp("go_files/syn/statement", "call_assign.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, call_assign_multiple) {
	SetUp("go_files/syn/statement", "call_assign_multiple.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, call_no_term) {
	SetUp("go_files/syn/statement", "call_no_term.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, call_params) {
	SetUp("go_files/syn/statement", "call_params.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, call_with_types) {
	SetUp("go_files/syn/statement", "call_with_types.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, def) {
	SetUp("go_files/syn/statement", "def.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, def_without_expr) {
	SetUp("go_files/syn/statement", "def_without_expr.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, def_without_id) {
	SetUp("go_files/syn/statement", "def_without_id.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, for_statement) {
	SetUp("go_files/syn/statement", "for.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, for_assign) {
	SetUp("go_files/syn/statement", "for_assign.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, for_bad_assign) {
	SetUp("go_files/syn/statement", "for_bad_assign.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, for_bad_def) {
	SetUp("go_files/syn/statement", "for_bad_def.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, for_def) {
	SetUp("go_files/syn/statement", "for_def.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, for_def_assign) {
	SetUp("go_files/syn/statement", "for_def_assign.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, for_no_semicolons) {
	SetUp("go_files/syn/statement", "for_no_semicolons.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, for_with_eol) {
	SetUp("go_files/syn/statement", "for_with_eol.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, for_without_eol) {
	SetUp("go_files/syn/statement", "for_without_eol.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, for_without_expr) {
	SetUp("go_files/syn/statement", "for_without_expr.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, if_statement) {
	SetUp("go_files/syn/statement", "if.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, if_else_with_expr) {
	SetUp("go_files/syn/statement", "if_else_with_expr.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, if_with_eol) {
	SetUp("go_files/syn/statement", "if_with_eol.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, if_without_else) {
	SetUp("go_files/syn/statement", "if_without_else.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS); // Extension: BOOLTHEN
}

TEST_F(TestSuiteSyn, if_without_eol) {
	SetUp("go_files/syn/statement", "if_without_eol.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, if_without_expr) {
	SetUp("go_files/syn/statement", "if_without_expr.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, if_without_then) {
	SetUp("go_files/syn/statement", "if_without_then.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, multiple_eols) {
	SetUp("go_files/syn/statement", "multiple_eols.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, no_eols_statements) {
	SetUp("go_files/syn/statement", "no_eols_statements.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, return_statement) {
	SetUp("go_files/syn/statement", "return.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS); // FAILED
}

TEST_F(TestSuiteSyn, return_value) {
	SetUp("go_files/syn/statement", "return_value.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, assign_unary_1) {
	SetUp("go_files/syn/statement", "assign_unary_1.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, assign_unary_2) {
	SetUp("go_files/syn/statement", "assign_unary_2.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

//// top

TEST_F(TestSuiteSyn, bad_header) {
	SetUp("go_files/syn/top", "bad_header.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, case_sensitive_kw) {
	SetUp("go_files/syn/top", "case_sensitive_kw.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, ending_eol) {
	SetUp("go_files/syn/top", "ending_eol.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, functions) {
	SetUp("go_files/syn/top", "functions.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, gloabl_variable) {
	SetUp("go_files/syn/top", "gloabl_variable.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, no_eol) {
	SetUp("go_files/syn/top", "no_eol.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}

TEST_F(TestSuiteSyn, no_function) {
	SetUp("go_files/syn/top", "no_function.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	int res = parser_parse(tklist);
	EXPECT_TRUE(res == ERROR_SYN || res == ERROR_DEFINITION);
}

TEST_F(TestSuiteSyn, no_header) {
	SetUp("go_files/syn/top", "no_header.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), ERROR_SYN);
}

TEST_F(TestSuiteSyn, starting_eol) {
	SetUp("go_files/syn/top", "starting_eol.go");
	ASSERT_NE(stream, nullptr);
	EXPECT_EQ(scanner_scan(stream, tklist), EXIT_SUCCESS);
	EXPECT_EQ(parser_parse(tklist), EXIT_SUCCESS);
}
