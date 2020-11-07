#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdbool.h>

#include "forward_list.h"

typedef struct symtable *symtable_t;

typedef enum {
	ST_VAR,
	ST_FUNC
} symbol_type_t;

typedef enum {
	DT_INTEGER,
	DT_FLOAT64,
	DT_STRING,
	DT_BOOL
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

symtable_t symtable_init();



void symtable_free();

#endif // !SYMTABLE_H