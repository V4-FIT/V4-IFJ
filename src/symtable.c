#include "symtable.h"

#include <assert.h>
#include <string.h>

#include "forward_list.h"
#include "hash_map.h"

////// Private

#define BUCKET_COUNT 769

struct symtable
{
	flist_t tables;
	hmap_t global_table;
};

hmap_t symtable_front(symtable_t symtable) {
	return *(hmap_t *)flist_front(symtable->tables);
}

////// Public

const char *dt2str_map[] = {"undefined", "int", "float64", "string", "bool"};

symtable_t symtable_init() {
	symtable_t symtable = malloc(sizeof(struct symtable));
	if (symtable == NULL) {
		return NULL;
	}

	symtable->tables = flist_init(sizeof(hmap_t));
	if (symtable->tables == NULL) {
		free(symtable);
		return NULL;
	}

	if (!symtable_enter_scope(symtable)) {
		flist_free(symtable->tables);
		free(symtable);
		return NULL;
	}

	symtable->global_table = symtable_front(symtable);

	return symtable;
}

bool symtable_enter_scope(symtable_t symtable) {
	assert(symtable);
	hmap_t hmap = hmap_init(BUCKET_COUNT, sizeof(symbol_t));
	if (hmap == NULL) {
		return false;
	}

	if (!flist_push_front(symtable->tables, &hmap)) {
		return false;
	}

	return true;
}

void symtable_exit_scope(symtable_t symtable) {
	assert(symtable && !flist_empty(symtable->tables));
	hmap_t hmap = symtable_front(symtable);

	for (hmap_iterator_t it = hmap_begin(hmap); !hmap_it_eq(it, hmap_end(hmap)); it = hmap_it_next(it)) {
		symbol_t sym = *(symbol_t *)hmap_get_value(it);
		if (sym.type == ST_FUNC) {
			flist_free(sym.func.param_list);
			flist_free(sym.func.return_list);
		}
	}

	hmap_free(hmap);
	flist_pop_front(symtable->tables);
}

bool symtable_has_symbol(symtable_t symtable, token_t id_token) {
	symbol_ref_t symbol_ref = symtable_find(symtable, id_token);
	return symbol_valid(symbol_ref);
}

bool symtable_has_func(symtable_t symtable, token_t id_token) {
	symbol_ref_t symbol_ref;
	symbol_ref.symbol = NULL;
	symbol_ref.symtable = symtable;
	symbol_ref.it = hmap_find(symtable->global_table, id_token->lexeme);
	if (hmap_it_valid(symbol_ref.it)) {
		symbol_ref.symbol = hmap_get_value(symbol_ref.it);
		if (symbol_ref.symbol->type == ST_FUNC) {
			return true;
		}
	}
	return false;
}

bool symtable_has_var(symtable_t symtable, token_t id_token) {
	symbol_ref_t symbol_ref = symtable_find(symtable, id_token);
	if (symbol_valid(symbol_ref)) {
		if (symbol_ref.symbol->type == ST_VAR) {
			return true;
		}
	}
	return false;
}

symbol_ref_t symtable_find(symtable_t symtable, token_t id_token) {
	assert(symtable && id_token);
	assert(!flist_empty(symtable->tables));
	assert(id_token->type == TK_IDENTIFIER || id_token->type == TK_KEYW_MAIN);
	assert(id_token->lexeme);

	hmap_key_t key = id_token->lexeme;
	symbol_ref_t symbol_ref;
	symbol_ref.symbol = NULL;
	symbol_ref.symtable = symtable;
	for (flist_iterator_t it = flist_begin(symtable->tables); flist_it_valid(it); it = flist_it_next(it)) {
		hmap_t hmap = *(hmap_t *)flist_get(it);
		symbol_ref.it = hmap_find(hmap, key);
		if (hmap_it_valid(symbol_ref.it)) {
			symbol_ref.symbol = hmap_get_value(symbol_ref.it);
			break;
		}
	}

	return symbol_ref;
}

symbol_ref_t symtable_insert(symtable_t symtable, token_t id_token, symbol_type_t symbol_type) {
	assert(symtable && id_token);
	assert(!flist_empty(symtable->tables));
	assert(id_token->type == TK_IDENTIFIER || id_token->type == TK_KEYW_MAIN);
	assert(id_token->lexeme);

	const char *name = id_token->lexeme;
	hmap_t hmap;
	if (symbol_type == ST_FUNC) {
		hmap = symtable->global_table;
	} else {
		hmap = symtable_front(symtable);
	}

	symbol_ref_t symbol_ref;
	symbol_ref.symbol = NULL;
	symbol_ref.symtable = symtable;
	symbol_ref.it = hmap_find_add(hmap, name);

	if (hmap_it_valid(symbol_ref.it)) {

		symbol_t symbol;
		symbol.type = symbol_type;
		symbol.name = hmap_get_key(symbol_ref.it);
		// init type specific data
		if (symbol_type == ST_FUNC) {
			symbol.func.param_count = 0;
			symbol.func.return_count = 0;
			symbol.func.param_list = flist_init(sizeof(data_type_t));
			if (symbol.func.param_list == NULL) {
				symbol_ref.it = hmap_end(hmap);
				return symbol_ref;
			}
			symbol.func.return_list = flist_init(sizeof(data_type_t));
			if (symbol.func.return_list == NULL) {
				flist_free(symbol.func.param_list);
				symbol_ref.it = hmap_end(hmap);
				return symbol_ref;
			}
		} else {
			symbol.var.data_type = DT_UNDEFINED;
			symbol.var.constant = false;
			symbol.var.value.i = 0;
		}
		// set symbol data
		hmap_set_value(symbol_ref.it, &symbol);
		symbol_ref.symbol = hmap_get_value(symbol_ref.it);
	}

	return symbol_ref;
}

bool symbol_func_add_param(symbol_ref_t symbol_ref, data_type_t data_type) {
	assert(symbol_valid(symbol_ref));
	assert(symbol_ref.symbol->type == ST_FUNC);

	flist_t list = symbol_ref.symbol->func.param_list;
	symbol_ref.symbol->func.param_count++;
	return flist_push_back(list, &data_type);
}

bool symbol_func_add_return(symbol_ref_t symbol_ref, data_type_t data_type) {
	assert(symbol_valid(symbol_ref));
	assert(symbol_ref.symbol->type == ST_FUNC);

	flist_t list = symbol_ref.symbol->func.return_list;
	symbol_ref.symbol->func.return_count++;
	return flist_push_back(list, &data_type);
}

void symbol_var_set_data(symbol_ref_t symbol_ref, sym_var_t sym_var_data) {
	assert(symbol_valid(symbol_ref));
	assert(symbol_ref.symbol->type == ST_VAR);
	symbol_ref.symbol->var = sym_var_data;
}

bool symbol_valid(symbol_ref_t symbol_ref) {
	return hmap_it_valid(symbol_ref.it);
}

bool symbol_current_scope(symbol_ref_t symbol_ref) {
	assert(symbol_valid(symbol_ref));
	assert(symbol_ref.symtable);
	assert(!flist_empty(symbol_ref.symtable->tables));
	hmap_t hmap = symtable_front(symbol_ref.symtable);
	return symbol_ref.it.hmap == hmap;
}

void symtable_free(symtable_t symtable) {
	assert(symtable);
	while (!flist_empty(symtable->tables)) {
		symtable_exit_scope(symtable);
	}
	flist_free(symtable->tables);
	free(symtable);
}
