#include "tokens.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

////// Private

struct flist_node
{
	void *data;
	flist_node_t next;
};

struct flist
{
	flist_node_t head;
	flist_node_t tail;
	size_t data_size;
};

////// Public

tklist_t tklist_init() {
	tklist_t tklist = flist_init(sizeof(token_t));
	if (tklist == NULL) {
		return NULL;
	}
	return tklist;
}

void tklist_pop_front(tklist_t tklist) {
	tklist_iterator_t it = tklist_begin(tklist);
	if (tklist_it_valid(it)) {
		if (tklist->head != tklist->tail) {
			token_free(tklist_get(it));
			flist_pop_front(tklist);
		}
	}
}

bool tklist_push_back(tklist_t tklist, token_t token) {
	token_t tk = token_copy(token);
	if (!flist_push_back(tklist, &tk)) {
		return false;
	}
	return true;
}

token_t tklist_front(tklist_t tklist) {
	if (flist_front(tklist)) {
		return *(token_t *)flist_front(tklist);
	} else {
		return NULL;
	}
}

token_t tklist_second(tklist_t tklist) {
	tklist_iterator_t it = tklist_it_next(tklist_begin(tklist));
	if (tklist_it_valid(it)) {
		return tklist_get(it);
	} else {
		return NULL;
	}
}

tklist_iterator_t tklist_begin(tklist_t tklist) {
	return flist_begin(tklist);
}

tklist_iterator_t tklist_end(tklist_t tklist) {
	return flist_end(tklist);
}

tklist_iterator_t tklist_it_next(tklist_iterator_t iterator) {
	return flist_it_next(iterator);
}

bool tklist_it_valid(tklist_iterator_t iterator) {
	return flist_it_valid(iterator);
}

void tklist_clear(tklist_t tklist) {
	assert(tklist);
	while (tklist->head != tklist->tail) {
		tklist_pop_front(tklist);
	}
	token_free(tklist_front(tklist));
	flist_pop_front(tklist);
}

token_t tklist_get(tklist_iterator_t iterator) {
	return *(token_t *)flist_get(iterator);
}

void tklist_free(tklist_t tklist) {
	assert(tklist);
	tklist_clear(tklist);
	free(tklist);
}

token_t token_copy(token_t token_to_copy) {
	assert(token_to_copy);
	token_t new_token = malloc(sizeof(struct token));
	if (new_token == NULL) {
		return NULL;
	}

	if (token_to_copy->type == TK_IDENTIFIER
		|| token_to_copy->type == TK_KEYW_MAIN
		|| token_to_copy->type == TK_STR_LIT) {
		size_t size = strlen(token_to_copy->param.s) + 1;
		new_token->param.s = malloc(size);
		if (new_token->param.s == NULL) {
			free(new_token);
			return NULL;
		}
		memcpy((void *)new_token->param.s, token_to_copy->param.s, size);
	} else {
		new_token->param = token_to_copy->param;
	}
	new_token->type = token_to_copy->type;

	return new_token;
}

void token_free(token_t token) {
	if (token->type == TK_IDENTIFIER
		|| token->type == TK_KEYW_MAIN
		|| token->type == TK_STR_LIT) {
		free((void *)token->param.s);
	}
	free(token);
}
