#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "forward_list.h"

struct forward_list
{
	void *data;
	flist_t next;
};

void flist_push_front(flist_t *flist, void *data, size_t data_size) {
	assert(flist && data && data_size);
	flist_t new_node = (flist_t)malloc(sizeof(struct forward_list));
	assert(new_node);

	new_node->data = malloc(data_size);
	assert(new_node->data);

	memcpy(new_node->data, data, data_size);

	new_node->next = *flist;
	*flist = new_node;
}

void flist_clear(flist_t *flist) {
	flist_t current = *flist;
	flist_t next;

	while (current != NULL) {
		next = current->next;
		free(current->data);
		free(current);
		current = next;
	}

	*flist = NULL;
}

