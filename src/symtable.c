#include <assert.h>

#include "symtable.h"
#include "forward_list.h"
#include "hash_map.h"

////// Private

#define BUCKET_COUNT 769

struct symtable {
	flist_t tables;
};

////// Public

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
	hmap_t hmap = *(hmap_t *)flist_front(symtable->tables);

	for (hmap_iterator_t it = hmap_begin(hmap); !hmap_it_eq(it, hmap_end(hmap)); hmap_it_next(it)) {
		symbol_t sym = *(symbol_t *)hmap_get_value(it);
		if (sym.type == ST_FUNC) {
			flist_free(sym.func.param_list);
			flist_free(sym.func.return_list);
		}
	}

	hmap_free(hmap);
	flist_pop_front(symtable->tables);
}



void symtable_free(symtable_t symtable) {
	assert(symtable);
	while (!flist_empty(symtable->tables)) {
		symtable_exit_scope(symtable);
	}
	flist_free(symtable->tables);
	free(symtable);
}
