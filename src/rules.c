#include "rules.h"

#include <assert.h>
#include <stdio.h>

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
static int rule_returntype(scanner_t scanner);
static int rule_statements(scanner_t scanner);
static int rule_def_ass_call(scanner_t scanner);
static int rule_ass_call(scanner_t scanner);
static int rule_def_ass_call2(scanner_t scanner);
static int rule_funCall(scanner_t scanner);
static int rule_var_define_opt(scanner_t scanner);
static int rule_var_define(scanner_t scanner);
static int rule_conditionals(scanner_t scanner);
static int rule_conditional_n(scanner_t scanner);
static int rule_else(scanner_t scanner);
static int rule_conditional(scanner_t scanner);
static int rule_iterative(scanner_t scanner);
static int rule_assignment(scanner_t scanner);
static int rule_assignment_opt(scanner_t scanner);
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
static int rule_eol_opt_n(scanner_t scanner);
static int rule_eol_opt(scanner_t scanner);

// adhoc rules
static int rule_term(scanner_t scanner);
static int rule_binaryOp(scanner_t scanner);
static int rule_unaryOp(scanner_t scanner);

////// Root

int rule_root(scanner_t scanner) {
	TK_NEXT();
	EXECUTE_RULE(rule_program);
	return EXIT_SUCCESS;
}

////// Actual rule definitions

/// 1
int rule_program(scanner_t scanner) {	
	// Program  -> 		 Eol_opt_n Prolog Eol_opt_n Functions Eol_opt_n eof .
	EXECUTE_RULE(rule_eol_opt_n);
	EXECUTE_RULE(rule_prolog);
	EXECUTE_RULE(rule_eol_opt_n);
	EXECUTE_RULE(rule_functions);
	EXECUTE_RULE(rule_eol_opt_n);
	TK_TEST(TK_EOF);
	return EXIT_SUCCESS;
}

/// 2
int rule_prolog(scanner_t scanner) {
	// Prolog -> 		  package main eol .
	TK_MATCH(TK_KEYW_PACKAGE);
	TK_MATCH(TK_KEYW_MAIN);
	TK_MATCH(TK_EOL);
	return EXIT_SUCCESS;
}

/// 3
int rule_functions(scanner_t scanner) {	
	// Functions -> 		  Function Eol_opt_n Functions
	//						| eps .
	switch (TOKEN) {
		case TK_KEYW_FUNC:
			EXECUTE_RULE(rule_function);
			EXECUTE_RULE(rule_eol_opt_n);
			EXECUTE_RULE(rule_functions);
			break;
		default:
			// eps;
			break;
	}
	return EXIT_SUCCESS;
}

/// 4
int rule_function(scanner_t scanner) {
	// Function -> 		  func id l_parenthesis Param_list r_parenthesis
	//					  Return_list l_curly eol Eol_opt_n Statements r_curly eol .
	TK_MATCH(TK_KEYW_FUNC);

	TK_TEST(TK_IDENTIFIER, TK_KEYW_MAIN);
	// semantic action
	TK_NEXT();

	TK_MATCH(TK_L_PARENTHESIS);
	EXECUTE_RULE(rule_param_list);
	TK_MATCH(TK_R_PARENTHESIS);

	EXECUTE_RULE(rule_param_list);

	TK_MATCH(TK_L_CURLY);
	TK_MATCH(TK_EOL);
	EXECUTE_RULE(rule_statements);
	TK_MATCH(TK_R_CURLY);
	TK_MATCH(TK_EOL);

	return EXIT_SUCCESS;
}

/// 5
int rule_param_list(scanner_t scanner) {	
	// Param_list ->		  Param Param_n
	//						| eps .
	switch (TOKEN) {
		case TK_IDENTIFIER:
			EXECUTE_RULE(rule_param);
			EXECUTE_RULE(rule_param_n);
			break;
		default:
			// eps
			break;
	}
	return EXIT_SUCCESS;
}

/// 6
int rule_param_n(scanner_t scanner) {	
	// Param_n -> 			  comma Eol_opt Param Param_n
	//						| eps.
	switch (TOKEN) {
		case TK_COMMA:
			TK_NEXT();
			EXECUTE_RULE(rule_eol_opt);
			EXECUTE_RULE(rule_param);
			EXECUTE_RULE(rule_param_n);
			break;
		default:
			// eps
			break;
	}
	return EXIT_SUCCESS;
}

/// 7
int rule_param(scanner_t scanner) {
	// Param -> 			  id Typename
	TK_MATCH(TK_IDENTIFIER);
	EXECUTE_RULE(rule_typename);
	return EXIT_SUCCESS;
}


/// 8
int rule_return_list(scanner_t scanner) {
	// Return_list -> 	  	  l_parenthesis Return_n r_parenthesis
	//						| eps .
	switch (TOKEN) {
		case TK_L_PARENTHESIS:
			TK_NEXT();
			EXECUTE_RULE(rule_return_n);
			TK_MATCH(TK_R_PARENTHESIS);
			break;
		default:
			// eps
			break;
	}
	return EXIT_SUCCESS;
}

/// 9
int rule_return_n(scanner_t scanner) {	
	// Return_n -> 			  Returntype Type_n .
	EXECUTE_RULE(rule_returntype);
	EXECUTE_RULE(rule_type_n);
	return EXIT_SUCCESS;
}

/// 10
int rule_type_n(scanner_t scanner) {
	// Type_n -> 			  comma Eol_opt Typename Type_n
	//						| eps .
	switch (TOKEN) {
		case TK_COMMA:
			TK_NEXT();
			EXECUTE_RULE(rule_eol_opt);
			EXECUTE_RULE(rule_typename);
			EXECUTE_RULE(rule_type_n);
		default:
			// eps
			break;
	}
	return EXIT_SUCCESS;
}

/// 11
int rule_typename(scanner_t scanner) {
	// Typename ->	 		  float64
	//						| int
	//						| string
	//						| bool .
	TK_MATCH(TK_KEYW_FLOAT64, TK_KEYW_INT, TK_KEYW_STRING, TK_KEYW_BOOL);
	return EXIT_SUCCESS;
}

int rule_returntype(scanner_t scanner) {
	// Typename ->	 		  float64
	//						| int
	//						| string
	//						| bool
	//						| eps .
	switch (TOKEN) {
		case TK_KEYW_FLOAT64:
		case TK_KEYW_INT:
		case TK_KEYW_STRING:
		case TK_KEYW_BOOL:
			TK_NEXT();
			break;
		default:
			// eps
			break;
	}
	return EXIT_SUCCESS;
}


/// 12
int rule_statements(scanner_t scanner) {
	// Statements ->		  Def_Ass_Call Eol_opt_n Statements
	//						| Conditionals Eol_opt_n Statements
	//						| Iterative Eol_opt_n Statements
	//						| Return Eol_opt_n Statements
	//						| eps .
	switch (TOKEN) {
		case TK_UNDERSCORE:
		case TK_IDENTIFIER:
			EXECUTE_RULE(rule_def_ass_call);
			EXECUTE_RULE(rule_eol_opt_n);
			EXECUTE_RULE(rule_statements);
		case TK_KEYW_IF:
			EXECUTE_RULE(rule_conditionals);
			EXECUTE_RULE(rule_eol_opt_n);
			EXECUTE_RULE(rule_statements);
		case TK_KEYW_FOR:
			EXECUTE_RULE(rule_iterative);
			EXECUTE_RULE(rule_eol_opt_n);
			EXECUTE_RULE(rule_statements);
		case TK_KEYW_RETURN:
			EXECUTE_RULE(rule_return);
			EXECUTE_RULE(rule_eol_opt_n);
			EXECUTE_RULE(rule_statements);
			break;
		default:
			// eps
			break;
	}
	return EXIT_SUCCESS;
}

/// 14
int rule_def_ass_call(scanner_t scanner) {
	// Def_Ass_Call ->	  	  underscore Ass_Call eol
	//						| id Def_Ass_Call2 eol .
	// TODO rework this whole shit
	switch (TOKEN) {
		case TK_UNDERSCORE:
			TK_NEXT();
			EXECUTE_RULE(rule_ass_call);
			TK_MATCH(TK_EOL);
			break;
		case TK_IDENTIFIER:
			TK_NEXT();
			EXECUTE_RULE(rule_def_ass_call2);
			TK_MATCH(TK_EOL);
			break;
	}
	return EXIT_SUCCESS;
}

/// 15
int rule_ass_call(scanner_t scanner) {
	// Ass_Call ->			  l_parenthesis FunCall
	//						| comma Ids AssignOp Exprs_FunCall
	//						| AssignOp Exprs_FunCall .
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
			EXECUTE_RULE(rule_assignOp);
			EXECUTE_RULE(rule_exprs_funCall);
			break;
	}
	return EXIT_SUCCESS;
}

/// 16
int rule_def_ass_call2(scanner_t scanner) {
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
			EXECUTE_RULE(rule_assignOp);
			EXECUTE_RULE(rule_exprs_funCall);
			break;
	}
	return EXIT_SUCCESS;
}

/// 17
int rule_funCall(scanner_t scanner) {
	// FunCall ->			  Eol_opt Arguments r_parenthesis .
	EXECUTE_RULE(rule_eol_opt);
	EXECUTE_RULE(rule_Arguments);
	TK_MATCH(TK_R_PARENTHESIS);
	return EXIT_SUCCESS;
}

/// 18
int rule_var_define_opt(scanner_t scanner) {
	// Var_define -> 	  	  Var_define
	//						| eps .
	switch (TOKEN) {
		case TK_IDENTIFIER:
			EXECUTE_RULE(rule_var_define);
			break;
		default:
			// eps
			break;
	}
	return EXIT_SUCCESS;
}

int rule_var_define(scanner_t scanner) {
	// Var_define -> 	  	  id defineOp Expression
	//						| eps .
	switch (TOKEN) {
		case TK_IDENTIFIER:
			TK_NEXT();
			TK_MATCH(TK_VAR_INIT);
			EXECUTE_RULE(rule_expression);
			break;
		default:
			// eps
			break;
	}
	return EXIT_SUCCESS;
}

/// 19
int rule_conditionals(scanner_t scanner) {
	// Conditionals -> 		  Conditional Conditional_n eol.
	EXECUTE_RULE(rule_conditional);
	EXECUTE_RULE(rule_conditional_n);
	TK_MATCH(TK_EOL);
	return EXIT_SUCCESS;
}

/// 20
int rule_conditional_n(scanner_t scanner) {
	// Conditional_n ->		  else Else
	//						| eps .
	switch (TOKEN) {
		case TK_KEYW_ELSE:
			TK_NEXT();
			EXECUTE_RULE(rule_else);
			break;
		default:
			break;
	}
	return EXIT_SUCCESS;
}

/// 21
int rule_else(scanner_t scanner) {
	// Else -> 				  Conditional Conditional_n
	//						| l_curly eol Eol_opt_n Statements r_curly .
	switch (TOKEN) {
		case TK_KEYW_IF:
			EXECUTE_RULE(rule_conditional);
			break;
		case TK_L_CURLY:
			TK_NEXT();
			TK_MATCH(TK_EOL);
			EXECUTE_RULE(rule_eol_opt_n);
			EXECUTE_RULE(rule_statements);
			TK_MATCH(TK_R_CURLY);
		default:
			break;
	}
	return EXIT_SUCCESS;
}

/// 22
int rule_conditional(scanner_t scanner) {
	// Conditional -> 	  	  if Expression l_curly eol Eol_opt_n Statements r_curly .
	TK_NEXT();
	EXECUTE_RULE(rule_expression);
	TK_MATCH(TK_L_CURLY);
	TK_MATCH(TK_EOL);
	EXECUTE_RULE(rule_eol_opt_n);
	EXECUTE_RULE(rule_statements);
	TK_MATCH(TK_R_CURLY);
	return EXIT_SUCCESS;
}

/// 23
int rule_iterative(scanner_t scanner) {
	// Iterative -> 		  for Var_define_opt semicolon Expression semicolon
	//						  Assignment_opt l_curly eol Eol_opt_n Statements r_curly eol.
	TK_NEXT();
	EXECUTE_RULE(rule_var_define_opt);
	TK_MATCH(TK_SEMICOLON);
	EXECUTE_RULE(rule_expression);
	TK_MATCH(TK_SEMICOLON);
	EXECUTE_RULE(rule_assignment_opt);
	TK_MATCH(TK_L_CURLY);
	TK_MATCH(TK_EOL);
	EXECUTE_RULE(rule_eol_opt_n);
	EXECUTE_RULE(rule_statements);
	TK_MATCH(TK_R_CURLY);
	TK_MATCH(TK_EOL);
	return EXIT_SUCCESS;
}

/// 24
int rule_assignment_opt(scanner_t scanner) {
	// Assignment -> 	  	  Assignment
	//						| eps .
	switch (TOKEN) {
		case TK_IDENTIFIER:
		case TK_UNDERSCORE:
			EXECUTE_RULE(rule_assignment);
			break;
		default:
			// eps
			break;
	}
	return EXIT_SUCCESS;
}

/// 24
int rule_assignment(scanner_t scanner) {
	// Assignment -> 	  	  Ids AssignOp Exprs_FunCall .
	EXECUTE_RULE(rule_ids);
	EXECUTE_RULE(rule_assignOp);
	EXECUTE_RULE(rule_exprs_funCall);
	return EXIT_SUCCESS;
}

/// 25
int rule_ids(scanner_t scanner) {
	// Ids -> 				  Id Id_n .
	EXECUTE_RULE(rule_id);
	EXECUTE_RULE(rule_id_n);
	return EXIT_SUCCESS;
}

/// 26
int rule_id_n(scanner_t scanner) {
	// Id_n ->	 			  comma Id Id_n
	//						| eps .
	switch (TOKEN) {
		case TK_COMMA:
			TK_NEXT();
			EXECUTE_RULE(rule_id);
			EXECUTE_RULE(rule_id_n);
		default:
			// eps
			break;
	}
	return EXIT_SUCCESS;
}

/// 27
int rule_id(scanner_t scanner) {
	// Id -> 				  id
	//						| underscore .
	TK_MATCH(TK_IDENTIFIER, TK_UNDERSCORE);
	return EXIT_SUCCESS;
}

/// 28
int rule_assignOp(scanner_t scanner) {
	// AssignOp -> 			  plus_assign
	//						| minus_assing
	//						| multiply_assign
	//						| divide_assign
	//						| assign .
	TK_MATCH(TK_PLUS_ASSIGN, TK_MINUS_ASSIGN, TK_MULTIPLY_ASSIGN, TK_DIVIDE_ASSIGN, TK_ASSIGN);
	return EXIT_SUCCESS;
}

/// 29
int rule_exprs_funCall(scanner_t scanner) {
	// Exprs_FunCall ->		  Expression
	//						| FunctionCall .

	// TODO: expre vs funcall - BIG TODO btw

	EXECUTE_RULE(rule_functionCall);

	return EXIT_SUCCESS;
}

/// 30
int rule_functionCall(scanner_t scanner) {
	// FunctionCall -> 		  id l_parenthesis Eol_opt Arguments r_parenthesis .
	TK_MATCH(TK_IDENTIFIER);
	TK_MATCH(TK_L_PARENTHESIS);
	EXECUTE_RULE(rule_eol_opt);
	EXECUTE_RULE(rule_Arguments);
	TK_MATCH(TK_R_PARENTHESIS);
	return EXIT_SUCCESS;
}

/// 31
int rule_Arguments(scanner_t scanner) {
	// Arguments ->			  Argument Argument_n
	//						| eps .
	switch (TOKEN) {
		case TK_IDENTIFIER:
		case TK_INT_LIT:
		case TK_FLOAT_LIT:
		case TK_STR_LIT:
		case TK_KEYW_TRUE:
		case TK_KEYW_FALSE:
			EXECUTE_RULE(rule_Argument);
			EXECUTE_RULE(rule_Argument_n);
		default:
			break;
	}
	return EXIT_SUCCESS;
}

/// 32
int rule_Argument_n(scanner_t scanner) {
	// Argument_n -> 	  	  comma Eol_opt Argument Argument_n
	//						| eps .
	switch (TOKEN) {
		case TK_COMMA:
			TK_NEXT();
			EXECUTE_RULE(rule_eol_opt);
			EXECUTE_RULE(rule_Argument);
			EXECUTE_RULE(rule_Argument_n);
		default:
			// eps
			break;
	}
	return EXIT_SUCCESS;
}

/// 33
int rule_Argument(scanner_t scanner) {
	// Argument -> 			  id
	//						| Literal .
	switch (TOKEN) {
		case TK_IDENTIFIER:
			TK_NEXT();
		default:
			EXECUTE_RULE(rule_literal);
			break;
	}
	return EXIT_SUCCESS;
}

/// 34
int rule_return(scanner_t scanner) {
	// Return -> 			  return Expressions eol.
	TK_MATCH(TK_KEYW_RETURN);
	EXECUTE_RULE(rule_expressions);
	TK_MATCH(TK_EOL);
	return EXIT_SUCCESS;
}

/// 35
int rule_expressions(scanner_t scanner) {
	// Expressions -> 	  	  Expression Expression_n .
	EXECUTE_RULE(rule_expression);
	EXECUTE_RULE(rule_expression_n);
	return EXIT_SUCCESS;
}

/// 36
int rule_expression_n(scanner_t scanner) {
	// Expression_n ->	  	  comma Expression Expression_n
	//						| eps .
	switch (TOKEN) {
		case TK_COMMA:
			TK_NEXT();
			EXECUTE_RULE(rule_expression);
			EXECUTE_RULE(rule_expression_n);
		default:
			// eps
			break;
	}
	return EXIT_SUCCESS;
}

/// 37
int rule_expression(scanner_t scanner) {
	// Expression -> 	  	  Term
	//						| Term BinaryOp Term
	//						| l_parenthesis Eol_opt Term r_parenthesis .
	// TODO: expression shits and don't forget other shits that are kinda expression related
	return EXIT_SUCCESS;
}

int rule_unaryOp(scanner_t scanner) {
	TK_MATCH(TK_PLUS, TK_MINUS, TK_NOT);
	return EXIT_SUCCESS;
}

int rule_term(scanner_t scanner) {
	// adhoc rule
	TK_MATCH(TK_IDENTIFIER, TK_INT_LIT, TK_FLOAT_LIT, TK_STR_LIT, TK_KEYW_TRUE, TK_KEYW_FALSE);
	return EXIT_SUCCESS;
}

int rule_binaryOp(scanner_t scanner) {
	// adhoc rule
	TK_MATCH(TK_PLUS, TK_MINUS, TK_MULTIPLY, TK_DIVIDE, TK_PLUS_ASSIGN, TK_MINUS_ASSIGN, TK_MULTIPLY_ASSIGN, TK_DIVIDE_ASSIGN, TK_ASSIGN, TK_EQUAL, TK_NOT_EQUAL, TK_LESS, TK_GREATER, TK_LESS_EQUAL, TK_GREATER_EQUAL, TK_OR, TK_AND);
	return EXIT_SUCCESS;
}

/// 38
int rule_literal(scanner_t scanner) {
	// Literal -> 			  intLit
	//						| floatLit
	//						| stringLit
	//						| true
	//						| false.
	TK_MATCH(TK_INT_LIT, TK_FLOAT_LIT, TK_STR_LIT, TK_KEYW_TRUE, TK_KEYW_FALSE);
	return EXIT_SUCCESS;
}

int rule_eol_opt_n(scanner_t scanner) {
	// adhoc rule
	// Eol_opt ->			  eol Eol_opt_n
	//						| eps .
	switch (TOKEN) {
		case TK_EOL:
			TK_NEXT();
			EXECUTE_RULE(rule_eol_opt_n);
			break;
		default:
			// eps
			break;
	}
	return EXIT_SUCCESS;
}

/// 39
int rule_eol_opt(scanner_t scanner) {	
	// Eol_opt ->			  eol
	//						| eps .
	switch (TOKEN) {
		case TK_EOL:
			TK_NEXT();
			break;
		default:
			// eps
			break;
	}
	return EXIT_SUCCESS;
}
