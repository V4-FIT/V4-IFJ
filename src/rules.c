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
	TRY_EXECUTE_RULE(rule_eol_opt_n, TK_EOL);
	EXECUTE_RULE(rule_prolog);
	TRY_EXECUTE_RULE(rule_eol_opt_n, TK_EOL);
	TRY_EXECUTE_RULE(rule_functions, TK_KEYW_FUNC, TK_EOL);
	TK_NEXT_IF_SET(TK_EOF, TK_EOL);

	return EXIT_SUCCESS;
}

/// 2
int rule_prolog(scanner_t scanner) {
	RULE_INIT();
	// Prolog -> 		  package main eol .
	TK_NEXT_IF(TK_KEYW_PACKAGE);
	TK_NEXT_IF(TK_KEYW_MAIN);
	TK_NEXT_IF(TK_EOL);

	return EXIT_SUCCESS;
}

/// 3
int rule_functions(scanner_t scanner) {
	RULE_INIT();
	// Functions -> 		  Function Functions
	//						| Eol_opt .
	TRY_EXECUTE_RULE(rule_function, TK_KEYW_FUNC);
	TRY_EXECUTE_RULE(rule_functions, TK_KEYW_FUNC);
	TRY_EXECUTE_RULE(rule_eol_opt, TK_EOL);

	return EXIT_SUCCESS;
}

/// 4
int rule_function(scanner_t scanner) {
	RULE_INIT();
	// Function -> 		  func id l_parenthesis Param_list r_parenthesis
	//					  Return_list l_curly Statements r_curly eol .
	TK_NEXT_IF(TK_KEYW_FUNC);

	TK_NEXT_IF_SET(TK_IDENTIFIER, TK_KEYW_MAIN);

	TK_NEXT_IF(TK_L_PARENTHESIS);
	TRY_EXECUTE_RULE(rule_param_list, TK_IDENTIFIER);
	TK_NEXT_IF(TK_R_PARENTHESIS);

	TRY_EXECUTE_RULE(rule_return_list, TK_L_PARENTHESIS);

	TK_NEXT_IF(TK_L_CURLY);
	TK_NEXT_IF(TK_EOL);
	TRY_EXECUTE_RULE(rule_statements, TK_IDENTIFIER, TK_UNDERSCORE, TK_KEYW_IF, TK_KEYW_FOR, TK_KEYW_RETURN, TK_EOL);
	TK_NEXT_IF(TK_R_CURLY);

	TK_NEXT_IF_SET(TK_EOL, TK_EOF);

	return EXIT_SUCCESS;
}

/// 5
int rule_param_list(scanner_t scanner) {
	RULE_INIT();
	// Param_list ->		  Param Param_n
	//						| eps .
	EXECUTE_RULE(rule_param);
	TRY_EXECUTE_RULE(rule_param_n, TK_COMMA);

	return EXIT_SUCCESS;
}

/// 6
int rule_param_n(scanner_t scanner) {
	RULE_INIT();
	// Param_n -> 			  comma Eol_opt Param Param_n
	//						| eps.
	TK_NEXT_IF(TK_COMMA);
	TRY_EXECUTE_RULE(rule_eol_opt, TK_EOL);
	EXECUTE_RULE(rule_param);
	TRY_EXECUTE_RULE(rule_param_n, TK_COMMA);

	return EXIT_SUCCESS;
}

/// 7
int rule_param(scanner_t scanner) {
	RULE_INIT();
	// Param -> 			  id Typename .
	TK_NEXT_IF(TK_IDENTIFIER);
	EXECUTE_RULE(rule_typename);

	return EXIT_SUCCESS;
}


/// 8
int rule_return_list(scanner_t scanner) {
	RULE_INIT();
	// Return_list -> 	  	  l_parenthesis Return_n r_parenthesis
	//						| eps .
	TK_NEXT_IF(TK_L_PARENTHESIS);
	TRY_EXECUTE_RULE(rule_return_n, TK_KEYW_INT, TK_KEYW_FLOAT64, TK_KEYW_STRING, TK_KEYW_BOOL);
	TK_NEXT_IF(TK_R_PARENTHESIS);

	return EXIT_SUCCESS;
}

/// 9
int rule_return_n(scanner_t scanner) {
	RULE_INIT();
	// Return_n -> 			  Typename Type_n
	//						| eps .
	EXECUTE_RULE(rule_typename);
	TRY_EXECUTE_RULE(rule_type_n, TK_COMMA);

	return EXIT_SUCCESS;
}

/// 10
int rule_type_n(scanner_t scanner) {
	RULE_INIT();
	// Type_n -> 			  comma Typename Type_n
	//						| eps .
	TK_NEXT_IF(TK_COMMA);
	EXECUTE_RULE(rule_typename);
	TRY_EXECUTE_RULE(rule_type_n, TK_COMMA);

	return EXIT_SUCCESS;
}

/// 11
int rule_typename(scanner_t scanner) {
	RULE_INIT();
	// Typename ->	 		  float64
	//						| int
	//						| string
	//						| bool .
	TK_NEXT_IF_SET(TK_KEYW_INT, TK_KEYW_FLOAT64, TK_KEYW_STRING, TK_KEYW_BOOL);

	return EXIT_SUCCESS;
}


/// 12
int rule_statements(scanner_t scanner) {
	RULE_INIT();
	// Statements ->		  Statement Statements
	//						| Eol_opt .
	TRY_EXECUTE_RULE(rule_statement, TK_IDENTIFIER, TK_UNDERSCORE, TK_KEYW_IF, TK_KEYW_FOR, TK_KEYW_RETURN);
	TRY_EXECUTE_RULE(rule_statements, TK_IDENTIFIER, TK_UNDERSCORE, TK_KEYW_IF, TK_KEYW_FOR, TK_KEYW_RETURN);
	TRY_EXECUTE_RULE(rule_eol_opt, TK_EOL);

	return EXIT_SUCCESS;
}

/// 13
int rule_statement(scanner_t scanner) {
	RULE_INIT();
	// Statement -> 		  Def_Ass_Call eol
	//						| Conditionals eol
	//						| Iterative eol
	//						| Return eol .
	TRY_EXECUTE_RULE(rule_def_ass_call, TK_IDENTIFIER, TK_UNDERSCORE);
	TRY_EXECUTE_RULE(rule_conditionals, TK_KEYW_IF);
	TRY_EXECUTE_RULE(rule_iterative, TK_KEYW_FOR);
	TRY_EXECUTE_RULE(rule_return, TK_KEYW_RETURN);

	TK_NEXT_IF(TK_EOL);

	return EXIT_SUCCESS;
}

/// 14
int rule_def_ass_call(scanner_t scanner) {
	RULE_INIT();
	// Def_Ass_Call ->	  	  underscore Ass_Call
	//						| id Def_Ass_Call2 .
	switch (TOKEN) {
		case TK_UNDERSCORE:
			TK_NEXT();
			EXECUTE_RULE(rule_ass_call);
			break;
		case TK_IDENTIFIER:
			TK_NEXT();
			EXECUTE_RULE(rule_def_ass_call2);
			break;
		default:
			return ERROR_SYN;
	}

	return EXIT_SUCCESS;
}

/// 15
int rule_ass_call(scanner_t scanner) {
	RULE_INIT();
	// Ass_Call ->			  l_parenthesis FunCall
	//						| comma Ids AssignOp Exprs_FunCall .
	switch (TOKEN) {
		case TK_L_PARENTHESIS:
			TK_NEXT();
			EXECUTE_RULE(rule_funCall);
			break;
		case TK_COMMA:
			TK_NEXT();
			EXECUTE_RULE(rule_ids);
			EXECUTE_RULE(rule_assignOp);
			EXECUTE_RULE(rule_exprs_funCall);
			break;
		default:
			return ERROR_SYN;
	}

	return EXIT_SUCCESS;
}

/// 16
int rule_def_ass_call2(scanner_t scanner) {
	RULE_INIT();
	// Def_Ass_Call2 ->		  l_parenthesis FunCall
	//						| defineOp Expression
	//						| comma Ids AssignOp Exprs_FunCall .
	switch (TOKEN) {
		case TK_L_PARENTHESIS:
			TK_NEXT();
			EXECUTE_RULE(rule_funCall);
			break;
		case TK_VAR_INIT:
			TK_NEXT();
			EXECUTE_RULE(rule_expression);
			break;
		case TK_COMMA:
			TK_NEXT();
			EXECUTE_RULE(rule_ids);
			EXECUTE_RULE(rule_assignOp);
			EXECUTE_RULE(rule_exprs_funCall);
			break;
		default:
			TRY_EXECUTE_RULE(rule_assignOp, TK_PLUS_ASSIGN, TK_MINUS_ASSIGN, TK_MULTIPLY_ASSIGN, TK_DIVIDE_ASSIGN, TK_ASSIGN);
			if (TRY_SUCCESS) {
				EXECUTE_RULE(rule_exprs_funCall);
			} else {
				return ERROR_SYN;
			}
	}

	return EXIT_SUCCESS;
}

/// 17
int rule_funCall(scanner_t scanner) {
	RULE_INIT();
	// FunCall ->			  Eol_opt Arguments r_parenthesis .
	TRY_EXECUTE_RULE(rule_eol_opt, TK_EOL);
	TRY_EXECUTE_RULE(rule_Arguments, TK_IDENTIFIER, TK_KEYW_INT, TK_INT_LIT, TK_FLOAT_LIT, TK_STR_LIT, TK_KEYW_TRUE, TK_KEYW_FALSE);
	TK_NEXT_IF(TK_R_PARENTHESIS);

	return EXIT_SUCCESS;
}

/// 18
int rule_var_define(scanner_t scanner) {
	RULE_INIT();
	// Var_define -> 	  	  id defineOp Expression .
	TK_NEXT_IF(TK_IDENTIFIER);
	TK_NEXT_IF(TK_VAR_INIT);
	EXECUTE_RULE(rule_expression);

	return EXIT_SUCCESS;
}

/// 19
int rule_conditionals(scanner_t scanner) {
	RULE_INIT();
	// Conditionals -> 		  Conditional Conditional_n .
	EXECUTE_RULE(rule_conditional);
	TRY_EXECUTE_RULE(rule_conditional_n, TK_KEYW_ELSE);

	return EXIT_SUCCESS;
}

/// 20
int rule_conditional_n(scanner_t scanner) {
	RULE_INIT();
	// Conditional_n ->		  else Else
	//						| eps .
	TK_NEXT_IF(TK_KEYW_ELSE);
	EXECUTE_RULE(rule_else);

	return EXIT_SUCCESS;
}

/// 21
int rule_else(scanner_t scanner) {
	RULE_INIT();
	// Else -> 				  Conditional Conditional_n
	//						| l_curly eol Statements r_curly .
	switch (TOKEN) {
		case TK_KEYW_IF:
			EXECUTE_RULE(rule_conditional);
			TRY_EXECUTE_RULE(rule_conditional_n, TK_KEYW_ELSE);
			break;
		case TK_L_CURLY:
			TK_NEXT();
			TK_NEXT_IF(TK_EOL);
			TRY_EXECUTE_RULE(rule_statements, TK_IDENTIFIER, TK_UNDERSCORE, TK_KEYW_IF, TK_KEYW_FOR, TK_KEYW_RETURN);
			TK_NEXT_IF(TK_R_CURLY);
			break;
		default:
			return ERROR_SYN;
	}

	return EXIT_SUCCESS;
}

/// 22
int rule_conditional(scanner_t scanner) {
	RULE_INIT();
	// Conditional -> 	  	  if Expression l_curly eol Statements r_curly .
	TK_NEXT_IF(TK_KEYW_IF);
	EXECUTE_RULE(rule_expression);
	TK_NEXT_IF(TK_L_CURLY);
	TK_NEXT_IF(TK_EOL);
	TRY_EXECUTE_RULE(rule_statements, TK_IDENTIFIER, TK_UNDERSCORE, TK_KEYW_IF, TK_KEYW_FOR, TK_KEYW_RETURN);
	TK_NEXT_IF(TK_R_CURLY);

	return EXIT_SUCCESS;
}

/// 23
int rule_iterative(scanner_t scanner) {
	RULE_INIT();
	// Iterative -> 		  for Var_define semicolon Expression semicolon
	//						  Assignment l_curly Statements r_curly .
	TK_NEXT_IF(TK_KEYW_FOR);
	TRY_EXECUTE_RULE(rule_var_define, TK_IDENTIFIER);
	TK_NEXT_IF(TK_SEMICOLON);

	EXECUTE_RULE(rule_expression);
	TK_NEXT_IF(TK_SEMICOLON);

	TRY_EXECUTE_RULE(rule_assignment, TK_IDENTIFIER, TK_UNDERSCORE);

	TK_NEXT_IF(TK_L_CURLY);
	TRY_EXECUTE_RULE(rule_statements, TK_IDENTIFIER, TK_UNDERSCORE, TK_KEYW_IF, TK_KEYW_FOR, TK_KEYW_RETURN);
	TK_NEXT_IF(TK_R_CURLY);

	return EXIT_SUCCESS;
}

/// 24
int rule_assignment(scanner_t scanner) {
	RULE_INIT();
	// Assignment -> 	  	  Ids AssignOp Exprs_FunCall .
	EXECUTE_RULE(rule_ids);
	EXECUTE_RULE(rule_assignOp);
	EXECUTE_RULE(rule_exprs_funCall);

	return EXIT_SUCCESS;
}

/// 25
int rule_ids(scanner_t scanner) {
	RULE_INIT();
	// Ids -> 				  Id Id_n .
	EXECUTE_RULE(rule_id);
	TRY_EXECUTE_RULE(rule_id_n, TK_COMMA);

	return EXIT_SUCCESS;
}

/// 26
int rule_id_n(scanner_t scanner) {
	RULE_INIT();
	// Id_n ->	 			  comma Id Id_n
	//						| eps .
	TK_NEXT_IF(TK_COMMA);
	EXECUTE_RULE(rule_id);
	TRY_EXECUTE_RULE(rule_id_n, TK_COMMA);

	return EXIT_SUCCESS;
}

/// 27
int rule_id(scanner_t scanner) {
	RULE_INIT();
	// Id -> 				  id
	//						| underscore .
	TK_NEXT_IF_SET(TK_IDENTIFIER, TK_UNDERSCORE);

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
	TK_NEXT_IF_SET(TK_PLUS_ASSIGN, TK_MINUS_ASSIGN, TK_MULTIPLY_ASSIGN, TK_DIVIDE_ASSIGN, TK_ASSIGN);

	return EXIT_SUCCESS;
}

/// 29
int rule_exprs_funCall(scanner_t scanner) {
	RULE_INIT();
	// Exprs_FunCall ->		  Expression
	//						| FunctionCall .

	// TODO: expre vs funcall - BIG TODO btw

	TRY_EXECUTE_RULE(rule_functionCall, TK_IDENTIFIER);
	EXECUTE_RULE(rule_expressions);
	return EXIT_SUCCESS;
}

/// 30
int rule_functionCall(scanner_t scanner) {
	RULE_INIT();
	// FunctionCall -> 		  id l_parenthesis Eol_opt Arguments r_parenthesis .
	TK_NEXT_IF(TK_IDENTIFIER);
	TK_NEXT_IF(TK_L_PARENTHESIS);
	TRY_EXECUTE_RULE(rule_eol_opt, TK_EOL);
	TRY_EXECUTE_RULE(rule_Arguments, TK_IDENTIFIER, TK_KEYW_INT, TK_INT_LIT, TK_FLOAT_LIT, TK_STR_LIT, TK_KEYW_TRUE, TK_KEYW_FALSE);
	return EXIT_SUCCESS;
}

/// 31
int rule_Arguments(scanner_t scanner) {
	RULE_INIT();
	// Arguments ->			  Argument Argument_n
	//						| eps .
	EXECUTE_RULE(rule_Argument);
	TRY_EXECUTE_RULE(rule_Argument_n, TK_COMMA);
	return EXIT_SUCCESS;
}

/// 32
int rule_Argument_n(scanner_t scanner) {
	RULE_INIT();
	// Argument_n -> 	  	  comma Eol_opt Argument Argument_n
	//						| eps .
	TK_NEXT_IF(TK_COMMA);
	TRY_EXECUTE_RULE(rule_eol_opt, TK_EOL);
	EXECUTE_RULE(rule_Argument);
	TRY_EXECUTE_RULE(rule_Argument_n, TK_COMMA);
	return EXIT_SUCCESS;
}

/// 33
int rule_Argument(scanner_t scanner) {
	RULE_INIT();
	// Argument -> 			  id
	//						| Literal .
	switch (TOKEN) {
		case TK_IDENTIFIER:
			TK_NEXT();
			break;
		default:
			EXECUTE_RULE(rule_literal);
	}

	return EXIT_SUCCESS;
}

/// 34
int rule_return(scanner_t scanner) {
	RULE_INIT();
	// Return -> 			  return Expressions .
	TK_NEXT_IF(TK_KEYW_RETURN);
	EXECUTE_RULE(rule_expressions);
	return EXIT_SUCCESS;
}

/// 35
int rule_expressions(scanner_t scanner) {
	RULE_INIT();
	// Expressions -> 	  	  Expression Expression_n .
	EXECUTE_RULE(rule_expression);
	TRY_EXECUTE_RULE(rule_expression_n, TK_COMMA);
	return EXIT_SUCCESS;
}

/// 36
int rule_expression_n(scanner_t scanner) {
	RULE_INIT();
	// Expression_n ->	  	  comma Expression Expression_n
	//						| eps .
	TK_NEXT_IF(TK_COMMA);
	EXECUTE_RULE(rule_expression);
	TRY_EXECUTE_RULE(rule_expression_n, TK_COMMA);
	return EXIT_SUCCESS;
}

/// 37
int rule_expression(scanner_t scanner) {
	RULE_INIT();
	// Expression -> 	  	  Term
	//						| Term BinaryOp Term
	//						| l_parenthesis Eol_opt Term r_parenthesis .

	TRY_EXECUTE_RULE(rule_unaryOp, TK_PLUS, TK_MINUS, TK_NOT);

	switch (TOKEN) {
		case TK_L_PARENTHESIS:
			TK_NEXT();
			TRY_EXECUTE_RULE(rule_eol_opt, TK_EOL);
			EXECUTE_RULE(rule_expression);
			TK_NEXT_IF(TK_R_PARENTHESIS);
			break;
		default:
			TRY_EXECUTE_RULE(rule_term, TK_IDENTIFIER, TK_INT_LIT, TK_FLOAT_LIT, TK_STR_LIT, TK_KEYW_TRUE, TK_KEYW_FALSE);
			if (TRY_SUCCESS) {
				TRY_EXECUTE_RULE(rule_binaryOp, TK_PLUS, TK_MINUS, TK_MULTIPLY, TK_DIVIDE, TK_PLUS_ASSIGN, TK_MINUS_ASSIGN, TK_MULTIPLY_ASSIGN, TK_DIVIDE_ASSIGN, TK_EQUAL, TK_NOT_EQUAL, TK_LESS, TK_GREATER, TK_LESS_EQUAL, TK_GREATER_EQUAL, TK_OR, TK_AND);
				if (!TRY_SUCCESS) {
					return EXIT_SUCCESS;
				}
				TRY_EXECUTE_RULE(rule_unaryOp, TK_PLUS, TK_MINUS, TK_NOT);
				EXECUTE_RULE(rule_term);
			} else {
				return ERROR_SYN;
			}
	}
	return EXIT_SUCCESS;
}

int rule_unaryOp(scanner_t scanner) {
	RULE_INIT();
	TK_NEXT_IF_SET(TK_PLUS, TK_MINUS, TK_NOT);
	return EXIT_SUCCESS;
}

int rule_term(scanner_t scanner) {
	RULE_INIT();
	// adhoc rule
	TK_NEXT_IF_SET(TK_IDENTIFIER, TK_INT_LIT, TK_FLOAT_LIT, TK_STR_LIT, TK_KEYW_TRUE, TK_KEYW_FALSE);
	return EXIT_SUCCESS;
}

int rule_binaryOp(scanner_t scanner) {
	RULE_INIT();
	// adhoc rule
	TK_NEXT_IF_SET(TK_PLUS, TK_MINUS, TK_MULTIPLY, TK_DIVIDE, TK_PLUS_ASSIGN, TK_MINUS_ASSIGN, TK_MULTIPLY_ASSIGN, TK_DIVIDE_ASSIGN, TK_ASSIGN, TK_EQUAL, TK_NOT_EQUAL, TK_LESS, TK_GREATER, TK_LESS_EQUAL, TK_GREATER_EQUAL, TK_OR, TK_AND);
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
	TK_NEXT_IF_SET(TK_INT_LIT, TK_FLOAT_LIT, TK_STR_LIT, TK_KEYW_TRUE, TK_KEYW_FALSE);
	return EXIT_SUCCESS;
}

int rule_eol_opt_n(scanner_t scanner) {
	RULE_INIT();
	// adhoc rule
	// Eol_opt ->			  Eol_opt Eol_opt_n .
	EXECUTE_RULE(rule_eol_opt);
	TRY_EXECUTE_RULE(rule_eol_opt_n, TK_EOL);
	return EXIT_SUCCESS;
}

/// 39
int rule_eol_opt(scanner_t scanner) {
	RULE_INIT();
	// Eol_opt ->			  eol
	//						| eps .

	TK_NEXT_IF(TK_EOL);

	return EXIT_SUCCESS;
}
