#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "forward_list.h"

#include <stdbool.h>

#include "hash_map.h"
#include "tokens.h"

typedef struct symtable *symtable_t;

typedef enum {
	ST_VAR,
	ST_FUNC
} symbol_type_t;

typedef enum {
	DT_INTEGER,
	DT_FLOAT64,
	DT_STRING,
	DT_BOOL,
	DT_UNDEFINED
} data_type_t;

typedef struct {
	data_type_t data_type;
	//TODO: SYMTABLE - add required attributes for variables
} sym_var_t;


typedef struct {
	unsigned int param_count;
	flist_t param_list;
	unsigned int return_count;
	flist_t return_list;
	//TODO: SYMTABLE - revise / add required attributes for functions
} sym_func_t;

typedef struct {
	const char *name;
	symbol_type_t type;
	union {
		sym_var_t var;
		sym_func_t func;
	};
} symbol_t;

typedef struct {
	hmap_iterator_t it;
	symbol_t *symbol;
}symbol_ref_t;

symtable_t symtable_init();

bool symtable_enter_scope(symtable_t symtable);

void symtable_exit_scope(symtable_t symtable);

bool symtable_has_symbol(symtable_t symtable, token_t id_token);

bool symtable_has_func(symtable_t symtable, token_t id_token);

bool symtable_has_var(symtable_t symtable, token_t id_token);

symbol_ref_t symtable_find(symtable_t symtable, token_t id_token);

symbol_ref_t symtable_insert(symtable_t symtable, token_t id_token, symbol_type_t symbol_type);

bool symbol_func_add_param(symbol_ref_t symbol_ref, data_type_t data_type);

bool symbol_func_add_return(symbol_ref_t symbol_ref, data_type_t data_type);

void symbol_var_set_data(symbol_ref_t symbol_ref, sym_var_t sym_var_data);

bool symbol_valid(symbol_ref_t symbol_ref);

void symtable_free(symtable_t symtable);

#endif // !SYMTABLE_H
