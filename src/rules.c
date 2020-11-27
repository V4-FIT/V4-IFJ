#include "rules.h"

#include <assert.h>
#include <stdio.h>

#include "rulemacros.h"
#include "precedence.h"

////// Forward declarations

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
static int rule_ass_call(parser_t parser);
static int rule_def_ass_call2(parser_t parser);
static int rule_funCall(parser_t parser);
static int rule_var_define_opt(parser_t parser);
static int rule_var_define(parser_t parser);
static int rule_conditionals(parser_t parser);
static int rule_conditional_n(parser_t parser);
static int rule_else(parser_t parser);
static int rule_conditional(parser_t parser);
static int rule_iterative(parser_t parser);
static int rule_assignment(parser_t parser);
static int rule_assignment_opt(parser_t parser);
static int rule_ids(parser_t parser);
static int rule_id_n(parser_t parser);
static int rule_id(parser_t parser);
static int rule_assignOp(parser_t parser);
static int rule_exprs_funCall(parser_t parser);
static int rule_functionCall(parser_t parser);
static int rule_Arguments(parser_t parser);
static int rule_Argument_n(parser_t parser);
static int rule_Argument(parser_t parser);
static int rule_return(parser_t parser);
static int rule_expressions(parser_t parser);
static int rule_expression_n(parser_t parser);
static int rule_expression(parser_t parser);
static int rule_literal(parser_t parser);
static int rule_eol_opt_n(parser_t parser);
static int rule_eol_opt(parser_t parser);

// adhoc rules
static int rule_term(parser_t parser);
static int rule_binaryOp(parser_t parser);
static int rule_unaryOp(parser_t parser);

////// Root

int rule_root(parser_t parser) {
	TK_NEXT();
	EXECUTE_RULE(rule_program);
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
	SEM_DEFINE_FUNC();
	TK_NEXT();

	TK_MATCH(TK_L_PARENTHESIS);
	EXECUTE_RULE(rule_param_list);
	TK_MATCH(TK_R_PARENTHESIS);

	EXECUTE_RULE(rule_return_list);

	TK_MATCH(TK_L_CURLY);
	TK_MATCH(TK_EOL);
	symtable_enter_scope(parser->symtable);
	EXECUTE_RULE(rule_statements);
	TK_MATCH(TK_R_CURLY);
	TK_MATCH(TK_EOL);
	symtable_exit_scope(parser->symtable);

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
int rule_param(parser_t parser) {
	// Param -> 			  id Typename
	TK_MATCH(TK_IDENTIFIER);
	EXECUTE_RULE(rule_typename);
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
int rule_typename(parser_t parser) {
	// Typename ->	 		  float64
	//						| int
	//						| string
	//						| bool .
	TK_MATCH(TK_KEYW_FLOAT64, TK_KEYW_INT, TK_KEYW_STRING, TK_KEYW_BOOL);
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
			// EXECUTE_RULE(rule_var_define);
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
int rule_def_ass_call(parser_t parser) {
	// Def_Ass_Call ->	  	  underscore Ass_Call eol
	//						| id Def_Ass_Call2 eol .
	// TODO rework this whole shit

	switch (TOKEN_TYPE) {
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
int rule_ass_call(parser_t parser) {
	// Ass_Call ->			  l_parenthesis FunCall
	//						| comma Ids AssignOp Exprs_FunCall
	//						| AssignOp Exprs_FunCall .
	switch (TOKEN_TYPE) {
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
int rule_def_ass_call2(parser_t parser) {
	// Def_Ass_Call2 ->		  l_parenthesis FunCall
	//						| defineOp Expression
	//						| comma Ids AssignOp Exprs_FunCall .
	switch (TOKEN_TYPE) {
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
int rule_funCall(parser_t parser) {
	// FunCall ->			  Eol_opt Arguments r_parenthesis .
	EXECUTE_RULE(rule_eol_opt);
	EXECUTE_RULE(rule_Arguments);
	TK_MATCH(TK_R_PARENTHESIS);
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
	//						| eps .
	switch (TOKEN_TYPE) {
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
			break;
		case TK_L_CURLY:
			TK_NEXT();
			TK_MATCH(TK_EOL);
			symtable_enter_scope(parser->symtable);
			EXECUTE_RULE(rule_eol_opt_n);
			EXECUTE_RULE(rule_statements);
			TK_MATCH(TK_R_CURLY);
			symtable_exit_scope(parser->symtable);
		default:
			break;
	}
	return EXIT_SUCCESS;
}

/// 22
int rule_conditional(parser_t parser) {
	// Conditional -> 	  	  if Expression l_curly eol Eol_opt_n Statements r_curly .
	TK_NEXT();
	EXECUTE_RULE(rule_expression);
	TK_MATCH(TK_L_CURLY);
	TK_MATCH(TK_EOL);
	symtable_enter_scope(parser->symtable);
	EXECUTE_RULE(rule_eol_opt_n);
	EXECUTE_RULE(rule_statements);
	TK_MATCH(TK_R_CURLY);
	symtable_exit_scope(parser->symtable);
	return EXIT_SUCCESS;
}

/// 23
int rule_iterative(parser_t parser) {
	// Iterative -> 		  for Var_define_opt semicolon Expression semicolon
	//						  Assignment_opt l_curly eol Eol_opt_n Statements r_curly eol.
	TK_NEXT();
	symtable_enter_scope(parser->symtable);
	EXECUTE_RULE(rule_var_define_opt);
	TK_MATCH(TK_SEMICOLON);
	EXECUTE_RULE(rule_expression);
	TK_MATCH(TK_SEMICOLON);
	EXECUTE_RULE(rule_assignment_opt);
	TK_MATCH(TK_L_CURLY);
	TK_MATCH(TK_EOL);
	symtable_enter_scope(parser->symtable);
	EXECUTE_RULE(rule_eol_opt_n);
	EXECUTE_RULE(rule_statements);
	TK_MATCH(TK_R_CURLY);
	TK_MATCH(TK_EOL);
	symtable_exit_scope(parser->symtable);
	symtable_exit_scope(parser->symtable);
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
	// Assignment -> 	  	  Ids AssignOp Exprs_FunCall .
	EXECUTE_RULE(rule_ids);
	EXECUTE_RULE(rule_assignOp);
	EXECUTE_RULE(rule_exprs_funCall);
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
	TK_MATCH(TK_IDENTIFIER, TK_UNDERSCORE);
	return EXIT_SUCCESS;
}

/// 28
int rule_assignOp(parser_t parser) {
	// AssignOp -> 			  plus_assign
	//						| minus_assing
	//						| multiply_assign
	//						| divide_assign
	//						| assign .
	TK_MATCH(TK_PLUS_ASSIGN, TK_MINUS_ASSIGN, TK_MULTIPLY_ASSIGN, TK_DIVIDE_ASSIGN, TK_ASSIGN);
	return EXIT_SUCCESS;
}

/// 29
int rule_exprs_funCall(parser_t parser) {
	// Exprs_FunCall ->		  Expression
	//						| FunctionCall .

	// TODO: expre vs funcall - BIG TODO btw

	token_t t = token_copy(scanner_token(parser->scanner));
	if (t == NULL) {
		fprintf(stderr, "ERROR: malloc failed\n");
		return ERROR_MISC;
	}

	if (TOKEN_TYPE == TK_IDENTIFIER) {
		TK_NEXT();
		if (TOKEN_TYPE == TK_L_PARENTHESIS) {
			EXECUTE_RULE(rule_funCall);
		} else {
			int res = parse_expr(t, parser->scanner);
			if (res == EXIT_SUCCESS) {
				EXECUTE_RULE(rule_expression_n);
			}
			token_free(t);
			return res;
		}
	} else {
		// TK_NEXT();
		// int res = parse_expr(t, parser->scanner);
		// printf("res: %d", res);

		EXECUTE_RULE(rule_expressions);
	}
	token_free(t);

	return EXIT_SUCCESS;
}

/// 30
int rule_functionCall(parser_t parser) {
	// FunctionCall -> 		  id l_parenthesis Eol_opt Arguments r_parenthesis .
	// TK_MATCH(TK_IDENTIFIER);
	// TK_MATCH(TK_L_PARENTHESIS);

	printf("FUN CALL\n");

	EXECUTE_RULE(rule_eol_opt);
	EXECUTE_RULE(rule_Arguments);
	TK_MATCH(TK_R_PARENTHESIS);
	return EXIT_SUCCESS;
}

/// 31
int rule_Arguments(parser_t parser) {
	// Arguments ->			  Argument Argument_n
	//						| eps .
	switch (TOKEN_TYPE) {
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
int rule_Argument_n(parser_t parser) {
	// Argument_n -> 	  	  comma Eol_opt Argument Argument_n
	//						| eps .
	switch (TOKEN_TYPE) {
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
int rule_Argument(parser_t parser) {
	// Argument -> 			  id
	//						| Literal .
	switch (TOKEN_TYPE) {
		case TK_IDENTIFIER:
			TK_NEXT();
		default:
			EXECUTE_RULE(rule_literal);
			break;
	}
	return EXIT_SUCCESS;
}

/// 34
int rule_return(parser_t parser) {
	// Return -> 			  return Expressions eol.
	TK_MATCH(TK_KEYW_RETURN);
	EXECUTE_RULE(rule_expressions);
	TK_MATCH(TK_EOL);
	return EXIT_SUCCESS;
}

/// 35
int rule_expressions(parser_t parser) {
	// Expressions -> 	  	  Expression Expression_n .
	EXECUTE_RULE(rule_expression);
	EXECUTE_RULE(rule_expression_n);
	printf("expressions done\n");
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
	// Expression -> 	  	  Term
	//						| Term BinaryOp Term
	//						| l_parenthesis Eol_opt Term r_parenthesis .
	// TODO: expression shits and don't forget other shits that are kinda expression related
	token_t t = token_copy(scanner_token(parser->scanner));
	if (t == NULL) {
		fprintf(stderr, "ERROR: malloc failed\n");
		return ERROR_MISC;
	}
	TK_NEXT();
	int res = parse_expr(t, parser->scanner);
	token_free(t);

	return res;
}

int rule_unaryOp(parser_t parser) {
	TK_MATCH(TK_PLUS, TK_MINUS, TK_NOT);
	return EXIT_SUCCESS;
}

int rule_term(parser_t parser) {
	// adhoc rule
	TK_MATCH(TK_IDENTIFIER, TK_INT_LIT, TK_FLOAT_LIT, TK_STR_LIT, TK_KEYW_TRUE, TK_KEYW_FALSE);
	return EXIT_SUCCESS;
}

int rule_binaryOp(parser_t parser) {
	// adhoc rule
	TK_MATCH(TK_PLUS, TK_MINUS, TK_MULTIPLY, TK_DIVIDE, TK_PLUS_ASSIGN, TK_MINUS_ASSIGN, TK_MULTIPLY_ASSIGN, TK_DIVIDE_ASSIGN, TK_ASSIGN, TK_EQUAL, TK_NOT_EQUAL, TK_LESS, TK_GREATER, TK_LESS_EQUAL, TK_GREATER_EQUAL, TK_OR, TK_AND);
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

/// 39
int rule_eol_opt(parser_t parser) {
	// Eol_opt ->			  eol
	//						| eps .
	switch (TOKEN_TYPE) {
		case TK_EOL:
			TK_NEXT();
			break;
		default:
			// eps
			break;
	}
	return EXIT_SUCCESS;
}
