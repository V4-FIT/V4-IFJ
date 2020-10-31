#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "forward_list.h"

typedef struct flist_node *flist_node_t;

struct flist_node
{
	void *data;
	flist_node_t next;
};

struct flist
{
	flist_node_t head;
};

flist_t flist_init() {
	flist_t flist = malloc(sizeof(struct flist));
	if (flist == NULL) {
		return NULL;
	}

	flist->head = NULL;
	return flist;
}

bool flist_empty(flist_t flist) {
	assert(flist);
	return flist->head == NULL;
}

bool flist_push_front(flist_t flist, void *data, size_t data_size) {
	assert(flist && data && data_size);
	flist_node_t new_node = malloc(sizeof(struct flist_node));
	if (new_node == NULL) {
		return false;
	}

	new_node->data = malloc(data_size);
	if (new_node->data == NULL) {
		free(new_node);
		return false;
	}

	memcpy(new_node->data, data, data_size);

	new_node->next = flist->head;
	flist->head = new_node;
	return true;
}

void flist_pop_front(flist_t flist) {
	assert(flist);
	flist_node_t node = flist->head;
	if (node) {
		flist->head = node->next;
		free(node->data);
		free(node);
	}
}

void *flist_front(flist_t flist) {
	assert(flist);
	if (flist->head) {
		return flist->head->data;
	} else {
		return NULL;
	}
}

void flist_clear(flist_t flist) {
	assert(flist);
	while (flist->head) {
		flist_pop_front(flist);
	}
}

void flist_free(flist_t flist) {
	assert(flist);
	flist_clear(flist);
	free(flist);
}

flist_iterator_t flist_begin(flist_t flist) {
	assert(flist);
	return flist->head;
}

flist_iterator_t flist_end(flist_t flist) {
	return NULL;
}

flist_iterator_t flist_next(flist_iterator_t flist_iterator) {
	if (flist_iterator) {
		return flist_iterator->next;
	}
	return NULL;
}

flist_iterator_t flist_advance(flist_iterator_t flist_iterator, size_t distance) {
	for (size_t i = 0; i < distance; i++) {
		flist_iterator = flist_next(flist_iterator);
	}
	return flist_iterator;
}

void* flist_get(flist_iterator_t flist_iterator) {
	if (flist_iterator) {
		return flist_iterator->data;
	}
	return NULL;
}
