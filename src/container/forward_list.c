#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "forward_list_private.h"

////// Private

////// Public

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

flist_node_t flist_front(flist_t flist) {
	assert(flist);
	return flist->head;
}

flist_node_t flist_node_next(flist_node_t flist_node) {
	assert(flist_node);
	return flist_node->next;
}

void* flist_node_data(flist_node_t flist_node) {
	if (flist_node) {
		return flist_node->data;
	}
	return NULL;
}
