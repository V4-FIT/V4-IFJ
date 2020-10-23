#include <criterion/criterion.h>
#include "container/forward_list.h"

struct forward_list
{
	void* data;
	flist_handle_t next;
};

Test( forward_list, push_front_1 )
{
	flist_create( list );
	int x = 5;
	flist_push_front( &list, &x, sizeof( x ) );
	cr_assert_not_null( list );
	cr_assert_null( list->next );
	cr_assert_eq( *(int*)list->data, 5 );

	flist_clear( &list );
}

Test( forward_list, push_front_2 )
{
	flist_create( list );
	int x = 5;
	flist_push_front( &list, &x, sizeof( x ) );
	cr_assert_not_null( list );
	cr_assert_null( list->next );
	cr_assert_eq( *(int*)list->data, x );

	x += 2;
	flist_push_front( &list, &x, sizeof( x ) );
	cr_assert_not_null( list );
	cr_assert_not_null( list->next );
	cr_assert_eq( *(int*)list->data, x );

	list = list->next;
	cr_assert_not_null( list );
	cr_assert_null( list->next );
	cr_assert_eq( *(int*)list->data, x - 2 );

	flist_clear( &list );
}

Test( forward_list, clear )
{
	flist_create( list );
	int x = 5;
	flist_push_front( &list, &x, sizeof( x ) );
	flist_push_front( &list, &x, sizeof( x ) );
	flist_push_front( &list, &x, sizeof( x ) );
	flist_push_front( &list, &x, sizeof( x ) );
	cr_assert_not_null( list );
	flist_clear( &list );
	cr_assert_null( list );
}
