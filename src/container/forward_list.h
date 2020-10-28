#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include <stdbool.h>

typedef struct flist *flist_t;

typedef struct flist_node *flist_node_t;

flist_t flist_init();

bool flist_empty(flist_t flist);

bool flist_push_front(flist_t flist, void *data, size_t data_size);

void flist_pop_front(flist_t flist);

void flist_clear(flist_t flist);

void flist_free(flist_t flist);

flist_node_t flist_front(flist_t flist);

flist_node_t flist_node_next(flist_node_t flist_node);

void *flist_node_data(flist_node_t flist_node);

#endif // !FORWARD_LIST_H
