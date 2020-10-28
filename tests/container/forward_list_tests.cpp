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
};

TEST(forward_list, initialization) {
	flist_t flist = flist_init();
	ASSERT_NE(flist, nullptr);
	ASSERT_EQ(flist->head, nullptr);
	flist_free(flist);
}

TEST(forward_list, push_front_1) {
	flist_t flist = flist_init();
	ASSERT_NE(flist, nullptr);
	ASSERT_EQ(flist->head, nullptr);

	struct vec2
	{
		int x;
		int y;
	};

	struct vec2 x = { 5 , 7 };
	
	flist_push_front(flist, &x, sizeof(x));
	ASSERT_NE(flist, nullptr);
	ASSERT_NE(flist->head, nullptr);
	auto *data = (struct vec2 *)flist->head->data;
	ASSERT_NE(data, nullptr);
	ASSERT_EQ(data->x, x.x);
	ASSERT_EQ(data->y, x.y);

	flist_free(flist);
}

TEST(forward_list, clear) {
	flist_t flist = flist_init();
	int x = 5;
	flist_push_front(flist, &x, sizeof(x));
	flist_push_front(flist, &x, sizeof(x));
	flist_push_front(flist, &x, sizeof(x));
	flist_push_front(flist, &x, sizeof(x));
	ASSERT_NE(flist->head, nullptr);
	flist_clear(flist);
	ASSERT_EQ(flist->head, nullptr);
	flist_free(flist);
}

TEST(forward_list, empty) {
	flist_t flist = flist_init();
	EXPECT_TRUE(flist_empty(flist));
	int x = 0;
	flist_push_front(flist, &x, sizeof(x));
	EXPECT_FALSE(flist_empty(flist));
	flist_free(flist);
}

TEST(forward_list, flist_node) {
	flist_t flist = flist_init();
	ASSERT_NE(flist, nullptr);
	ASSERT_EQ(flist->head, nullptr);

	for (int i = 0; i < 5; i++) {
		flist_push_front(flist, &i, sizeof(int));
	}
	ASSERT_NE(flist, nullptr);

	flist_node_t node = flist_front(flist);
	ASSERT_NE(node, nullptr);

	for (int i = 4; node; i--) {
		auto x = (int *)flist_node_data(node);
		ASSERT_EQ(*x, i);
		node = flist_node_next(node);
	}
	ASSERT_EQ(node, nullptr);

	flist_free(flist);
}
