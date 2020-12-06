#include "semantics.h"

#include <assert.h>

#include "error.h"
#include "precedence.h"

////// Semantic error messages

#define PARSER_EXPR_ERROR_MSG(_EXTRA_MSG)                                                 \
	fprintf(stderr, "ERROR (line %d) - Invalid operation: ", parser->token->line_number); \
	while (parser->sem.expr_begin_it.ptr != parser->tkit.ptr) {                           \
		if (fprintf(stderr, "%s", tklist_get(parser->sem.expr_begin_it)->lexeme) > 0) {   \
			fprintf(stderr, " ");                                                         \
		}                                                                                 \
		parser->sem.expr_begin_it = tklist_it_next(parser->sem.expr_begin_it);            \
	}                                                                                     \
	_EXTRA_MSG;                                                                           \
	fprintf(stderr, "\n")

#define PARSER_COND_EXPR_ERROR_MSG()                                                    \
	fprintf(stderr, "ERROR (line %d) - non-bool ", parser->token->line_number);         \
	while (parser->sem.expr_begin_it.ptr != parser->tkit.ptr) {                         \
		if (fprintf(stderr, "%s", tklist_get(parser->sem.expr_begin_it)->lexeme) > 0) { \
			fprintf(stderr, " ");                                                       \
		}                                                                               \
		parser->sem.expr_begin_it = tklist_it_next(parser->sem.expr_begin_it);          \
	}                                                                                   \
	fprintf(stderr, "(type %s) used as %s condition\n",                                 \
			dt2str_map[parser->sem.expr_data_type],                                     \
			stmt2str_map[parser->sem.stmt])

#define MISMATCHED_TYPES_MSG                        \
	fprintf(stderr, "(mismatched types %s and %s)", \
			dt2str_map[STACK_THIRD->sem.data_type], \
			dt2str_map[STACK_FIRST->sem.data_type])

#define INVALID_TYPE_MSG                 \
	fprintf(stderr, "(invalid type %s)", \
			dt2str_map[STACK_FIRST->sem.data_type])

#define OPERATION_NOT_DEFINED_MSG                      \
	fprintf(stderr, "(operator %s not defined on %s)", \
			STACK_SECOND->token->lexeme,               \
			dt2str_map[STACK_FIRST->sem.data_type])

#define RETURN_TYPE_COMPAT_ERROR_MSG()                                                  \
	fprintf(stderr, "ERROR (line %d) - cannot use ", parser->token->line_number);       \
	while (parser->sem.expr_begin_it.ptr != parser->tkit.ptr) {                         \
		if (fprintf(stderr, "%s", tklist_get(parser->sem.expr_begin_it)->lexeme) > 0) { \
			fprintf(stderr, " ");                                                       \
		}                                                                               \
		parser->sem.expr_begin_it = tklist_it_next(parser->sem.expr_begin_it);          \
	}                                                                                   \
	fprintf(stderr, "(type %s) as type %s in return argument\n",                        \
			dt2str_map[parser->sem.expr_data_type],                                     \
			dt2str_map[func_ret_dt])

////// Private

// func inputs() (string, int)
int sem_define_builtin_inputs(parser_t parser) {
	struct token tk = {.type = TK_IDENTIFIER, .lexeme = "inputs"};
	symbol_ref_t func_ref = symtable_insert(parser->symtable, &tk, ST_FUNC);
	if (!symbol_valid(func_ref)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_return(func_ref, DT_STRING)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_return(func_ref, DT_INTEGER)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	return EXIT_SUCCESS;
}

// func inputi() (int, int)
int sem_define_builtin_inputi(parser_t parser) {
	struct token tk = {.type = TK_IDENTIFIER, .lexeme = "inputi"};
	symbol_ref_t func_ref = symtable_insert(parser->symtable, &tk, ST_FUNC);
	if (!symbol_valid(func_ref)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_return(func_ref, DT_INTEGER)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_return(func_ref, DT_INTEGER)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	return EXIT_SUCCESS;
}

// func inputf() (float64, int)
int sem_define_builtin_inputf(parser_t parser) {
	struct token tk = {.type = TK_IDENTIFIER, .lexeme = "inputf"};
	symbol_ref_t func_ref = symtable_insert(parser->symtable, &tk, ST_FUNC);
	if (!symbol_valid(func_ref)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_return(func_ref, DT_FLOAT64)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_return(func_ref, DT_INTEGER)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	return EXIT_SUCCESS;
}

// func inputb() (bool, int)
int sem_define_builtin_inputb(parser_t parser) {
	struct token tk = {.type = TK_IDENTIFIER, .lexeme = "inputb"};
	symbol_ref_t func_ref = symtable_insert(parser->symtable, &tk, ST_FUNC);
	if (!symbol_valid(func_ref)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_return(func_ref, DT_BOOL)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_return(func_ref, DT_INTEGER)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	return EXIT_SUCCESS;
}

// func print (term1, term2, ..., termn)
int sem_define_builtin_print(parser_t parser) {
	struct token tk = {.type = TK_IDENTIFIER, .lexeme = "print"};
	symbol_ref_t func_ref = symtable_insert(parser->symtable, &tk, ST_FUNC);
	if (!symbol_valid(func_ref)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	parser->sem.func_print = func_ref;
	return EXIT_SUCCESS;
}

//func int2float(i int) (float64)
int sem_define_builtin_int2float(parser_t parser) {
	struct token tk = {.type = TK_IDENTIFIER, .lexeme = "int2float"};
	symbol_ref_t func_ref = symtable_insert(parser->symtable, &tk, ST_FUNC);
	if (!symbol_valid(func_ref)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_param(func_ref, DT_INTEGER)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_return(func_ref, DT_FLOAT64)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	return EXIT_SUCCESS;
}

// func float2int(f float64) (int)
int sem_define_builtin_float2int(parser_t parser) {
	struct token tk = {.type = TK_IDENTIFIER, .lexeme = "float2int"};
	symbol_ref_t func_ref = symtable_insert(parser->symtable, &tk, ST_FUNC);
	if (!symbol_valid(func_ref)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_param(func_ref, DT_FLOAT64)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_return(func_ref, DT_INTEGER)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	return EXIT_SUCCESS;
}

// func len(s string) (int)
int sem_define_builtin_len(parser_t parser) {
	struct token tk = {.type = TK_IDENTIFIER, .lexeme = "len"};
	symbol_ref_t func_ref = symtable_insert(parser->symtable, &tk, ST_FUNC);
	if (!symbol_valid(func_ref)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_param(func_ref, DT_STRING)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_return(func_ref, DT_INTEGER)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	return EXIT_SUCCESS;
}

// func substr(s string, i int, n int) (string, int)
int sem_define_builtin_substr(parser_t parser) {
	struct token tk = {.type = TK_IDENTIFIER, .lexeme = "substr"};
	symbol_ref_t func_ref = symtable_insert(parser->symtable, &tk, ST_FUNC);
	if (!symbol_valid(func_ref)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_param(func_ref, DT_STRING)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_param(func_ref, DT_INTEGER)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_param(func_ref, DT_INTEGER)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_return(func_ref, DT_STRING)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_return(func_ref, DT_INTEGER)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	return EXIT_SUCCESS;
}

// func ord(s string, i int) (int, int)
int sem_define_builtin_ord(parser_t parser) {
	struct token tk = {.type = TK_IDENTIFIER, .lexeme = "ord"};
	symbol_ref_t func_ref = symtable_insert(parser->symtable, &tk, ST_FUNC);
	if (!symbol_valid(func_ref)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_param(func_ref, DT_STRING)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_param(func_ref, DT_INTEGER)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_return(func_ref, DT_INTEGER)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_return(func_ref, DT_INTEGER)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	return EXIT_SUCCESS;
}

// func chr(i int) (string, int)
int sem_define_builtin_chr(parser_t parser) {
	struct token tk = {.type = TK_IDENTIFIER, .lexeme = "chr"};
	symbol_ref_t func_ref = symtable_insert(parser->symtable, &tk, ST_FUNC);
	if (!symbol_valid(func_ref)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_param(func_ref, DT_INTEGER)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_return(func_ref, DT_STRING)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	if (!symbol_func_add_return(func_ref, DT_INTEGER)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	return EXIT_SUCCESS;
}

// converts token to data type
data_type_t tk2dt(parser_t parser, token_t token) {
	symbol_ref_t symbol_ref;
	switch (token->type) {
		case TK_IDENTIFIER:
			symbol_ref = symtable_find(parser->symtable, token);
			assert(symbol_valid(symbol_ref));
			return symbol_ref.symbol->var.data_type;
		case TK_INT_LIT:
			return DT_INTEGER;
		case TK_FLOAT_LIT:
			return DT_FLOAT64;
		case TK_STR_LIT:
			return DT_STRING;
		case TK_KEYW_TRUE:
		case TK_KEYW_FALSE:
			return DT_BOOL;
	}
	return DT_UNDEFINED;
}

////// Semantic actions

int sem_define_builtin_functions(parser_t parser) {
	if (parser->first_pass) {
		SEM_ACTION(sem_define_builtin_inputs);
		SEM_ACTION(sem_define_builtin_inputi);
		SEM_ACTION(sem_define_builtin_inputf);
		SEM_ACTION(sem_define_builtin_inputb);
		SEM_ACTION(sem_define_builtin_print);
		SEM_ACTION(sem_define_builtin_int2float);
		SEM_ACTION(sem_define_builtin_float2int);
		SEM_ACTION(sem_define_builtin_len);
		SEM_ACTION(sem_define_builtin_substr);
		SEM_ACTION(sem_define_builtin_ord);
		SEM_ACTION(sem_define_builtin_chr);
	}
	return EXIT_SUCCESS;
}

int sem_enter_scope(parser_t parser) {
	if (!symtable_enter_scope(parser->symtable)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	return EXIT_SUCCESS;
}

int sem_exit_scope(parser_t parser) {
	symtable_exit_scope(parser->symtable);
	return EXIT_SUCCESS;
}

int sem_func_define(parser_t parser) {
	if (parser->first_pass) {
		if (symtable_has_func(parser->symtable, parser->token)) {
			PARSER_ERROR_MSG("The function '%s' has already been defined.", parser->token->lexeme);
			return ERROR_DEFINITION;
		} else {
			symbol_ref_t symbol_ref = symtable_insert(parser->symtable, parser->token, ST_FUNC);
			if (!symbol_valid(symbol_ref)) {
				ALLOCATION_ERROR_MSG();
				return ERROR_MISC;
			}
			parser->sem.func_cur = symbol_ref;
		}
	} else {
		parser->sem.func_cur = symtable_find(parser->symtable, parser->token);
		assert(symbol_valid(parser->sem.func_cur));
	}
	return EXIT_SUCCESS;
}

int sem_func_declare_param(parser_t parser) {
	if (!parser->first_pass) {
		symbol_ref_t symbol_ref = symtable_insert(parser->symtable, parser->token, ST_VAR);
		if (!symbol_valid(symbol_ref)) {
			ALLOCATION_ERROR_MSG();
			return ERROR_MISC;
		}
		sym_var_t var_data = {.data_type = DT_UNDEFINED, .constant = false, .value = 0};
		switch (parser->token_second->type) {
			case TK_KEYW_FLOAT64:
				var_data.data_type = DT_FLOAT64;
				break;
			case TK_KEYW_INT:
				var_data.data_type = DT_INTEGER;
				break;
			case TK_KEYW_STRING:
				var_data.data_type = DT_STRING;
				break;
			case TK_KEYW_BOOL:
				var_data.data_type = DT_BOOL;
				break;
		}
		symbol_var_set_data(symbol_ref, var_data);
	}
	return EXIT_SUCCESS;
}

int sem_func_add_param_type(parser_t parser) {
	bool success = true;
	if (parser->first_pass) {
		switch (TOKEN_TYPE) {
			case TK_KEYW_FLOAT64:
				success = symbol_func_add_param(parser->sem.func_cur, DT_FLOAT64);
				break;
			case TK_KEYW_INT:
				success = symbol_func_add_param(parser->sem.func_cur, DT_INTEGER);
				break;
			case TK_KEYW_STRING:
				success = symbol_func_add_param(parser->sem.func_cur, DT_STRING);
				break;
			case TK_KEYW_BOOL:
				success = symbol_func_add_param(parser->sem.func_cur, DT_BOOL);
				break;
		}
	}
	if (success) {
		return EXIT_SUCCESS;
	} else {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
}

int sem_func_add_return_type(parser_t parser) {
	bool success = true;
	if (parser->first_pass) {
		switch (TOKEN_TYPE) {
			case TK_KEYW_FLOAT64:
				success = symbol_func_add_return(parser->sem.func_cur, DT_FLOAT64);
				break;
			case TK_KEYW_INT:
				success = symbol_func_add_return(parser->sem.func_cur, DT_INTEGER);
				break;
			case TK_KEYW_STRING:
				success = symbol_func_add_return(parser->sem.func_cur, DT_STRING);
				break;
			case TK_KEYW_BOOL:
				success = symbol_func_add_return(parser->sem.func_cur, DT_BOOL);
				break;
		}
	}
	if (success) {
		return EXIT_SUCCESS;
	} else {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
}

int sem_func_has_return_stmt(parser_t parser) {
	symbol_t *symbol = parser->sem.func_cur.symbol;
	if (symbol->func.return_count > 0 && parser->sem.stmt != STMT_RETURN) {
		ERROR_MSG("Missing return at the end of function '%s'", symbol->name);
		return ERROR_PARAM;
	}
	return EXIT_SUCCESS;
}

int sem_func_callable(parser_t parser) {
	symbol_ref_t symbol_ref = symtable_find(parser->symtable, parser->token);
	if (symbol_valid(symbol_ref)) {
		if (symbol_ref.symbol->type == ST_FUNC) {
			parser->sem.func_call = symbol_ref;
			parser->sem.func_param_it = flist_begin(symbol_ref.symbol->func.param_list);
		} else {
			PARSER_ERROR_MSG("The function is hidden by the variable '%s'", parser->token->lexeme);
			return ERROR_SEM;
		}
	} else {
		PARSER_ERROR_MSG("The function '%s' has not been defined.", parser->token->lexeme);
		return ERROR_DEFINITION;
	}
	return EXIT_SUCCESS;
}

int sem_main_defined(parser_t parser) {
	if (parser->first_pass) {
		struct token tk = {.type = TK_IDENTIFIER, .lexeme = "main"};
		symbol_ref_t symbol_ref = symtable_find(parser->symtable, &tk);
		if (symbol_valid(symbol_ref)) {
			if (symbol_ref.symbol->func.param_count || symbol_ref.symbol->func.return_count) {
				PARSER_ERROR_MSG("The function 'main' should not have parameters or returns");
				return ERROR_PARAM;
			}
		} else {
			ERROR_MSG("The function 'main' has not been defined");
			return ERROR_DEFINITION;
		}
	}
	return EXIT_SUCCESS;
}

int sem_var_define(parser_t parser) {
	symbol_ref_t symbol_ref = symtable_find(parser->symtable, parser->token);
	if (symbol_valid(symbol_ref) && symbol_current_scope(symbol_ref)) {
		PARSER_ERROR_MSG("Symbol '%s' redefinition", symbol_ref.symbol->name);
		return ERROR_DEFINITION;
	}
	symbol_ref = symtable_insert(parser->symtable, parser->token, ST_VAR);
	if (!symbol_valid(symbol_ref)) {
		ALLOCATION_ERROR_MSG();
		return ERROR_MISC;
	}
	parser->sem.var = symbol_ref;
	return EXIT_SUCCESS;
}

int sem_var_define_type(parser_t parser) {
	parser->sem.var.symbol->var.data_type = parser->sem.expr_data_type;
	parser->sem.var.symbol->var.constant = parser->sem.expr_constant;
	parser->sem.var.symbol->var.value = parser->sem.expr_value;
	return EXIT_SUCCESS;
}

int sem_var_check(parser_t parser) {
	if (parser->token->type == TK_IDENTIFIER) {
		symbol_ref_t symbol_ref = symtable_find(parser->symtable, parser->token);
		if (!symbol_valid(symbol_ref)) {
			PARSER_ERROR_MSG("Undefined: %s", parser->token->lexeme);
			return ERROR_DEFINITION;
		}
	}
	return 0;
}

int sem_func_stmts_begin(parser_t parser) {
	parser->sem.stmt = STMT_DEFAULT;
	return EXIT_SUCCESS;
}

int sem_define_begin(parser_t parser) {
	parser->sem.stmt = STMT_DEFINE;
	return EXIT_SUCCESS;
}

int sem_assign_begin(parser_t parser) {
	parser->sem.stmt = STMT_ASSIGN;
	parser->sem.ids_begin_it = parser->tkit;
	parser->sem.ids_count = 0;
	parser->sem.expr_count = 0;
	return EXIT_SUCCESS;
}

int sem_call_begin(parser_t parser) {
	if (parser->sem.stmt == STMT_ASSIGN) {
		parser->sem.stmt = STMT_ASSIGN_CALL;
	} else {
		parser->sem.stmt = STMT_CALL;
	}
	return EXIT_SUCCESS;
}

int sem_conditional_begin(parser_t parser) {
	parser->sem.stmt = STMT_CONDITIONAL;
	return EXIT_SUCCESS;
}

int sem_iterative_begin(parser_t parser) {
	parser->sem.stmt = STMT_ITERATIVE;
	return EXIT_SUCCESS;
}

int sem_return_begin(parser_t parser) {
	parser->sem.stmt = STMT_RETURN;
	parser->sem.func_return_it = flist_begin(parser->sem.func_cur.symbol->func.return_list);
	parser->sem.expr_count = 0;
	parser->sem.expr_begin_it = parser->tkit2;
	return EXIT_SUCCESS;
}

int sem_expression_begin(parser_t parser) {
	parser->sem.expr_begin_it = parser->tkit;
	parser->sem.expr_count++;
	return EXIT_SUCCESS;
}

int sem_binary_op_type_compat(parser_t parser, prec_stack_t *head) {
	if (STACK_FIRST->sem.data_type != STACK_THIRD->sem.data_type) {
		PARSER_EXPR_ERROR_MSG(MISMATCHED_TYPES_MSG);
		return ERROR_TYPE_COMPAT;
	}
	if (STACK_FIRST->sem.data_type == DT_BOOL) {
		if (STACK_SECOND->token->type != TK_EQUAL
			&& STACK_SECOND->token->type != TK_NOT_EQUAL) {
			PARSER_EXPR_ERROR_MSG(OPERATION_NOT_DEFINED_MSG);
			return ERROR_TYPE_COMPAT;
		}
	}
	if (STACK_FIRST->sem.data_type == DT_STRING) {
		if (STACK_SECOND->token->type == TK_MINUS
			|| STACK_SECOND->token->type == TK_MULTIPLY
			|| STACK_SECOND->token->type == TK_DIVIDE) {
			PARSER_EXPR_ERROR_MSG(OPERATION_NOT_DEFINED_MSG);
			return ERROR_TYPE_COMPAT;
		}
	}
	return EXIT_SUCCESS;
}

int sem_logical_op_type_compat(parser_t parser, prec_stack_t *head) {
	if (STACK_FIRST->sem.data_type != DT_BOOL || STACK_THIRD->sem.data_type != DT_BOOL) {
		PARSER_EXPR_ERROR_MSG(MISMATCHED_TYPES_MSG);
		return ERROR_TYPE_COMPAT;
	}
	return EXIT_SUCCESS;
}

int sem_unary_op_type_compat(parser_t parser, prec_stack_t *head) {
	switch (STACK_SECOND->token->type) {
		case TK_NOT:
			if (STACK_FIRST->sem.data_type != DT_BOOL) {
				PARSER_EXPR_ERROR_MSG(INVALID_TYPE_MSG);
				return ERROR_TYPE_COMPAT;
			}
			break;
		default:
			if (STACK_FIRST->sem.data_type != DT_INTEGER && STACK_FIRST->sem.data_type != DT_FLOAT64) {
				PARSER_EXPR_ERROR_MSG(INVALID_TYPE_MSG);
				return ERROR_TYPE_COMPAT;
			}
			break;
	}
	return EXIT_SUCCESS;
}

int sem_prec_rule_exit(parser_t parser, prec_stack_t *head) {
	prec_stack_sem_t sem = STACK_FIRST->sem;
	parser->sem.expr_data_type = sem.data_type;
	parser->sem.expr_constant = sem.constant;
	parser->sem.expr_value = sem.value;
	return EXIT_SUCCESS;
}

int sem_evaulate_binary_const_expr(parser_t parser, prec_stack_t *head) {
	if (STACK_FIRST->sem.constant && STACK_THIRD->sem.constant) {
		switch (STACK_SECOND->token->type) {
			case TK_PLUS:
				if (STACK_THIRD->sem.data_type == DT_INTEGER) {
					STACK_THIRD->sem.value.i += STACK_FIRST->sem.value.i;
				} else if (STACK_THIRD->sem.data_type == DT_FLOAT64) {
					STACK_THIRD->sem.value.f += STACK_FIRST->sem.value.f;
				}
				break;
			case TK_MINUS:
				if (STACK_THIRD->sem.data_type == DT_INTEGER) {
					STACK_THIRD->sem.value.i -= STACK_FIRST->sem.value.i;
				} else if (STACK_THIRD->sem.data_type == DT_FLOAT64) {
					STACK_THIRD->sem.value.f -= STACK_FIRST->sem.value.f;
				}
				break;
			case TK_MULTIPLY:
				if (STACK_THIRD->sem.data_type == DT_INTEGER) {
					STACK_THIRD->sem.value.i *= STACK_FIRST->sem.value.i;
				} else if (STACK_THIRD->sem.data_type == DT_FLOAT64) {
					STACK_THIRD->sem.value.f *= STACK_FIRST->sem.value.f;
				}
				break;
			case TK_DIVIDE:
				if (STACK_THIRD->sem.data_type == DT_INTEGER) {
					STACK_THIRD->sem.value.i /= STACK_FIRST->sem.value.i;
				} else if (STACK_THIRD->sem.data_type == DT_FLOAT64) {
					STACK_THIRD->sem.value.f /= STACK_FIRST->sem.value.f;
				}
				break;
			default:
				break;
		}
	}
	return EXIT_SUCCESS;
}

int sem_evaulate_unary_const_expr(parser_t parser, prec_stack_t *head) {
	if (STACK_SECOND->token->type == TK_MINUS && STACK_FIRST->sem.constant) {
		if (STACK_FIRST->sem.data_type == DT_INTEGER) {
			STACK_FIRST->sem.value.i = -STACK_FIRST->sem.value.i;
		} else if (STACK_FIRST->sem.data_type == DT_FLOAT64) {
			STACK_FIRST->sem.value.f = -STACK_FIRST->sem.value.f;
		}
	}
	return EXIT_SUCCESS;
}

int sem_zero_division(parser_t parser, prec_stack_t *head) {
	if (STACK_FIRST->sem.constant && STACK_SECOND->token->type == TK_DIVIDE) {
		switch (STACK_FIRST->sem.data_type) {
			case DT_INTEGER:
				if (STACK_FIRST->sem.value.i == 0) {
					PARSER_ERROR_MSG("division by zero");
					return ERROR_ZERO_DIV;
				}
				break;
			case DT_FLOAT64:
				if (STACK_FIRST->sem.value.f == 0.0) {
					PARSER_ERROR_MSG("division by zero");
					return ERROR_ZERO_DIV;
				}
				break;
		}
	}
	return EXIT_SUCCESS;
}

int sem_bool_condiiton(parser_t parser) {
	if (parser->sem.expr_data_type != DT_BOOL) {
		PARSER_COND_EXPR_ERROR_MSG();
		return ERROR_TYPE_COMPAT;
	}
	return EXIT_SUCCESS;
}

int sem_id_begin(parser_t parser) {
	parser->sem.ids_count++;
	if (parser->token->type == TK_IDENTIFIER) {
		symbol_ref_t symbol_ref = symtable_find(parser->symtable, parser->token);
		if (symbol_valid(symbol_ref) && symbol_ref.symbol->type == ST_VAR) {
			symbol_ref.symbol->var.constant = false;
		}
	}
	return EXIT_SUCCESS;
}

int sem_call_no_return(parser_t parser) {
	if (parser->sem.func_call.symbol->func.return_count > 0) {
		PARSER_ERROR_MSG("The function '%s' has return parameters (cannot be called on its own)",
						 parser->sem.func_call.symbol->name);
		return ERROR_SEM;
	}
	return EXIT_SUCCESS;
}

int sem_return_expr_type_compat(parser_t parser) {
	if (parser->sem.stmt == STMT_RETURN && flist_it_valid(parser->sem.func_return_it)) {
		data_type_t func_ret_dt = *(data_type_t *)flist_get(parser->sem.func_return_it);
		if (parser->sem.expr_data_type != func_ret_dt) {
			RETURN_TYPE_COMPAT_ERROR_MSG();
			return ERROR_PARAM;
		}
		parser->sem.func_return_it = flist_it_next(parser->sem.func_return_it);
	}
	return EXIT_SUCCESS;
}

int sem_return_expr_count(parser_t parser) {
	if (parser->sem.expr_count != parser->sem.func_cur.symbol->func.return_count) {
		fprintf(stderr, "ERROR (line %d) - ", parser->token->line_number);
		if (parser->sem.expr_count > parser->sem.func_cur.symbol->func.return_count) {
			fprintf(stderr, "too many arguments to return\n\t\thave ( ");
		} else {
			fprintf(stderr, "not enough arguments to return\n\t\thave ( ");
		}
		while (parser->sem.expr_begin_it.ptr != parser->tkit.ptr) {
			if (fprintf(stderr, "%s", dt2str_map[tk2dt(parser, tklist_get(parser->sem.expr_begin_it))]) > 0) {
				fprintf(stderr, " ");
			}
			parser->sem.expr_begin_it = tklist_it_next(parser->sem.expr_begin_it);
		}
		fprintf(stderr, ")\n\t\twant ( ");
		flist_iterator_t it = flist_begin(parser->sem.func_cur.symbol->func.return_list);
		while (flist_it_valid(it)) {
			if (fprintf(stderr, "%s", dt2str_map[*(data_type_t *)flist_get(it)]) > 0) {
				fprintf(stderr, " ");
			}
			it = flist_it_next(it);
		}
		fprintf(stderr, ")\n");
		return ERROR_PARAM;
	}
	return EXIT_SUCCESS;
}

int sem_arguments_begin(parser_t parser) {
	parser->sem.argument_count = 0;
	parser->sem.argument_begin_it = parser->tkit;
	return EXIT_SUCCESS;
}

int sem_argument_begin(parser_t parser) {
	parser->sem.argument_count++;
	if (flist_it_valid(parser->sem.func_param_it)) {
		data_type_t func_param_dt = *(data_type_t *)flist_get(parser->sem.func_param_it);
		if (func_param_dt != tk2dt(parser, parser->token)) {
			fprintf(stderr, "ERROR (line %d) - cannot use %s ",
					parser->token->line_number,
					parser->token->lexeme);
			fprintf(stderr, "(type %s) as type %s in argument to %s\n",
					dt2str_map[tk2dt(parser, parser->token)],
					dt2str_map[func_param_dt], parser->sem.func_call.symbol->name);
			return ERROR_PARAM;
		}
		parser->sem.func_param_it = flist_it_next(parser->sem.func_param_it);
	}
	return EXIT_SUCCESS;
}

bool calling_builtin_print(parser_t parser) {
	return hmap_it_eq(parser->sem.func_call.it, parser->sem.func_print.it);
}

int sem_call_argument_count(parser_t parser) {
	if (calling_builtin_print(parser)) {
		return EXIT_SUCCESS;
	}
	if (parser->sem.argument_count != parser->sem.func_call.symbol->func.param_count) {
		fprintf(stderr, "ERROR (line %d) - ", parser->token->line_number);
		if (parser->sem.argument_count > parser->sem.func_call.symbol->func.param_count) {
			fprintf(stderr, "too many arguments in call to %s\n\t\thave ( ", parser->sem.func_call.symbol->name);
		} else {
			fprintf(stderr, "not enough arguments in call to %s\n\t\thave ( ", parser->sem.func_call.symbol->name);
		}
		while (parser->sem.argument_begin_it.ptr != parser->tkit.ptr) {
			if (tklist_get(parser->sem.argument_begin_it)->type != TK_COMMA) {
				if (fprintf(stderr, "%s", dt2str_map[tk2dt(parser, tklist_get(parser->sem.argument_begin_it))]) > 0) {
					fprintf(stderr, " ");
				}
			}
			parser->sem.argument_begin_it = tklist_it_next(parser->sem.argument_begin_it);
		}
		fprintf(stderr, ")\n\t\twant ( ");
		flist_iterator_t it = flist_begin(parser->sem.func_call.symbol->func.param_list);
		while (flist_it_valid(it)) {
			if (fprintf(stderr, "%s", dt2str_map[*(data_type_t *)flist_get(it)]) > 0) {
				fprintf(stderr, " ");
			}
			it = flist_it_next(it);
		}
		fprintf(stderr, ")\n");
		return ERROR_PARAM;
	}
	return EXIT_SUCCESS;
}

bool assign_op(token_type_t token_type) {
	return token_type == TK_PLUS_ASSIGN || token_type == TK_MINUS_ASSIGN || token_type == TK_MULTIPLY_ASSIGN || token_type == TK_DIVIDE_ASSIGN || token_type == TK_ASSIGN;
}

int sem_assignment_call_return(parser_t parser) {
	if (parser->sem.stmt != STMT_ASSIGN_CALL) {
		return EXIT_SUCCESS;
	}
	symbol_t symbol = *parser->sem.func_call.symbol;
	if (symbol.func.return_count == 0) {
		PARSER_ERROR_MSG("%s() used as value", symbol.name);
		return ERROR_PARAM;
	}
	if (parser->sem.ids_count != symbol.func.return_count) {
		PARSER_ERROR_MSG("assignment mismatch: %d variables but %s returns %d values",
						 parser->sem.ids_count, symbol.name, symbol.func.return_count);
		return ERROR_PARAM;
	}
	flist_iterator_t return_it = flist_begin(symbol.func.return_list);
	tklist_iterator_t id_it = parser->sem.ids_begin_it;
	while (!assign_op(tklist_get(id_it)->type)) {
		data_type_t return_dt = *(data_type_t *)flist_get(return_it);
		data_type_t id_dt = tk2dt(parser, tklist_get(id_it));
		if (id_dt != return_dt && tklist_get(id_it)->type != TK_UNDERSCORE) {
			if (parser->sem.ids_count > 1) {
				PARSER_ERROR_MSG("cannot assign %s to a (type %s) in a multiple assignment",
								 dt2str_map[return_dt], dt2str_map[id_dt]);
			} else {
				PARSER_ERROR_MSG("cannot use %s (type %s) as type %s in assignment",
								 symbol.name, dt2str_map[return_dt], dt2str_map[id_dt]);
			}
			return ERROR_PARAM;
		}
		return_it = flist_it_next(return_it);
		id_it = tklist_it_next(id_it);
		if (tklist_get(id_it)->type == TK_COMMA) {
			id_it = tklist_it_next(id_it);
		}
	}
	return EXIT_SUCCESS;
}

int sem_assign_expr_type_compat(parser_t parser) {
	if (parser->sem.stmt != STMT_ASSIGN) {
		return EXIT_SUCCESS;
	}
	if (tklist_it_valid(parser->sem.ids_begin_it)) {
		token_t var = tklist_get(parser->sem.ids_begin_it);
		if (var->type != TK_IDENTIFIER) {
			if (var->type == TK_UNDERSCORE) {
				parser->sem.ids_begin_it = tklist_it_next(parser->sem.ids_begin_it);
			}
			return EXIT_SUCCESS;
		}
		data_type_t var_dt = tk2dt(parser, var);
		if (var_dt != parser->sem.expr_data_type) {
			PARSER_ERROR_MSG("cannot use %s (type %s) as type %s in assignment",
							 var->lexeme, dt2str_map[var_dt], dt2str_map[parser->sem.expr_data_type]);
			return ERROR_TYPE_COMPAT;
		}
		parser->sem.ids_begin_it = tklist_it_next(parser->sem.ids_begin_it);
	}
	return EXIT_SUCCESS;
}

int sem_assign_expr_count(parser_t parser) {
	if (parser->sem.stmt != STMT_ASSIGN) {
		return EXIT_SUCCESS;
	}
	if (parser->sem.ids_count != parser->sem.expr_count) {
		PARSER_ERROR_MSG("assignment mismatch: %d variables but %d values",
						 parser->sem.ids_count, parser->sem.expr_count);
		return ERROR_SEM;
	}
	return EXIT_SUCCESS;
}

int sem_assign_zero_div(parser_t parser) {
	if (tklist_it_valid(parser->sem.ids_begin_it)) {
		token_t assign_op = tklist_get(parser->sem.ids_begin_it);
		if (assign_op->type == TK_DIVIDE_ASSIGN) {
			switch (parser->sem.expr_data_type) {
				case DT_INTEGER:
					if (parser->sem.expr_value.i == 0) {
						PARSER_ERROR_MSG("division by zero");
						return ERROR_ZERO_DIV;
					}
					break;
				case DT_FLOAT64:
					if (parser->sem.expr_value.f == 0.0) {
						PARSER_ERROR_MSG("division by zero");
						return ERROR_ZERO_DIV;
					}
					break;
			}
		}
	}
	return EXIT_SUCCESS;
}
