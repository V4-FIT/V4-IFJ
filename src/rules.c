#include "rules.h"

#include <stdarg.h>
#include <assert.h>

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
// Var_define -> εREQUIRE_NONTERMINAL
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
	GET_NEXT_TOKEN();
	EXECUTE_RULE(rule_program);

	return EXIT_SUCCESS;
}

////// Actual rule definitions

/// 1
static int rule_program(scanner_t scanner) {
	// Program -> Prolog Functions eof
	EXECUTE_RULE(rule_prolog);
	TRY_EXECUTE_RULE(rule_functions, 1, TK_KEYW_FUNC);
	REQUIRE(TK_EOF);

	return EXIT_SUCCESS;
}

/// 2
static int rule_prolog(scanner_t scanner) {
	// Prolog -> package main eol
	REQUIRE(TK_KEYW_PACKAGE);
	REQUIRE(TK_KEYW_MAIN);
	REQUIRE(TK_EOL);

	return EXIT_SUCCESS;
}

/// 3
static int rule_functions(scanner_t scanner) {
	// Functions -> Function Function_n
	EXECUTE_RULE(rule_function);
	TRY_EXECUTE_RULE(rule_functions, 1, TK_KEYW_FUNC);

	return EXIT_SUCCESS;
}

/// 4
/// 5
//static int rule_function_n(scanner_t scanner) {
//	// Function_n -> Function Function_n
//	// Function_n -> ε
//	EXECUTE_RULE(rule_function);
//	EXECUTE_RULE(rule_function_n);
//
//	return EXIT_SUCCESS;
//}

/// 6
static int rule_function(scanner_t scanner) {
	// Function -> func Id ( Params ) ReturnTypes { Statements }
	REQUIRE(TK_KEYW_FUNC);

	REQUIRE_SET(2, TK_IDENTIFIER, TK_KEYW_MAIN);

	REQUIRE(TK_L_PARENTHESIS);
	TRY_EXECUTE_RULE(rule_params, 4, TK_KEYW_INT, TK_KEYW_FLOAT64, TK_KEYW_STRING, TK_KEYW_BOOL);
	REQUIRE(TK_R_PARENTHESIS);

	TRY_EXECUTE_RULE(rule_returnTypes, 1, TK_L_PARENTHESIS);

	REQUIRE(TK_L_CURLY);
	TRY_EXECUTE_RULE(rule_statements, 4, TK_IDENTIFIER, TK_KEYW_IF, TK_KEYW_FOR, TK_KEYW_RETURN);
	REQUIRE(TK_R_CURLY);

	return EXIT_SUCCESS;
}

/// 7
static int rule_params(scanner_t scanner) {
	// Params -> Param Param_n
	EXECUTE_RULE(rule_param);
	TRY_EXECUTE_RULE(rule_param_n, 1, TK_COMMA);

	return EXIT_SUCCESS;
}

/// 8
/// 9
static int rule_param_n(scanner_t scanner) {
	// Param_n -> , Param Param_n
	// Param_n -> ε
	REQUIRE(TK_COMMA);

	EXECUTE_RULE(rule_param);
	TRY_EXECUTE_RULE(rule_param_n, 1, TK_COMMA);

	return EXIT_SUCCESS;
}

/// 10
/// 11
static int rule_param(scanner_t scanner) {
	// Param -> Type id
	// Param -> ε
	EXECUTE_RULE(rule_type);
	REQUIRE(TK_IDENTIFIER);

	return EXIT_SUCCESS;
}

/// 12
static int rule_returnTypes(scanner_t scanner) {
	// ReturnTypes -> ( Type Type_n )

	REQUIRE(TK_L_PARENTHESIS);

	TRY_EXECUTE_RULE(rule_type, 4, TK_KEYW_INT, TK_KEYW_FLOAT64, TK_KEYW_STRING, TK_KEYW_BOOL);
	TRY_EXECUTE_RULE(rule_type_n, 1, TK_COMMA);

	REQUIRE(TK_R_PARENTHESIS);

	return EXIT_SUCCESS;
}

/// 13
/// 14
static int rule_type_n(scanner_t scanner) {
	// Type_n -> , Type Type_n
	// Type_n -> ε
	REQUIRE(TK_COMMA);
	EXECUTE_RULE(rule_type);
	TRY_EXECUTE_RULE(rule_type_n, 1, TK_COMMA);

	return EXIT_SUCCESS;
}

/// 15
/// 16
/// 17
/// 18
static int rule_type(scanner_t scanner) { // 4x must return EPS
	// Type -> float64
	// Type -> int
	// Type -> string
	REQUIRE_SET(4, TK_KEYW_FLOAT64, TK_KEYW_INT, TK_KEYW_STRING, TK_KEYW_BOOL);

	return EXIT_SUCCESS;
}

// TODO

static int rule_statements(scanner_t scanner) {
	// Statements -> Statement Statement_n
	// Statements -> ε
	EXECUTE_RULE(rule_statement);
	EXECUTE_RULE(rule_statement_n);
	return EXIT_SUCCESS;
}

static int rule_statement_n(scanner_t scanner) { // 2x
	// Statement_n -> eol Statement Statement_n
	REQUIRE(TK_EOL);
	EXECUTE_RULE(rule_statement);
	EXECUTE_RULE(rule_statement_n);
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

	

	EXECUTE_RULE(rule_var_define);
	EXECUTE_RULE(rule_functionCall);
	EXECUTE_RULE(rule_assignment);

	EXECUTE_RULE(rule_conditionals);
	EXECUTE_RULE(rule_iterative);
	EXECUTE_RULE(rule_return);
	return EXIT_SUCCESS;
}

static int rule_var_define(scanner_t scanner) {
	//Var_define -> id := Expression
	// Var_define -> ε

	REQUIRE(TK_IDENTIFIER);

	return EXIT_SUCCESS;
}

// LL: Var_define -> ε

static int rule_assignment(scanner_t scanner) {
	return EXIT_SUCCESS;
}

// LL: Assignment -> ε

static int rule_ids(scanner_t scanner) {
	//	Ids -> id Id_n
	REQUIRE(TK_IDENTIFIER);
	EXECUTE_RULE(rule_id_n);
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

	REQUIRE_SET(5, TK_ASSIGN, TK_PLUS_ASSIGN, TK_MINUS_ASSIGN, TK_MULTIPLY_ASSIGN, TK_DIVIDE_ASSIGN);
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

	EXECUTE_RULE(rule_term);
	return EXIT_SUCCESS;
}

static int rule_term(scanner_t scanner) { // 2x
	// Term -> id
	// Term -> Literal

	// ?
	REQUIRE(TK_IDENTIFIER);
	EXECUTE_RULE(rule_literal);
	return EXIT_SUCCESS;
}

static int rule_literal(scanner_t scanner) { // 2x
	// Literal -> IntLit
	// Literal -> FloatLit
	// Literal -> StringLit
	// Literal -> true
	// Literal -> false
	REQUIRE_SET(5, TK_KEYW_TRUE, TK_KEYW_FALSE, TK_STR_LIT, TK_INT_LIT, TK_FLOAT_LIT);

	return EXIT_SUCCESS;
}

static int rule_return(scanner_t scanner) {
	// Return -> return Expressions

	REQUIRE(TK_KEYW_RETURN);
	EXECUTE_RULE(rule_expressions);
	return EXIT_SUCCESS;
}

