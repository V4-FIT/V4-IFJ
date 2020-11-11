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
	REQUIRE_NONTERMINAL(rule_program);

	return EXIT_SUCCESS;
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
	EXPECT_NONTERMINAL(rule_function_n);

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

static int rule_statements(scanner_t scanner) {
	// Statements -> Statement Statement_n
	// Statements -> ε
	EXPECT_NONTERMINAL(rule_statement);
	EXPECT_NONTERMINAL(rule_statement_n);
	return EXIT_SUCCESS;
}

static int rule_statement_n(scanner_t scanner) { // 2x
	// Statement_n -> eol Statement Statement_n
	EXPECT_TERMINAL(TK_EOL);
	EXPECT_NONTERMINAL(rule_statement);
	EXPECT_NONTERMINAL(rule_statement_n);
	return EXIT_SUCCESS;
}

static int rule_statement(scanner_t scanner) { // 6x
	//Statement -> Var_define
	// Statement -> Assignment
	// Statement -> Conditionals
	// Statement -> Iterative
	// Statement -> FunctionCall
	// Statement -> Return
	// Statement -> ε

	EXPECT_NONTERMINAL(rule_var_define);
	EXPECT_NONTERMINAL(rule_assignment);
	EXPECT_NONTERMINAL(rule_conditionals);
	EXPECT_NONTERMINAL(rule_iterative);
	EXPECT_NONTERMINAL(rule_return);
	EXPECT_NONTERMINAL(rule_functionCall);
	return EXIT_SUCCESS;
}

static int rule_var_define(scanner_t scanner) {
	//Var_define -> id := Expression
	// Var_define -> ε

	EXPECT_TERMINAL(TK_IDENTIFIER);

	return EXIT_SUCCESS;
}

// LL: Var_define -> ε

static int rule_assignment(scanner_t scanner) {
	return EXIT_SUCCESS;
}

// LL: Assignment -> ε

static int rule_ids(scanner_t scanner) {
	//	Ids -> id Id_n
	REQUIRE_TERMINAL(TK_IDENTIFIER);
	REQUIRE_NONTERMINAL(rule_id_n);
	return EXIT_SUCCESS;
}

static int rule_id_n(scanner_t scanner) { // 2x
	// Id_n -> , id Id_n
	// Id_n -> ε

	return EXIT_SUCCESS;
}

static int rule_expressions(scanner_t scanner) {
	// Expressions -> Expression Expression_n
	return EXIT_SUCCESS;
}

static int rule_expression_n(scanner_t scanner) { // 2x
	// Expression_n -> , Expression Expression_n
	// Expression_n -> ε
	return EXIT_SUCCESS;
}

static int rule_assignOp(scanner_t scanner) { // 5x
	// AssignOp -> +=
	// AssignOp -> -=
	// AssignOp -> *=
	// AssignOp -> /=
	// AssignOp -> =

	int validKW[5] = {TK_ASSIGN, TK_PLUS_ASSIGN, TK_MINUS_ASSIGN, TK_MULTIPLY_ASSIGN, TK_DIVIDE_ASSIGN };
	EXPECT_TERMINAL_SET(validKW, 5);
	return EXIT_SUCCESS;
}

static int rule_conditionals(scanner_t scanner) {
	return EXIT_SUCCESS;
}

static int rule_conditional_n(scanner_t scanner) { // 3x
	return EXIT_SUCCESS;
}

static int rule_conditional(scanner_t scanner) { // 2x
	return EXIT_SUCCESS;
}

static int rule_iterative(scanner_t scanner) {
	return EXIT_SUCCESS;
}

static int rule_functionCall(scanner_t scanner) { // 2x
	return EXIT_SUCCESS;
}

static int rule_funParams(scanner_t scanner) {
	return EXIT_SUCCESS;
}

static int rule_funParam_n(scanner_t scanner) { // 2x
	return EXIT_SUCCESS;
}

static int rule_funParam(scanner_t scanner) { // 2x
	// FunParam -> Term
	// FunParam -> ε

	EXPECT_NONTERMINAL(rule_term);
	return EXIT_SUCCESS;
}

static int rule_term(scanner_t scanner) { // 2x
	// Term -> id
	// Term -> Literal

	// ?
	EXPECT_TERMINAL(TK_IDENTIFIER);
	EXPECT_NONTERMINAL(rule_literal);
	return EXIT_SUCCESS;
}

static int rule_literal(scanner_t scanner) { // 2x
	// Literal -> IntLit
	// Literal -> FloatLit
	// Literal -> StringLit
	// Literal -> true
	// Literal -> false
	int validKW[5] = {TK_KEYW_TRUE, TK_KEYW_FALSE, TK_STR_LIT,	TK_INT_LIT,	TK_FLOAT_LIT};
	EXPECT_TERMINAL_SET(validKW, 5);

	return EXIT_SUCCESS;
}

static int rule_return(scanner_t scanner) {
	// Return -> return Expressions

	REQUIRE_TERMINAL(TK_KEYW_RETURN);
	REQUIRE_NONTERMINAL(rule_expressions);
	return EXIT_SUCCESS;
}

