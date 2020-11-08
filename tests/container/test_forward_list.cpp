#include <gtest/gtest.h>

extern "C" {
#include "forward_list.h"
}

struct flist_node
{
	void *data;
	flist_node_t next;
};

struct flist
{
	flist_node_t head;
	size_t data_size;
};

TEST(forward_list, initialization) {
	flist_t flist = flist_init(1);
	ASSERT_NE(flist, nullptr);
	ASSERT_EQ(flist->head, nullptr);
	ASSERT_EQ(flist->data_size, 1);
	flist_free(flist);
}

TEST(forward_list, push_front_1) {
	struct vec2
	{
		int x;
		int y;
	};

	flist_t flist = flist_init(sizeof(struct vec2));
	ASSERT_NE(flist, nullptr);
	ASSERT_EQ(flist->head, nullptr);

	struct vec2 x = { 5 , 7 };
	
	flist_push_front(flist, &x);
	ASSERT_NE(flist, nullptr);
	ASSERT_NE(flist->head, nullptr);
	auto *data = (struct vec2 *)flist->head->data;
	ASSERT_NE(data, nullptr);
	ASSERT_EQ(data->x, x.x);
	ASSERT_EQ(data->y, x.y);

	flist_free(flist);
}

TEST(forward_list, clear) {
	flist_t flist = flist_init(sizeof(int));
	int x = 5;
	flist_push_front(flist, &x);
	flist_push_front(flist, &x);
	flist_push_front(flist, &x);
	flist_push_front(flist, &x);
	ASSERT_NE(flist->head, nullptr);
	flist_clear(flist);
	ASSERT_EQ(flist->head, nullptr);
	flist_free(flist);
}

TEST(forward_list, empty) {
	flist_t flist = flist_init(sizeof(int));
	EXPECT_TRUE(flist_empty(flist));
	int x = 0;
	flist_push_front(flist, &x);
	EXPECT_FALSE(flist_empty(flist));
	flist_free(flist);
}

TEST(forward_list, flist_iterator) {
	flist_t flist = flist_init(sizeof(int));
	ASSERT_NE(flist, nullptr);
	ASSERT_EQ(flist->head, nullptr);

	flist_iterator_t it = flist_begin(flist);
	ASSERT_FALSE(flist_it_valid(it));

	for (int i = 0; i < 5; i++) {
		flist_push_front(flist, &i);
	}
	ASSERT_NE(flist->head, nullptr);

	it = flist_begin(flist);
	ASSERT_TRUE(flist_it_valid(it));

	for (int i = 4; flist_it_valid(it); i--) {
		auto x = (int *)flist_get(it);
		ASSERT_EQ(*x, i);
		it = flist_it_next(it);
	}
	ASSERT_FALSE(flist_it_valid(it));

	it = flist_begin(flist);
	it = flist_it_advance(it, 5);
	ASSERT_FALSE(flist_it_valid(it));

	flist_free(flist);
}
