#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct flist *flist_t;

typedef struct flist_node *flist_node_t;

/**
 * Allocates and initializes a forward list
 * @return the initialized forward list
 */
flist_t flist_init();

/**
 * Checks if the list is empty
 * @param	flist
 * @return	true when empty
 */
bool flist_empty(flist_t flist);

/**
 * Inserts a new node to the front of the list
 * @param	flist
 * @param [in]	*data		The memory area to copy from
 * @param [in]	data_size	The number of bytes to copy
 * @return true on success, false on allocation error
 */
bool flist_push_front(flist_t flist, void *data, size_t data_size);

/**
 * Deletes a node from the front of the list
 * @param	flist
 */
void flist_pop_front(flist_t flist);

/**
 * Deletes all nodes from the list
 * @param	flist
 * @note	Invalidates all flist_node_t references
 */
void flist_clear(flist_t flist);

/**
 * Free the allocated list and delete all nodes
 * @param	flist
 * @note	Invalidates flist and all flist_node_t references 
 */
void flist_free(flist_t flist);

/**
 * Initializes flist_node_t
 * @param	flist
 * @return	node at front of the list or NULL if the list is empty
 */
flist_node_t flist_front(flist_t flist);

/**
 * Advances the node to the next one in the list
 * @param	flist_node
 * @return	the next node in the list or NULL if we reached the end
 * @note	calling this function on an unitialized flist_node is an error
 */
flist_node_t flist_node_next(flist_node_t flist_node);

/**
 * Access the data from the node
 * @param	flist_node
 * @return	a pointer to the data of flist_node or NULL if flist_node is also NULL
 * @note	calling this function on an unitialized flist_node is an error
 */
void *flist_node_data(flist_node_t flist_node);

#endif // !FORWARD_LIST_H
