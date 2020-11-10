#include "rules.h"
#include "error.h"

////// Defines

// for eps return a special return code
#define EPS_RETVAL 0xDEAD

////// Macros

#define REQUIRE_TERMINAL(_KEYW) \
do { \
    token_t _tmp_token = scanner_retrieve_token(scanner); \
    if (_tmp_token->type != _KEYW) { \
        return ERROR_SYN; \
    } \
} while(0)

// expects keyword, if not then accepts everything but immediately returns
#define EXPECT_TERMINAL(_KEYW) \
do { \
    token_t _tmp_token = scanner_retrieve_token(scanner); \
    if (_tmp_token->type != _KEYW) { \
        return EXIT_SUCCESS; \
    } \
} while(0)

// expects keyword, if not then immediately returns and notifies parent
// WARNING: the parent must implement EXPECT_NONTERMINAL
// omitting this warning could result in an invalid return value from the compiler!
#define EXPECT_TERMINAL_NOTIFYPARENT(_KEYW) \
do { \
    token_t _tmp_token = scanner_retrieve_token(scanner); \
    if (_tmp_token->type != _KEYW) { \
        return EPS_RETVAL; \
    } \
} while(0)

#define REQUIRE_NONTERMINAL(_SUBFUNC) \
do { \
    int _err_retval = _SUBFUNC(scanner); \
    if (_err_retval != EXIT_SUCCESS) { \
        return _err_retval; \
    } \
} while(0)

// accepts EPS from child - silently ignores child eps return value
// the children of this call should implement EXPECT_TERMINAL_NOTIFYPARENT
#define EXPECT_NONTERMINAL(_SUBFUNC) \
do { \
    int _err_retval = _SUBFUNC(scanner); \
    if (_err_retval != EXIT_SUCCESS && _err_retval != EPS_RETVAL) { \
        return _err_retval; \
    } \
} while(0)

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
	return rule_program(scanner);
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
	EXPECT_TERMINAL_NOTIFYPARENT(TK_KEYW_FUNC);
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

static int rule_type(scanner_t scanner) { // 4x must return EPS
	// Type -> ε
	// Type -> float64
	// Type -> int
	// Type -> string

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
