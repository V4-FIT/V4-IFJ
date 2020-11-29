#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "forward_list.h"

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

flist_t flist_init(size_t data_size) {
	flist_t flist = malloc(sizeof(struct flist));
	if (flist == NULL) {
		return NULL;
	}

	flist->head = NULL;
	flist->tail = NULL;
	flist->data_size = data_size;
	return flist;
}

bool flist_empty(flist_t flist) {
	assert(flist);
	return flist->head == NULL;
}

bool flist_push_front(flist_t flist, void *data) {
	flist_iterator_t it = flist_init_front(flist);
	if (!flist_it_valid(it)) {
		return false;
	}

	memcpy(it.ptr->data, data, flist->data_size);
	return true;
}

bool flist_push_back(flist_t flist, void *data) {
	flist_iterator_t it = flist_init_back(flist);
	if (!flist_it_valid(it)) {
		return false;
	}

	memcpy(it.ptr->data, data, flist->data_size);
	return true;
}

flist_iterator_t flist_init_front(flist_t flist) {
	assert(flist);
	flist_node_t new_node = malloc(sizeof(struct flist_node));
	if (new_node == NULL) {
		return flist_end(flist);
	}

	new_node->data = malloc(flist->data_size);
	if (new_node->data == NULL) {
		free(new_node);
		return flist_end(flist);
	}

	new_node->next = flist->head;
	flist->head = new_node;
	if (flist->tail == NULL) {
		flist->tail = new_node;
	}
	return flist_begin(flist);
}

flist_iterator_t flist_init_back(flist_t flist) {
	assert(flist);
	flist_node_t new_node = malloc(sizeof(struct flist_node));
	if (new_node == NULL) {
		return flist_end(flist);
	}

	new_node->data = malloc(flist->data_size);
	if (new_node->data == NULL) {
		free(new_node);
		return flist_end(flist);
	}

	new_node->next = NULL;
	if (flist->tail) {
		flist->tail->next = new_node;
	}
	flist->tail = new_node;
	if (flist->head == NULL) {
		flist->head = new_node;
	}
	flist_iterator_t it = {flist->tail, flist};
	return it;
}

void flist_pop_front(flist_t flist) {
	assert(flist);
	flist_node_t node = flist->head;
	if (node) {
		if (flist->tail == flist->head) {
			flist->tail = NULL;
		}
		flist->head = node->next;
		free(node->data);
		free(node);
	}
}

void flist_pop_back(flist_t flist) {
	assert(flist);
	flist_iterator_t it = flist_begin(flist);
	if (!flist_it_valid(it)) {
		return;
	}

	assert(flist->tail);
	flist_node_t node = flist->tail;
	if (it.ptr == flist->tail) {
		flist->tail = NULL;
		flist->head = NULL;
	} else {
		while (it.ptr->next != flist->tail) {
			it = flist_it_next(it);
		};
		flist->tail = it.ptr;
	}
	free(node->data);
	free(node);
}

void *flist_front(flist_t flist) {
	assert(flist);
	if (flist->head) {
		return flist->head->data;
	} else {
		return NULL;
	}
}

void *flist_back(flist_t flist) {
	assert(flist);
	if (flist->tail) {
		return flist->tail->data;
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
	flist_iterator_t it = {flist->head, flist};
	return it;
}

flist_iterator_t flist_end(flist_t flist) {
	flist_iterator_t it = {NULL, flist};
	return it;
}

flist_iterator_t flist_it_next(flist_iterator_t it) {
	if (it.ptr) {
		it.ptr = it.ptr->next;
		return it;
	}
	return it;
}

flist_iterator_t flist_it_advance(flist_iterator_t it, size_t distance) {
	for (size_t i = 0; i < distance; i++) {
		it = flist_it_next(it);
	}
	return it;
}

bool flist_it_valid(flist_iterator_t it) {
	return it.ptr != NULL;
}

void *flist_get(flist_iterator_t it) {
	if (it.ptr) {
		return it.ptr->data;
	}
	return NULL;
}

void flist_set(flist_iterator_t it, void *data) {
	assert(it.ptr);
	memcpy(it.ptr->data, data, it.flist->data_size);
}
