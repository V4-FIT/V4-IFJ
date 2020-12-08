#include "hash_map.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

////// Private

#define FNV_BASE 2166136261u
#define FNV_PRIME 16777619u

struct hmap
{
	size_t size;
	size_t value_size;
	size_t bucket_count;
	hmap_item_t buckets[];
};

struct hmap_item
{
	hmap_key_t key;
	void *value;
	hmap_item_t next;
};

// 32-bit FNV-1a hashing algorithm
static uint32_t hmap_hash(hmap_key_t key) {
	uint32_t hash = FNV_BASE;
	for (const uint8_t *p = (const uint8_t *)key; *p != '\0'; p++) {
		hash ^= *p;
		hash *= FNV_PRIME;
	}
	return hash;
}

// return true if keys are equal
static bool hmap_key_eq(hmap_key_t lhs, hmap_key_t rhs) {
	return !strcmp(lhs, rhs);
}

// return an allocated item or NULL if allocation fails
static hmap_item_t hmap_item_init(hmap_key_t key, size_t value_size) {
	assert(key);
	hmap_item_t item = malloc(sizeof(struct hmap_item));
	if (item == NULL) {
		return NULL;
	}

	size_t key_size = strlen(key) + 1;
	item->key = malloc(key_size);
	if (item->key == NULL) {
		free(item);
		return NULL;
	}

	item->value = malloc(value_size);
	if (item->value == NULL) {
		free((void *)item->key);
		free(item);
		return NULL;
	}

	memcpy((void *)item->key, key, key_size);
	memset(item->value, 0, value_size);
	item->next = NULL;

	return item;
}

////// Public

hmap_t hmap_init(size_t bucket_count, size_t value_size) {
	assert(bucket_count);
	hmap_t hmap = malloc(sizeof(struct hmap) + bucket_count * sizeof(hmap_item_t));
	if (hmap == NULL) {
		return NULL;
	}

	hmap->size = 0;
	hmap->value_size = value_size;
	hmap->bucket_count = bucket_count;

	for (size_t i = 0; i < bucket_count; i++) {
		hmap->buckets[i] = NULL;
	}

	return hmap;
}

size_t hmap_size(hmap_t hmap) {
	assert(hmap);
	return hmap->size;
}

hmap_iterator_t hmap_find(hmap_t hmap, hmap_key_t key) {
	assert(hmap && key);
	const size_t hash = hmap_hash(key);
	const size_t idx = hash % hmap->bucket_count;

	hmap_iterator_t it = {.ptr = hmap->buckets[idx], .hmap = hmap, .bucket_idx = idx};
	while (it.bucket_idx == idx && hmap_it_valid(it)) {
		if (hmap_key_eq(key, it.ptr->key)) {
			return it;
		}
		it = hmap_it_next(it);
	}

	return hmap_end(hmap);
}

hmap_iterator_t hmap_find_add(hmap_t hmap, hmap_key_t key) {
	assert(hmap && key);

	hmap_iterator_t it = hmap_find(hmap, key);
	if (!hmap_it_eq(it, hmap_end(hmap))) {
		return it;
	}

	const size_t hash = hmap_hash(key);
	const size_t idx = hash % hmap->bucket_count;

	it.hmap = hmap;
	it.bucket_idx = idx;
	it.ptr = hmap_item_init(key, hmap->value_size);
	if (it.ptr == NULL) {
		return hmap_end(hmap);
	}

	hmap_item_t item = hmap->buckets[idx];
	if (item == NULL) {
		hmap->buckets[idx] = it.ptr;
	} else {
		while (item->next) {
			item = item->next;
		}
		item->next = it.ptr;
	}
	hmap->size++;

	return it;
}

hmap_iterator_t hmap_insert(hmap_t hmap, hmap_key_t key, void *value) {
	assert(hmap && key && value);
	hmap_iterator_t it = hmap_find_add(hmap, key);
	if (!hmap_it_eq(it, hmap_end(hmap))) {
		hmap_set_value(it, value);
	}
	return it;
}

hmap_iterator_t hmap_erase(hmap_iterator_t it) {
	assert(hmap_it_valid(it));
	hmap_iterator_t ret = hmap_it_next(it);
	hmap_t hmap = it.hmap;
	size_t idx = it.bucket_idx;

	if (hmap->buckets[idx] == it.ptr) {
		hmap->buckets[idx] = it.ptr->next;
	} else {
		hmap_iterator_t it_prev = {.ptr = hmap->buckets[idx], .hmap = hmap, .bucket_idx = idx};
		while (it_prev.ptr->next != it.ptr) {
			it_prev.ptr = it_prev.ptr->next;
		}
		it_prev.ptr->next = it.ptr->next;
	}

	free((void *)it.ptr->key);
	free(it.ptr->value);
	free(it.ptr);
	it.hmap->size--;

	return ret;
}

void hmap_clear(hmap_t hmap) {
	assert(hmap);
	hmap_iterator_t it = hmap_begin(hmap);
	while (!hmap_it_eq(it, hmap_end(hmap))) {
		it = hmap_erase(it);
	}
}

void hmap_free(hmap_t hmap) {
	assert(hmap);
	hmap_clear(hmap);
	free(hmap);
}

hmap_iterator_t hmap_begin(hmap_t hmap) {
	assert(hmap);
	hmap_iterator_t it = {.ptr = NULL, .hmap = hmap, .bucket_idx = 0};

	// find the next bucket with a valid element
	for (size_t i = 0; i < hmap->bucket_count; i++) {
		if ((it.ptr = it.hmap->buckets[i]) != NULL) {
			it.bucket_idx = i;
			break;
		}
	}

	return it;
}

hmap_iterator_t hmap_end(hmap_t hmap) {
	assert(hmap);
	hmap_iterator_t it = {.ptr = NULL, .hmap = hmap, .bucket_idx = 0};

	for (int64_t i = hmap->bucket_count - 1; i >= 0; i--) {
		if (hmap->buckets[i] != NULL) {
			it.bucket_idx = i + 1;
			break;
		}
	}

	return it;
}

bool hmap_it_valid(hmap_iterator_t it) {
	return it.ptr != NULL;
}

bool hmap_it_eq(hmap_iterator_t lhs, hmap_iterator_t rhs) {
	return lhs.ptr == rhs.ptr && lhs.hmap == rhs.hmap;
}

hmap_iterator_t hmap_it_next(hmap_iterator_t it) {
	assert(hmap_it_valid(it));

	// if we can, follow the linked list first
	if (it.ptr->next != NULL) {
		it.ptr = it.ptr->next;
		return it;
	}

	// find the next bucket with a valid element
	for (size_t i = it.bucket_idx + 1; i < it.hmap->bucket_count; i++) {
		if ((it.ptr = it.hmap->buckets[i]) != NULL) {
			it.bucket_idx = i;
			return it;
		}
	}

	return hmap_end(it.hmap);
}

hmap_key_t hmap_get_key(hmap_iterator_t it) {
	assert(hmap_it_valid(it));
	return it.ptr->key;
}

void *hmap_get_value(hmap_iterator_t it) {
	assert(hmap_it_valid(it));
	return it.ptr->value;
}

void hmap_set_value(hmap_iterator_t it, void *value) {
	assert(hmap_it_valid(it) && value);
	memcpy(it.ptr->value, value, it.hmap->value_size);
}
