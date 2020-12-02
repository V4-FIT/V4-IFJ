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

int sem_func_add_param(parser_t parser) {
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

int sem_func_add_return(parser_t parser) {
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
