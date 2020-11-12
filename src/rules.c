#include "rules.h"

#include <assert.h>

#include "rulemacros.h"

////// Forward declarations

static int rule_program(scanner_t scanner);
static int rule_prolog(scanner_t scanner);
static int rule_functions(scanner_t scanner);
static int rule_function(scanner_t scanner);
static int rule_param_list(scanner_t scanner);
static int rule_param_n(scanner_t scanner);
static int rule_param(scanner_t scanner);
static int rule_return_list(scanner_t scanner);
static int rule_return_n(scanner_t scanner);
static int rule_type_n(scanner_t scanner);
static int rule_typename(scanner_t scanner);
static int rule_statements(scanner_t scanner);
static int rule_statement(scanner_t scanner);
static int rule_def_ass_call(scanner_t scanner);
static int rule_ass_call(scanner_t scanner);
static int rule_def_ass_call2(scanner_t scanner);
static int rule_funcall(scanner_t sacanner);
static int rule_var_define(scanner_t scanner);
static int rule_conditionals(scanner_t scanner);
static int rule_conditional_n(scanner_t scanner);
static int rule_else(scanner_t scanner);
static int rule_conditional(scanner_t scanner);
static int rule_iterative(scanner_t scanner);
static int rule_assignment(scanner_t scanner);
static int rule_ids(scanner_t scanner);
static int rule_id_n(scanner_t scanner);
static int rule_id(scanner_t scanner);
static int rule_assignOp(scanner_t scanner);
static int rule_exprs_funCall(scanner_t scanner);
static int rule_functionCall(scanner_t scanner);
static int rule_Arguments(scanner_t scanner);
static int rule_Argument_n(scanner_t scanner);
static int rule_Argument(scanner_t scanner);
static int rule_return(scanner_t scanner);
static int rule_expressions(scanner_t scanner);
static int rule_expression_n(scanner_t scanner);
static int rule_expression(scanner_t scanner);
static int rule_literal(scanner_t scanner);
static int rule_eol_opt(scanner_t sacnner);


////// Root

int rule_root(scanner_t scanner) {
	TK_NEXT();
	EXECUTE_RULE(rule_program);

	return EXIT_SUCCESS;
}

////// Actual rule definitions

/// 1
int rule_program(scanner_t scanner) {
	// Program -> Prolog Functions eof
	EXECUTE_RULE(rule_prolog);
	TRY_EXECUTE_RULE(rule_functions, TK_KEYW_FUNC);
	TK_NEXT_IF(TK_EOF);

	return EXIT_SUCCESS;
}

/// 2
int rule_prolog(scanner_t scanner) {
	// Prolog -> package main eol
	TK_NEXT_IF(TK_KEYW_PACKAGE);
	TK_NEXT_IF(TK_KEYW_MAIN);
	TK_NEXT_IF(TK_EOL);

	return EXIT_SUCCESS;
}

/// 3
int rule_functions(scanner_t scanner) {
	// Functions -> Function Function_n
	EXECUTE_RULE(rule_function);
	TRY_EXECUTE_RULE(rule_functions, TK_KEYW_FUNC);

	return EXIT_SUCCESS;
}

/// 4
int rule_function(scanner_t scanner) {
	// Function -> func Id ( Params ) ReturnTypes { Statements }
	TK_NEXT_IF(TK_KEYW_FUNC);

	TK_NEXT_IF_SET(TK_IDENTIFIER, TK_KEYW_MAIN);

	TK_NEXT_IF(TK_L_PARENTHESIS);
	TRY_EXECUTE_RULE(rule_params, TK_KEYW_INT, TK_KEYW_FLOAT64, TK_KEYW_STRING, TK_KEYW_BOOL);
	TK_NEXT_IF(TK_R_PARENTHESIS);

	TRY_EXECUTE_RULE(rule_returnTypes, TK_L_PARENTHESIS);

	TK_NEXT_IF(TK_L_CURLY);
	TRY_EXECUTE_RULE(rule_statements, TK_IDENTIFIER, TK_KEYW_IF, TK_KEYW_FOR, TK_KEYW_RETURN);
	TK_NEXT_IF(TK_R_CURLY);

	return EXIT_SUCCESS;
}

/// 5
int rule_param_list(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 6
int rule_param_n(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 7
int rule_param(scanner_t scanner) {
	return EXIT_SUCCESS;
}


/// 8
int rule_return_list(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 9
int rule_return_n(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 10
int rule_type_n(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 11
int rule_typename(scanner_t scanner) {
	return EXIT_SUCCESS;
}


/// 12
int rule_statements(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 13
int rule_statement(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 14
int rule_def_ass_call(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 15
int rule_ass_call(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 16
int rule_def_ass_call2(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 17
int rule_funcall(scanner_t sacanner) {
	return EXIT_SUCCESS;
}

/// 18
int rule_var_define(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 19
int rule_conditionals(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 20
int rule_conditional_n(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 21
int rule_else(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 22
int rule_conditional(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 23
int rule_iterative(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 24
int rule_assignment(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 25
int rule_ids(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 26
int rule_id_n(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 27
int rule_id(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 28
int rule_assignOp(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 29
int rule_exprs_funCall(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 30
int rule_functionCall(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 31
int rule_Arguments(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 32
int rule_Argument_n(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 33
int rule_Argument(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 34
int rule_return(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 35
int rule_expressions(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 36
int rule_expression_n(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 37
int rule_expression(scanner_t scanner) {
	return EXIT_SUCCESS;
}

/// 38
int rule_literal(scanner_t scanner) {
	return EXIT_SUCCESS;
}


/// 39
int rule_eol_opt(scanner_t sacnner) {
	return EXIT_SUCCESS;
}



