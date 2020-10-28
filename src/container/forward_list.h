#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include <stdbool.h>

typedef struct forward_list *flist_t;

#define flist_create(name) flist_t name = NULL;

void flist_push_front(flist_t *flist, void *data, size_t data_size);

void flist_clear(flist_t *flist);

#endif // !FORWARD_LIST_H
