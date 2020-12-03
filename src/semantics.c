#include "semantics.h"

#include "error.h"

int sem_define_func(parser_t parser) {
	if (parser->first_pass) {
		if (symtable_has_func(parser->symtable, parser->token)) {
			PARSER_ERROR_MSG("The function '%s' has already been defined.", parser->token->param.s);
			return ERROR_DEFINITION; /*redefinition*/
		} else {
			symbol_ref_t symbol = symtable_insert(parser->symtable, parser->token, ST_FUNC);
			if (!symbol_valid(symbol)) {
				ALLOCATION_ERROR_MSG();
				return ERROR_MISC;
			}
			parser->sem.func = symbol;
		}
	}
	return EXIT_SUCCESS;
}

int sem_func_declare_param(parser_t parser) {
	if (!parser->first_pass) {
		symbol_ref_t symbol = symtable_insert(parser->symtable, parser->token, ST_VAR);
		if (!symbol_valid(symbol)) {
			ALLOCATION_ERROR_MSG();
			return ERROR_MISC;
		}
		sym_var_t var_data = {0};
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
		symbol_var_set_data(symbol, var_data);
	}
	return EXIT_SUCCESS;
}

int sem_func_add_param_type(parser_t parser) {
	bool success = true;
	if (parser->first_pass) {
		switch (parser->token->type) {
			case TK_KEYW_FLOAT64:
				success = symbol_func_add_param(parser->sem.func, DT_FLOAT64);
				break;
			case TK_KEYW_INT:
				success = symbol_func_add_param(parser->sem.func, DT_INTEGER);
				break;
			case TK_KEYW_STRING:
				success = symbol_func_add_param(parser->sem.func, DT_STRING);
				break;
			case TK_KEYW_BOOL:
				success = symbol_func_add_param(parser->sem.func, DT_BOOL);
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
				success = symbol_func_add_return(parser->sem.func, DT_FLOAT64);
				break;
			case TK_KEYW_INT:
				success = symbol_func_add_return(parser->sem.func, DT_INTEGER);
				break;
			case TK_KEYW_STRING:
				success = symbol_func_add_return(parser->sem.func, DT_STRING);
				break;
			case TK_KEYW_BOOL:
				success = symbol_func_add_return(parser->sem.func, DT_BOOL);
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

int sem_func_callable(parser_t parser) {
	symbol_ref_t symbol = symtable_find(parser->symtable, parser->token);
	if (symbol_valid(symbol)) {
		if (symbol.symbol->type == ST_FUNC) {
			parser->sem.func = symbol;
		} else {
			PARSER_ERROR_MSG("The function is hidden by the variable '%s'", parser->token->param.s);
			return ERROR_SEM;
		}
	} else {
		PARSER_ERROR_MSG("The function '%s' has not been defined.", parser->token->param.s);
		return ERROR_DEFINITION;
	}
	return EXIT_SUCCESS;
}

int sem_main_defined(parser_t parser) {
	if (parser->first_pass) {
		struct token tk = {TK_IDENTIFIER, "main"};
		symbol_ref_t symbol = symtable_find(parser->symtable, &tk);
		if (symbol_valid(symbol)) {
			if (symbol.symbol->func.param_count || symbol.symbol->func.return_count) {
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
