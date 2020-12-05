#include "semantics.h"

#include <assert.h>

#include "error.h"
#include "precedence.h"

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
		switch (parser->token->type) {
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
		switch (parser->token->type) {
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

int sem_binary_op_type_compat(parser_t parser, prec_stack_t *head) {
	if (STACK_FIRST->sem.data_type != STACK_THIRD->sem.data_type) {
		PARSER_EXPR_ERROR_MSG(MISMATCHED_TYPES_MSG);
		return ERROR_TYPE_COMPAT;
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
	// TODO
	return 0;
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
	if (STACK_FIRST->sem.constant) {
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
