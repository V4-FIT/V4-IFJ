#include "rules.h"

#include <assert.h>
#include <stdio.h>

#include "rulemacros.h"

int debug = 0;

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
static int rule_funCall(scanner_t scanner);
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
static int rule_eol_opt(scanner_t scanner);

// adhoc rules
static int rule_term(scanner_t scanner);
static int rule_binaryOp(scanner_t scanner);
static int rule_eol_opt_n(scanner_t scanner);
static int rule_unaryOp(scanner_t scanner);

////// Root

int rule_root(scanner_t scanner) {
	RULE_INIT();
	debug = 0;
	TK_NEXT();
	EXECUTE_RULE(rule_program);

	return EXIT_SUCCESS;
}

////// Actual rule definitions

/// 1
int rule_program(scanner_t scanner) {
	RULE_INIT();
	// Program  -> 		  Prolog Functions eof .
	return EXIT_SUCCESS;
}

/// 2
int rule_prolog(scanner_t scanner) {
	RULE_INIT();
	// Prolog -> 		  package main eol .
	return EXIT_SUCCESS;
}

/// 3
int rule_functions(scanner_t scanner) {
	RULE_INIT();
	// Functions -> 		  Function Functions
	//						| Eol_opt .
	return EXIT_SUCCESS;
}

/// 4
int rule_function(scanner_t scanner) {
	RULE_INIT();
	// Function -> 		  func id l_parenthesis Param_list r_parenthesis
	//					  Return_list l_curly Statements r_curly eol .
	return EXIT_SUCCESS;
}

/// 5
int rule_param_list(scanner_t scanner) {
	RULE_INIT();
	// Param_list ->		  Param Param_n
	//						| eps .
	return EXIT_SUCCESS;
}

/// 6
int rule_param_n(scanner_t scanner) {
	RULE_INIT();
	// Param_n -> 			  comma Eol_opt Param Param_n
	//						| eps.
	return EXIT_SUCCESS;
}

/// 7
int rule_param(scanner_t scanner) {
	RULE_INIT();
	// Param -> 			  id Typename .
	return EXIT_SUCCESS;
}


/// 8
int rule_return_list(scanner_t scanner) {
	RULE_INIT();
	// Return_list -> 	  	  l_parenthesis Return_n r_parenthesis
	//						| eps .
	return EXIT_SUCCESS;
}

/// 9
int rule_return_n(scanner_t scanner) {
	RULE_INIT();
	// Return_n -> 			  Typename Type_n
	//						| eps .
	return EXIT_SUCCESS;
}

/// 10
int rule_type_n(scanner_t scanner) {
	RULE_INIT();
	// Type_n -> 			  comma Typename Type_n
	//						| eps .
	return EXIT_SUCCESS;
}

/// 11
int rule_typename(scanner_t scanner) {
	RULE_INIT();
	// Typename ->	 		  float64
	//						| int
	//						| string
	//						| bool .
	return EXIT_SUCCESS;
}


/// 12
int rule_statements(scanner_t scanner) {
	RULE_INIT();
	// Statements ->		  Statement Statements
	//						| Eol_opt .
	return EXIT_SUCCESS;
}

/// 13
int rule_statement(scanner_t scanner) {
	RULE_INIT();
	// Statement -> 		  Def_Ass_Call eol
	//						| Conditionals eol
	//						| Iterative eol
	//						| Return eol .
	return EXIT_SUCCESS;
}

/// 14
int rule_def_ass_call(scanner_t scanner) {
	RULE_INIT();
	// Def_Ass_Call ->	  	  underscore Ass_Call
	//						| id Def_Ass_Call2 .
	return EXIT_SUCCESS;
}

/// 15
int rule_ass_call(scanner_t scanner) {
	RULE_INIT();
	// Ass_Call ->			  l_parenthesis FunCall
	//						| comma Ids AssignOp Exprs_FunCall .
	return EXIT_SUCCESS;
}

/// 16
int rule_def_ass_call2(scanner_t scanner) {
	RULE_INIT();
	// Def_Ass_Call2 ->		  l_parenthesis FunCall
	//						| defineOp Expression
	//						| comma Ids AssignOp Exprs_FunCall .
	return EXIT_SUCCESS;
}

/// 17
int rule_funCall(scanner_t scanner) {
	RULE_INIT();
	// FunCall ->			  Eol_opt Arguments r_parenthesis .
	return EXIT_SUCCESS;
}

/// 18
int rule_var_define(scanner_t scanner) {
	RULE_INIT();
	// Var_define -> 	  	  id defineOp Expression .
	return EXIT_SUCCESS;
}

/// 19
int rule_conditionals(scanner_t scanner) {
	RULE_INIT();
	// Conditionals -> 		  Conditional Conditional_n .
	return EXIT_SUCCESS;
}

/// 20
int rule_conditional_n(scanner_t scanner) {
	RULE_INIT();
	// Conditional_n ->		  else Else
	//						| eps .
	return EXIT_SUCCESS;
}

/// 21
int rule_else(scanner_t scanner) {
	RULE_INIT();
	// Else -> 				  Conditional Conditional_n
	//						| l_curly eol Statements r_curly .
	return EXIT_SUCCESS;
}

/// 22
int rule_conditional(scanner_t scanner) {
	RULE_INIT();
	// Conditional -> 	  	  if Expression l_curly eol Statements r_curly .
	return EXIT_SUCCESS;
}

/// 23
int rule_iterative(scanner_t scanner) {
	RULE_INIT();
	// Iterative -> 		  for Var_define semicolon Expression semicolon
	//						  Assignment l_curly Statements r_curly .
	return EXIT_SUCCESS;
}

/// 24
int rule_assignment(scanner_t scanner) {
	RULE_INIT();
	// Assignment -> 	  	  Ids AssignOp Exprs_FunCall .
	return EXIT_SUCCESS;
}

/// 25
int rule_ids(scanner_t scanner) {
	RULE_INIT();
	// Ids -> 				  Id Id_n .
	return EXIT_SUCCESS;
}

/// 26
int rule_id_n(scanner_t scanner) {
	RULE_INIT();
	// Id_n ->	 			  comma Id Id_n
	//						| eps .
	return EXIT_SUCCESS;
}

/// 27
int rule_id(scanner_t scanner) {
	RULE_INIT();
	// Id -> 				  id
	//						| underscore .
	return EXIT_SUCCESS;
}

/// 28
int rule_assignOp(scanner_t scanner) {
	RULE_INIT();
	// AssignOp -> 			  plus_assign
	//						| minus_assing
	//						| multiply_assign
	//						| divide_assign
	//						| assign .
	return EXIT_SUCCESS;
}

/// 29
int rule_exprs_funCall(scanner_t scanner) {
	RULE_INIT();
	// Exprs_FunCall ->		  Expression
	//						| FunctionCall .

	// TODO: expre vs funcall - BIG TODO btw
	return EXIT_SUCCESS;
}

/// 30
int rule_functionCall(scanner_t scanner) {
	RULE_INIT();
	// FunctionCall -> 		  id l_parenthesis Eol_opt Arguments r_parenthesis .
	return EXIT_SUCCESS;
}

/// 31
int rule_Arguments(scanner_t scanner) {
	RULE_INIT();
	// Arguments ->			  Argument Argument_n
	//						| eps .
	return EXIT_SUCCESS;
}

/// 32
int rule_Argument_n(scanner_t scanner) {
	RULE_INIT();
	// Argument_n -> 	  	  comma Eol_opt Argument Argument_n
	//						| eps .
	return EXIT_SUCCESS;
}

/// 33
int rule_Argument(scanner_t scanner) {
	RULE_INIT();
	// Argument -> 			  id
	//						| Literal .
	return EXIT_SUCCESS;
}

/// 34
int rule_return(scanner_t scanner) {
	RULE_INIT();
	// Return -> 			  return Expressions .
	return EXIT_SUCCESS;
}

/// 35
int rule_expressions(scanner_t scanner) {
	RULE_INIT();
	// Expressions -> 	  	  Expression Expression_n .
	return EXIT_SUCCESS;
}

/// 36
int rule_expression_n(scanner_t scanner) {
	RULE_INIT();
	// Expression_n ->	  	  comma Expression Expression_n
	//						| eps .
	return EXIT_SUCCESS;
}

/// 37
int rule_expression(scanner_t scanner) {
	RULE_INIT();
	// Expression -> 	  	  Term
	//						| Term BinaryOp Term
	//						| l_parenthesis Eol_opt Term r_parenthesis .
	return EXIT_SUCCESS;
}

int rule_unaryOp(scanner_t scanner) {
	RULE_INIT();
	return EXIT_SUCCESS;
}

int rule_term(scanner_t scanner) {
	RULE_INIT();
	// adhoc rule
	return EXIT_SUCCESS;
}

int rule_binaryOp(scanner_t scanner) {
	RULE_INIT();
	// adhoc rule
	return EXIT_SUCCESS;
}

/// 38
int rule_literal(scanner_t scanner) {
	RULE_INIT();
	// Literal -> 			  intLit
	//						| floatLit
	//						| stringLit
	//						| true
	//						| false.
	return EXIT_SUCCESS;
}

int rule_eol_opt_n(scanner_t scanner) {
	RULE_INIT();
	// adhoc rule
	// Eol_opt ->			  Eol_opt Eol_opt_n .
	return EXIT_SUCCESS;
}

/// 39
int rule_eol_opt(scanner_t scanner) {
	RULE_INIT();
	// Eol_opt ->			  eol
	//						| eps .
	return EXIT_SUCCESS;
}
