#include "rules.h"

#include <assert.h>
#include <stdio.h>

#include "parser.h"
#include "rulemacros.h"
#include "semantics.h"

////// Forward declarations

// defined in precedence.c
int parse_expr(parser_t parser);

static int rule_program(parser_t parser);
static int rule_prolog(parser_t parser);
static int rule_functions(parser_t parser);
static int rule_function(parser_t parser);
static int rule_param_list(parser_t parser);
static int rule_param_n(parser_t parser);
static int rule_param(parser_t parser);
static int rule_return_list(parser_t parser);
static int rule_return_n(parser_t parser);
static int rule_type_n(parser_t parser);
static int rule_typename(parser_t parser);
static int rule_returntype(parser_t parser);
static int rule_statements(parser_t parser);
static int rule_def_ass_call(parser_t parser);
static int rule_var_define_opt(parser_t parser);
static int rule_var_define(parser_t parser);
static int rule_conditionals(parser_t parser);
static int rule_conditional_n(parser_t parser);
static int rule_else(parser_t parser);
static int rule_conditional(parser_t parser);
static int rule_iterative(parser_t parser);
static int rule_assignment(parser_t parser);
static int rule_assignment_opt(parser_t parser);
static int rule_assignment_list(parser_t parser);
static int rule_ids(parser_t parser);
static int rule_id_n(parser_t parser);
static int rule_id(parser_t parser);
static int rule_assign_op(parser_t parser);
static int rule_exprs_funcall(parser_t parser);
static int rule_functionCall(parser_t parser);
static int rule_arguments(parser_t parser);
static int rule_argument_n(parser_t parser);
static int rule_argument(parser_t parser);
static int rule_return(parser_t parser);
static int rule_expressions_opt(parser_t parser);
static int rule_expressions(parser_t parser);
static int rule_expression_n(parser_t parser);
static int rule_expression(parser_t parser);
static int rule_literal(parser_t parser);

////// Root

int rule_root(parser_t parser) {
	EXECUTE_RULE(parser_setup);
	SEM_ACTION(sem_define_builtin_functions);
	EXECUTE_RULE(rule_program);
	SEM_ACTION(sem_main_defined);
	return EXIT_SUCCESS;
}

////// Actual rule definitions

/// 1
int rule_program(parser_t parser) {
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
int rule_prolog(parser_t parser) {
	// Prolog -> 		  package main eol .
	TK_MATCH(TK_KEYW_PACKAGE);
	TK_MATCH(TK_KEYW_MAIN);
	TK_MATCH(TK_EOL);
	return EXIT_SUCCESS;
}

/// 3
int rule_functions(parser_t parser) {
	// Functions -> 		  Function Eol_opt_n Functions
	//						| eps .
	switch (TOKEN_TYPE) {
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
int rule_function(parser_t parser) {
	// Function -> 		  func id l_parenthesis Param_list r_parenthesis
	//					  Return_list l_curly eol Eol_opt_n Statements r_curly eol .
	TK_MATCH(TK_KEYW_FUNC);
	TK_TEST(TK_IDENTIFIER, TK_KEYW_MAIN);
	SEM_ACTION(sem_func_define);
	TK_NEXT();
	TK_MATCH(TK_L_PARENTHESIS);
	SEM_ACTION(sem_enter_scope);
	EXECUTE_RULE(rule_param_list);
	TK_MATCH(TK_R_PARENTHESIS);
	EXECUTE_RULE(rule_return_list);
	/*************************/
	/**/ FIRST_PASS_END(); /**/
	/*************************/
	TK_MATCH(TK_L_CURLY);
	TK_MATCH(TK_EOL);
	EXECUTE_RULE(rule_eol_opt_n);
	SEM_ACTION(sem_func_stmts_begin);
	EXECUTE_RULE(rule_statements);
	TK_MATCH(TK_R_CURLY);
	SEM_ACTION(sem_exit_scope);
	SEM_ACTION(sem_func_has_return_stmt);
	return EXIT_SUCCESS;
}

/// 5
int rule_param_list(parser_t parser) {
	// Param_list ->		  Param Param_n
	//						| eps .
	switch (TOKEN_TYPE) {
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
int rule_param_n(parser_t parser) {
	// Param_n -> 			  comma Eol_opt Param Param_n
	//						| eps.
	switch (TOKEN_TYPE) {
		case TK_COMMA:
			TK_NEXT();
			EXECUTE_RULE(rule_eol_opt_n);
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
int rule_param(parser_t parser) {
	// Param -> 			  id float64
	//						| id int
	//						| id string
	//						| id bool .
	TK_TEST(TK_IDENTIFIER);
	SEM_ACTION(sem_func_declare_param);
	TK_NEXT();
	TK_TEST(TK_KEYW_FLOAT64, TK_KEYW_INT, TK_KEYW_STRING, TK_KEYW_BOOL);
	SEM_ACTION(sem_func_add_param_type);
	TK_NEXT();
	return EXIT_SUCCESS;
}


/// 8
int rule_return_list(parser_t parser) {
	// Return_list -> 	  	  l_parenthesis Return_n r_parenthesis
	//						| eps .
	switch (TOKEN_TYPE) {
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
int rule_return_n(parser_t parser) {
	// Return_n -> 			  Returntype Type_n .
	EXECUTE_RULE(rule_returntype);
	EXECUTE_RULE(rule_type_n);
	return EXIT_SUCCESS;
}

/// 10
int rule_type_n(parser_t parser) {
	// Type_n -> 			  comma Eol_opt Typename Type_n
	//						| eps .
	switch (TOKEN_TYPE) {
		case TK_COMMA:
			TK_NEXT();
			EXECUTE_RULE(rule_eol_opt_n);
			EXECUTE_RULE(rule_typename);
			EXECUTE_RULE(rule_type_n);
		default:
			// eps
			break;
	}
	return EXIT_SUCCESS;
}

/// 11
int rule_typename(parser_t parser) {
	// Typename ->	 		  float64
	//						| int
	//						| string
	//						| bool .
	TK_TEST(TK_KEYW_FLOAT64, TK_KEYW_INT, TK_KEYW_STRING, TK_KEYW_BOOL);
	SEM_ACTION(sem_func_add_return_type);
	TK_NEXT();
	return EXIT_SUCCESS;
}

int rule_returntype(parser_t parser) {
	// Typename ->	 		  float64
	//						| int
	//						| string
	//						| bool
	//						| eps .
	switch (TOKEN_TYPE) {
		case TK_KEYW_FLOAT64:
		case TK_KEYW_INT:
		case TK_KEYW_STRING:
		case TK_KEYW_BOOL:
			SEM_ACTION(sem_func_add_return_type);
			TK_NEXT();
			break;
		default:
			// eps
			break;
	}
	return EXIT_SUCCESS;
}


/// 12
int rule_statements(parser_t parser) {
	// Statements ->		  Def_Ass_Call Eol_opt_n Statements
	//						| Conditionals Eol_opt_n Statements
	//						| Iterative Eol_opt_n Statements
	//						| Return Eol_opt_n Statements
	//						| eps .
	switch (TOKEN_TYPE) {
		case TK_UNDERSCORE:
		case TK_IDENTIFIER:
			EXECUTE_RULE(rule_def_ass_call);
			EXECUTE_RULE(rule_eol_opt_n);
			EXECUTE_RULE(rule_statements);
			break;
		case TK_KEYW_IF:
			EXECUTE_RULE(rule_conditionals);
			EXECUTE_RULE(rule_eol_opt_n);
			EXECUTE_RULE(rule_statements);
			break;

		case TK_KEYW_FOR:
			EXECUTE_RULE(rule_iterative);
			EXECUTE_RULE(rule_eol_opt_n);
			EXECUTE_RULE(rule_statements);
			break;

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
int rule_def_ass_call(parser_t parser) {
	// Def_Ass_Call ->	  	  Var_define eol
	//						| FunctionCall eol
	//						| Assignment eol .
	if (TOKEN_SECOND_TYPE == TK_VAR_INIT) {
		EXECUTE_RULE(rule_var_define);
	} else if (TOKEN_SECOND_TYPE == TK_L_PARENTHESIS) {
		EXECUTE_RULE(rule_functionCall);
		SEM_ACTION(sem_call_no_return);
	} else {
		EXECUTE_RULE(rule_assignment);
	}
	TK_MATCH(TK_EOL);
	return EXIT_SUCCESS;
}

/// 18
int rule_var_define_opt(parser_t parser) {
	// Var_define -> 	  	  Var_define
	//						| eps .

	switch (TOKEN_TYPE) {
		case TK_IDENTIFIER:
			EXECUTE_RULE(rule_var_define);
			break;
		default:
			// eps
			break;
	}
	return EXIT_SUCCESS;
}

int rule_var_define(parser_t parser) {
	// Var_define -> 	  	  id defineOp Expression
	SEM_ACTION(sem_define_begin);
	TK_TEST(TK_IDENTIFIER);
	SEM_ACTION(sem_var_define);
	TK_NEXT();
	TK_MATCH(TK_VAR_INIT);
	EXECUTE_RULE(rule_expression);
	SEM_ACTION(sem_var_define_type);
	return EXIT_SUCCESS;
}

/// 19
int rule_conditionals(parser_t parser) {
	// Conditionals -> 		  Conditional Conditional_n eol.
	EXECUTE_RULE(rule_conditional);
	EXECUTE_RULE(rule_conditional_n);
	TK_MATCH(TK_EOL);
	return EXIT_SUCCESS;
}

/// 20
int rule_conditional_n(parser_t parser) {
	// Conditional_n ->		  else Else
	//						| eps .
	switch (TOKEN_TYPE) {
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
int rule_else(parser_t parser) {
	// Else -> 				  Conditional Conditional_n
	//						| l_curly eol Eol_opt_n Statements r_curly .
	switch (TOKEN_TYPE) {
		case TK_KEYW_IF:
			EXECUTE_RULE(rule_conditional);
			EXECUTE_RULE(rule_conditional_n);
			break;
		case TK_L_CURLY:
			TK_NEXT();
			TK_MATCH(TK_EOL);
			SEM_ACTION(sem_enter_scope);
			EXECUTE_RULE(rule_eol_opt_n);
			EXECUTE_RULE(rule_statements);
			TK_MATCH(TK_R_CURLY);
			SEM_ACTION(sem_exit_scope);
		default:
			break;
	}
	return EXIT_SUCCESS;
}

/// 22
int rule_conditional(parser_t parser) {
	// Conditional -> 	  	  if Expression l_curly eol Eol_opt_n Statements r_curly .
	SEM_ACTION(sem_conditional_begin);
	TK_NEXT();
	EXECUTE_RULE(rule_expression);
	SEM_ACTION(sem_bool_condiiton);
	TK_MATCH(TK_L_CURLY);
	TK_MATCH(TK_EOL);
	SEM_ACTION(sem_enter_scope);
	EXECUTE_RULE(rule_eol_opt_n);
	EXECUTE_RULE(rule_statements);
	TK_MATCH(TK_R_CURLY);
	SEM_ACTION(sem_exit_scope);
	return EXIT_SUCCESS;
}

/// 23
int rule_iterative(parser_t parser) {
	// Iterative -> 		  for Var_define_opt semicolon Expression semicolon
	//						  Assignment_opt l_curly eol Eol_opt_n Statements r_curly eol.
	SEM_ACTION(sem_iterative_begin);
	TK_NEXT();
	SEM_ACTION(sem_enter_scope);
	EXECUTE_RULE(rule_var_define_opt);
	TK_MATCH(TK_SEMICOLON);
	EXECUTE_RULE(rule_expression);
	SEM_ACTION(sem_bool_condiiton);
	TK_MATCH(TK_SEMICOLON);
	EXECUTE_RULE(rule_assignment_opt);
	TK_MATCH(TK_L_CURLY);
	TK_MATCH(TK_EOL);
	SEM_ACTION(sem_enter_scope);
	EXECUTE_RULE(rule_eol_opt_n);
	EXECUTE_RULE(rule_statements);
	TK_MATCH(TK_R_CURLY);
	TK_MATCH(TK_EOL);
	SEM_ACTION(sem_exit_scope);
	SEM_ACTION(sem_exit_scope);
	return EXIT_SUCCESS;
}

/// 24
int rule_assignment_opt(parser_t parser) {
	// Assignment -> 	  	  Assignment
	//						| eps .
	switch (TOKEN_TYPE) {
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
int rule_assignment(parser_t parser) {
	// Assignment -> 	  	  Assignment_list
	//						| Id AssignOp Exprs_FunCall .
	SEM_ACTION(sem_assign_begin);
	if (TOKEN_SECOND_TYPE == TK_COMMA) {
		EXECUTE_RULE(rule_assignment_list);
	} else {
		EXECUTE_RULE(rule_id);
		EXECUTE_RULE(rule_assign_op);
		EXECUTE_RULE(rule_exprs_funcall);
	}
	return EXIT_SUCCESS;
}

int rule_assignment_list(parser_t parser) {
	EXECUTE_RULE(rule_ids);
	TK_MATCH(TK_ASSIGN);
	EXECUTE_RULE(rule_exprs_funcall);
	return EXIT_SUCCESS;
}

/// 25
int rule_ids(parser_t parser) {
	// Ids -> 				  Id Id_n .
	EXECUTE_RULE(rule_id);
	EXECUTE_RULE(rule_id_n);
	return EXIT_SUCCESS;
}

/// 26
int rule_id_n(parser_t parser) {
	// Id_n ->	 			  comma Id Id_n
	//						| eps .
	switch (TOKEN_TYPE) {
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
int rule_id(parser_t parser) {
	// Id -> 				  id
	//						| underscore .
	SEM_ACTION(sem_id_begin);
	TK_TEST(TK_IDENTIFIER, TK_UNDERSCORE);
	if (TK_IDENTIFIER) {
		SEM_ACTION(sem_var_check);
	}
	TK_NEXT();
	return EXIT_SUCCESS;
}

/// 28
int rule_assign_op(parser_t parser) {
	// AssignOp -> 			  plus_assign
	//						| minus_assing
	//						| multiply_assign
	//						| divide_assign
	//						| assign .
	TK_MATCH(TK_PLUS_ASSIGN, TK_MINUS_ASSIGN, TK_MULTIPLY_ASSIGN, TK_DIVIDE_ASSIGN, TK_ASSIGN);
	EXECUTE_RULE(rule_eol_opt_n);
	return EXIT_SUCCESS;
}

/// 29
int rule_exprs_funcall(parser_t parser) {
	// Exprs_FunCall ->		  Expression
	//						| FunctionCall .
	if (TOKEN_SECOND_TYPE == TK_L_PARENTHESIS) {
		EXECUTE_RULE(rule_functionCall);
	} else {
		EXECUTE_RULE(rule_expressions);
		SEM_ACTION(sem_assign_expr_count);
	}
	return EXIT_SUCCESS;
}

/// 30
int rule_functionCall(parser_t parser) {
	// FunctionCall -> 		  id l_parenthesis Eol_opt Arguments r_parenthesis .
	SEM_ACTION(sem_call_begin);
	TK_TEST(TK_IDENTIFIER);
	SEM_ACTION(sem_func_callable);
	SEM_ACTION(sem_assignment_call_return);
	TK_NEXT();
	TK_MATCH(TK_L_PARENTHESIS);
	EXECUTE_RULE(rule_eol_opt_n);
	EXECUTE_RULE(rule_arguments);
	SEM_ACTION(sem_call_argument_count);
	TK_MATCH(TK_R_PARENTHESIS);
	return EXIT_SUCCESS;
}

/// 31
int rule_arguments(parser_t parser) {
	// Arguments ->			  Argument Argument_n
	//						| eps .
	SEM_ACTION(sem_arguments_begin);
	switch (TOKEN_TYPE) {
		case TK_IDENTIFIER:
		case TK_INT_LIT:
		case TK_FLOAT_LIT:
		case TK_STR_LIT:
		case TK_KEYW_TRUE:
		case TK_KEYW_FALSE:
			EXECUTE_RULE(rule_argument);
			EXECUTE_RULE(rule_argument_n);
			break;
		case TK_R_PARENTHESIS:
			// eps
			break;
		default:
			TK_TEST(TK_IDENTIFIER, TK_INT_LIT, TK_FLOAT_LIT, TK_STR_LIT, TK_KEYW_TRUE, TK_KEYW_FALSE);
			break;
	}
	return EXIT_SUCCESS;
}

/// 32
int rule_argument_n(parser_t parser) {
	// Argument_n -> 	  	  comma Eol_opt Argument Argument_n
	//						| eps .

	switch (TOKEN_TYPE) {
		case TK_COMMA:
			TK_NEXT();
			EXECUTE_RULE(rule_eol_opt_n);
			EXECUTE_RULE(rule_argument);
			EXECUTE_RULE(rule_argument_n);
		default:
			// eps
			break;
	}
	return EXIT_SUCCESS;
}

/// 33
int rule_argument(parser_t parser) {
	// Argument -> 			  id
	//						| Literal .
	SEM_ACTION(sem_argument_begin);
	switch (TOKEN_TYPE) {
		case TK_IDENTIFIER:
			TK_NEXT();
			break;
		default:
			EXECUTE_RULE(rule_literal);
			break;
	}
	return EXIT_SUCCESS;
}

/// 34
int rule_return(parser_t parser) {
	// Return -> 			  return Expressions_opt eol.
	SEM_ACTION(sem_return_begin);
	TK_MATCH(TK_KEYW_RETURN);
	EXECUTE_RULE(rule_expressions_opt);
	SEM_ACTION(sem_return_expr_count);
	TK_MATCH(TK_EOL);
	return EXIT_SUCCESS;
}


int rule_expressions_opt(parser_t parser) {
	// Expressions -> 	  	  Expression Expression_n
	//						| eps .
	switch (TOKEN_TYPE) {
		case TK_EOL:
			break;
		default:
			EXECUTE_RULE(rule_expressions);
			break;
	}
	return EXIT_SUCCESS;
}

/// 35
int rule_expressions(parser_t parser) {
	// Expressions -> 	  	  Expression Expression_n .
	EXECUTE_RULE(rule_expression);
	EXECUTE_RULE(rule_expression_n);
	return EXIT_SUCCESS;
}

/// 36
int rule_expression_n(parser_t parser) {
	// Expression_n ->	  	  comma Expression Expression_n
	//						| eps .
	switch (TOKEN_TYPE) {
		case TK_EOL:
			break;
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
int rule_expression(parser_t parser) {
	// Expression -> 	  	 expression .
	SEM_ACTION(sem_expression_begin);
	EXECUTE_RULE(parse_expr);
	SEM_ACTION(sem_return_expr_type_compat);
	SEM_ACTION(sem_assign_expr_type_compat);
	return EXIT_SUCCESS;
}

/// 38
int rule_literal(parser_t parser) {
	// Literal -> 			  intLit
	//						| floatLit
	//						| stringLit
	//						| true
	//						| false.
	TK_MATCH(TK_INT_LIT, TK_FLOAT_LIT, TK_STR_LIT, TK_KEYW_TRUE, TK_KEYW_FALSE);
	return EXIT_SUCCESS;
}

int rule_eol_opt_n(parser_t parser) {
	// adhoc rule
	// Eol_opt ->			  eol Eol_opt_n
	//						| eps .
	switch (TOKEN_TYPE) {
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
