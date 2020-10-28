#include <criterion/criterion.h>
#include "container/forward_list_private.h"

Test(forward_list, initialization) {
	flist_t flist = flist_init();
	cr_assert_not_null(flist);
	cr_assert_null(flist->head);
	flist_free(flist);
}

Test(forward_list, push_front_1) {
	flist_t flist = flist_init();
	cr_assert_not_null(flist);
	cr_assert_null(flist->head);

	struct vec2
	{
		int x;
		int y;
	};

	struct vec2 x = { 5 , 7 };
	
	flist_push_front(flist, &x, sizeof(x));
	cr_assert_not_null(flist);
	cr_assert_not_null(flist->head);
	struct vec2 *data = flist->head->data;
	cr_assert_not_null(data);
	cr_assert_eq(data->x, x.x);
	cr_assert_eq(data->y, x.y);

	flist_free(flist);
}

Test(forward_list, clear) {
	flist_t flist = flist_init();
	int x = 5;
	flist_push_front(flist, &x, sizeof(x));
	flist_push_front(flist, &x, sizeof(x));
	flist_push_front(flist, &x, sizeof(x));
	flist_push_front(flist, &x, sizeof(x));
	cr_assert_not_null(flist->head);
	flist_clear(flist);
	cr_assert_null(flist->head);
	flist_free(flist);
}

Test(forward_list, empty) {
	flist_t flist = flist_init();
	cr_expect(flist_empty(flist));
	int x = 0;
	flist_push_front(flist, &x, sizeof(x));
	cr_expect_not(flist_empty(flist));
	flist_free(flist);
}

Test(forward_list, flist_node) {
	flist_t flist = flist_init();
	cr_assert_not_null(flist);
	cr_assert_null(flist->head);

	for (int i = 0; i < 5; i++) {
		flist_push_front(flist, &i, sizeof(int));
	}
	cr_assert_not_null(flist);

	flist_node_t node = flist_front(flist);
	cr_assert_not_null(node);

	for (int i = 4; node; i--) {
		int *x = flist_node_data(node);
		cr_assert_eq(*x, i);
		node = flist_node_next(node);
	}
	cr_assert_null(node);

	flist_free(flist);
}
