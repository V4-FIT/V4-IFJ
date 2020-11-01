#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdbool.h>

typedef struct hmap *hmap_t;

typedef const char *hmap_key_t;

typedef void *hmap_value_t;

typedef struct hmap_item *hmap_item_t;

typedef struct hmap_iterator
{
	hmap_item_t ptr;
	hmap_t hmap;
	size_t bucket_idx;
} hmap_iterator_t;

hmap_t hmap_init(size_t bucket_count, size_t data_size);

size_t hmap_size(hmap_t hmap);

hmap_iterator_t hmap_find(hmap_t hmap, hmap_key_t key);

hmap_iterator_t hmap_find_add(hmap_t hmap, hmap_key_t key);

hmap_iterator_t hmap_insert(hmap_t hmap, hmap_key_t key, hmap_value_t value);

hmap_iterator_t hmap_erase(hmap_iterator_t iterator);

void hmap_clear(hmap_t hmap);

void hmap_free(hmap_t hmap);

hmap_iterator_t hmap_begin(hmap_t hmap);

hmap_iterator_t hmap_end(hmap_t hmap);

bool hmap_it_valid(hmap_iterator_t it);

bool hmap_it_eq(hmap_iterator_t lhs, hmap_iterator_t rhs);

hmap_iterator_t hmap_it_next(hmap_iterator_t iterator);

hmap_key_t hmap_get_key(hmap_iterator_t iterator);

hmap_value_t hmap_get_value(hmap_iterator_t iterator);

void hmap_set_value(hmap_iterator_t iterator, hmap_value_t value);


#endif // !HASH_MAP_H
