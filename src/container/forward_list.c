#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "forward_list.h"

struct forward_list
{
	void* data;
	flist_handle_t next;
};

void flist_push_front( flist_handle_t* flist, void* data, size_t data_size )
{
	assert( flist && data && data_size );
	flist_handle_t new_node = (flist_handle_t)malloc( sizeof( flist_t ) );
	assert( new_node );

	new_node->data = malloc( data_size );
	assert( new_node->data );

	for( size_t i = 0; i < data_size; i++ )
	{
		*( (uint8_t*)( new_node->data) + i  ) = *( (uint8_t*)( data ) + i );
	}

	new_node->next = *flist;
	*flist = new_node;
}

void flist_clear( flist_handle_t* flist )
{
	flist_handle_t current = *flist;
	flist_handle_t next;

	while( current != NULL )
	{
		next = current->next;
		free( current->data );
		free( current );
		current = next;
	}

	*flist = NULL;
}

