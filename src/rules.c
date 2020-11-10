#include "rules.h"
#include "error.h"
#include "rulemacros.h"

////// Forward declarations

static int rule_program(scanner_t scanner);
static int rule_prolog(scanner_t scanner);
static int rule_functions(scanner_t scanner);
static int rule_function_n(scanner_t scanner); // 2x
static int rule_function(scanner_t scanner);
static int rule_params(scanner_t scanner);
static int rule_param_n(scanner_t scanner); // 2x
static int rule_param(scanner_t scanner); // 2x
static int rule_returnTypes(scanner_t scanner);
static int rule_type_n(scanner_t scanner); // 2x
static int rule_type(scanner_t scanner); // 4x
static int rule_statements(scanner_t scanner);
// Statements -> ε
static int rule_statement_n(scanner_t scanner); // 2x
static int rule_statement(scanner_t scanner); // 6x
static int rule_var_define(scanner_t scanner);
// Var_define -> ε
static int rule_assignment(scanner_t scanner);
// Assignment -> ε
static int rule_ids(scanner_t scanner);
static int rule_id_n(scanner_t scanner); // 2x
static int rule_expressions(scanner_t scanner);
static int rule_expression_n(scanner_t scanner); // 2x
static int rule_assignOp(scanner_t scanner); // 5x
static int rule_conditionals(scanner_t scanner);
static int rule_conditional_n(scanner_t scanner); // 3x
static int rule_conditional(scanner_t scanner); // 2x
static int rule_iterative(scanner_t scanner);
static int rule_functionCall(scanner_t scanner); // 2x
static int rule_funParams(scanner_t scanner);
static int rule_funParam_n(scanner_t scanner); // 2x
static int rule_funParam(scanner_t scanner); // 2x
static int rule_term(scanner_t scanner); // 2x
static int rule_literal(scanner_t scanner); // 2x
static int rule_return(scanner_t scanner);

////// Root

int rule_root(scanner_t scanner) {
	int ret = rule_program(scanner);
	return (ret == EPS_RETVAL) ? ERROR_SYN : ret;
}

////// Actual rule definitions

/// 1
static int rule_program(scanner_t scanner) {
	// Program -> Prolog Functions eof
	REQUIRE_NONTERMINAL(rule_prolog);
	REQUIRE_NONTERMINAL(rule_functions);
	REQUIRE_TERMINAL(TK_EOF);

	return EXIT_SUCCESS;
}

/// 2
static int rule_prolog(scanner_t scanner) {
	// Prolog -> package main eol
	REQUIRE_TERMINAL(TK_KEYW_PACKAGE);
	REQUIRE_TERMINAL(TK_KEYW_MAIN);
	REQUIRE_TERMINAL(TK_EOL);

	return EXIT_SUCCESS;
}

/// 3
static int rule_functions(scanner_t scanner) {
	// Functions -> Function Function_n
	REQUIRE_NONTERMINAL(rule_function);
	REQUIRE_NONTERMINAL(rule_function_n);

	return EXIT_SUCCESS;
}

/// 4
/// 5
static int rule_function_n(scanner_t scanner) {
	// Function_n -> Function Function_n
	// Function_n -> ε
	EXPECT_NONTERMINAL(rule_function);
	REQUIRE_NONTERMINAL(rule_function_n);

	return EXIT_SUCCESS;
}

/// 6
static int rule_function(scanner_t scanner) {
	// Function -> func Id ( Params ) ( ReturnTypes ) { Statements }
	REQUIRE_TERMINAL(TK_KEYW_FUNC);
	REQUIRE_TERMINAL(TK_IDENTIFIER);

	REQUIRE_TERMINAL(TK_L_PARENTHESIS);
	REQUIRE_NONTERMINAL(rule_params);
	REQUIRE_TERMINAL(TK_R_PARENTHESIS);

	REQUIRE_TERMINAL(TK_L_PARENTHESIS);
	REQUIRE_NONTERMINAL(rule_returnTypes);
	REQUIRE_TERMINAL(TK_R_PARENTHESIS);

	REQUIRE_TERMINAL(TK_L_CURLY);
	REQUIRE_NONTERMINAL(rule_statements);
	REQUIRE_TERMINAL(TK_R_CURLY);

	return EXIT_SUCCESS;
}

/// 7
static int rule_params(scanner_t scanner) {
	// Params -> Param Param_n
	REQUIRE_NONTERMINAL(rule_param);
	REQUIRE_NONTERMINAL(rule_param_n);

	return EXIT_SUCCESS;
}

// {optional Param_n?}
// Params -> Param Param_n // Param [ε] - valid // ignore epscode
// Param_n -> ε // return eps
// Param_n -> , Param Param_n

// vs

// {not <- required Type?}
// Param -> Type id // [ε] id - invalid // convert epscode to error
// Type -> ε // return eps
// Type -> float64
// Type -> int
// Type -> string
// Type -> bool


/// 8
/// 9
static int rule_param_n(scanner_t scanner) {
	// Param_n -> , Param Param_n
	// Param_n -> ε
	EXPECT_TERMINAL(TK_COMMA);
	REQUIRE_NONTERMINAL(rule_param);
	REQUIRE_NONTERMINAL(rule_param_n);

	return EXIT_SUCCESS;
}

/// 10
/// 11
static int rule_param(scanner_t scanner) {
	// Param -> Type id
	// Param -> ε
	EXPECT_NONTERMINAL(rule_type);
	REQUIRE_TERMINAL(TK_IDENTIFIER);

	return EXIT_SUCCESS;
}

/// 12
static int rule_returnTypes(scanner_t scanner) {
	// ReturnTypes ->  Type Type_n
	REQUIRE_NONTERMINAL(rule_type);
	REQUIRE_NONTERMINAL(rule_type_n);

	return EXIT_SUCCESS;
}

/// 13
/// 14
static int rule_type_n(scanner_t scanner) {
	// Type_n -> , Type Type_n
	// Type_n -> ε
	EXPECT_TERMINAL(TK_COMMA);
	REQUIRE_NONTERMINAL(rule_type);
	REQUIRE_NONTERMINAL(rule_type_n);

	return EXIT_SUCCESS;
}

/// 15
/// 16
/// 17
/// 18
static int rule_type(scanner_t scanner) { // 4x must return EPS
	// Type -> ε
	// Type -> float64
	// Type -> int
	// Type -> string
	int validKW[3] = {TK_KEYW_FLOAT64, TK_KEYW_INT, TK_KEYW_STRING};
	EXPECT_TERMINAL_SET(validKW, 3);

	return EXIT_SUCCESS;
}

// TODO
static int rule_statements(scanner_t scanner);
// Statements -> ε
static int rule_statement_n(scanner_t scanner); // 2x
static int rule_statement(scanner_t scanner); // 6x
static int rule_var_define(scanner_t scanner);
// Var_define -> ε
static int rule_assignment(scanner_t scanner);
// Assignment -> ε
static int rule_ids(scanner_t scanner);
static int rule_id_n(scanner_t scanner); // 2x
static int rule_expressions(scanner_t scanner);
static int rule_expression_n(scanner_t scanner); // 2x
static int rule_assignOp(scanner_t scanner); // 5x
static int rule_conditionals(scanner_t scanner);
static int rule_conditional_n(scanner_t scanner); // 3x
static int rule_conditional(scanner_t scanner); // 2x
static int rule_iterative(scanner_t scanner);
static int rule_functionCall(scanner_t scanner); // 2x
static int rule_funParams(scanner_t scanner);
static int rule_funParam_n(scanner_t scanner); // 2x
static int rule_funParam(scanner_t scanner); // 2x
static int rule_term(scanner_t scanner); // 2x
static int rule_literal(scanner_t scanner); // 2x
static int rule_return(scanner_t scanner);
