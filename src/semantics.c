#include "semantics.h"

#include "error.h"

int sem_define_func(parser_t parser) {
	if (parser->first_pass) {
		if (symtable_has_func(parser->symtable, parser->token)) {
			return ERROR_DEFINITION; /*redefinition*/
		} else {
			symbol_ref_t symbol = symtable_insert(parser->symtable, parser->token, ST_FUNC);
			if (!symbol_valid(symbol)) {
				return ERROR_MISC; /*allocation error*/
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
	return success ? EXIT_SUCCESS : ERROR_MISC;
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
	return success ? EXIT_SUCCESS : ERROR_MISC;
}

int sem_func_callable(parser_t parser) {
	symbol_ref_t symbol = symtable_find(parser->symtable, parser->token);
	if (symbol_valid(symbol)) {
		if (symbol.symbol->type == ST_FUNC) {
			parser->sem.func = symbol;
		} else {
			return ERROR_DEFINITION; // might be ERROR_SEM if there is func like that defined behind the variable
		}
	} else {
		return ERROR_DEFINITION;
	}
	return EXIT_SUCCESS;
}
