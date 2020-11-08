#include <gtest/gtest.h>

extern "C" {
#include "hash_map.h"
}

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

TEST(hash_map, initialization) {
	hmap_t hmap = hmap_init(100, 1);
	ASSERT_NE(hmap, nullptr);
	EXPECT_EQ(hmap->size, 0);
	EXPECT_EQ(hmap->value_size, 1);
	EXPECT_EQ(hmap->bucket_count, 100);
	for (size_t i = 0; i < hmap->bucket_count; i++) {
		EXPECT_EQ(hmap->buckets[i], nullptr);
	}
	hmap_free(hmap);
}

TEST(hash_map, find_empty) {
	hmap_t hmap = hmap_init(100, 1);
	hmap_iterator_t it = hmap_find(hmap, "key");
	EXPECT_TRUE(hmap_it_eq(it, hmap_end(hmap)));
	hmap_free(hmap);
}

TEST(hash_map, find_add) {
	hmap_t hmap = hmap_init(100, sizeof(int));
	hmap_iterator_t it = hmap_find_add(hmap, "key");
	EXPECT_FALSE(hmap_it_eq(it, hmap_end(hmap)));
	int x = 5;
	hmap_set_value(it, &x);
	int *y;
	y = (int *)hmap_get_value(it);
	EXPECT_EQ(x, *y);
	hmap_iterator_t it2 = hmap_find(hmap, "key");
	EXPECT_TRUE(hmap_it_eq(it, it2));
	it2 = hmap_find(hmap, "k");
	EXPECT_TRUE(hmap_it_eq(it2, hmap_end(hmap)));
	hmap_free(hmap);
}

TEST(hash_map, insert) {
	hmap_t hmap = hmap_init(100, sizeof(int));
	int x = 5;
	hmap_iterator_t it = hmap_insert(hmap, "key", &x);
	EXPECT_FALSE(hmap_it_eq(it, hmap_end(hmap)));
	int *y;
	y = (int *)hmap_get_value(it);
	EXPECT_EQ(x, *y);
	hmap_iterator_t it2 = hmap_find(hmap, "key");
	EXPECT_TRUE(hmap_it_eq(it, it2));
	it2 = hmap_find(hmap, "k");
	EXPECT_TRUE(hmap_it_eq(it2, hmap_end(hmap)));
	hmap_free(hmap);
}

TEST(hash_map, erase) {
	hmap_t hmap = hmap_init(100, sizeof(int));
	int x = 5;
	hmap_iterator_t it = hmap_insert(hmap, "key", &x);
	EXPECT_FALSE(hmap_it_eq(it, hmap_end(hmap)));
	hmap_iterator_t it2 = hmap_find(hmap, "key");
	EXPECT_TRUE(hmap_it_eq(it, it2));
	it2 = hmap_find(hmap, "k");
	EXPECT_TRUE(hmap_it_eq(it2, hmap_end(hmap)));
	EXPECT_EQ(hmap_size(hmap), 1);
	it2 = hmap_end(hmap);
	it = hmap_erase(it);
	EXPECT_TRUE(hmap_it_eq(it, it2));
	EXPECT_EQ(hmap_size(hmap), 0);
	hmap_free(hmap);
}

TEST(hash_map, iterator_begin_end) {
	hmap_t hmap = hmap_init(100, 1);
	EXPECT_TRUE(hmap_it_eq(hmap_begin(hmap), hmap_end(hmap)));
	hmap_free(hmap);
}

TEST(hash_map, insert_hash_collision) {
	hmap_t hmap = hmap_init(2, 2);

	hmap_find_add(hmap, "a");
	hmap_find_add(hmap, "b");
	hmap_find_add(hmap, "c");
	hmap_find_add(hmap, "d");
	hmap_find_add(hmap, "e");
	hmap_find_add(hmap, "f");
	EXPECT_EQ(hmap_size(hmap), 6);

	hmap_iterator_t it = hmap_find(hmap, "c");
	hmap_iterator_t next = hmap_it_next(it);
	it = hmap_erase(it);
	EXPECT_TRUE(hmap_it_eq(it, next));
	EXPECT_EQ(hmap_size(hmap), 5);

	hmap_clear(hmap);
	EXPECT_TRUE(hmap_it_eq(hmap_begin(hmap), hmap_end(hmap)));
	EXPECT_EQ(hmap_size(hmap), 0);
	hmap_free(hmap);
}
