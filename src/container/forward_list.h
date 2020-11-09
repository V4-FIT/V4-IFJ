#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include <stdbool.h>
#include <stdlib.h>

/**
 * @brief	Handle for the forward list
*/
typedef struct flist *flist_t;

typedef struct flist_node *flist_node_t;

/**
 * @brief	handle for a forward list iterator
*/
typedef struct iterator
{
	flist_node_t ptr;
	flist_t flist;
} flist_iterator_t;

/**
 * @brief	Allocates and initializes a forward list
 * @return	the initialized forward list or NULL if allocation fails
 */
flist_t flist_init(size_t data_size);

/**
 * @brief	Checks if the list is empty
 * @param	flist
 * @param	data_size	The size of the data to be stored in bytes
 * @return	true when empty
 */
bool flist_empty(flist_t flist);

/**
 * @brief	Inserts a new node to the front of the list
 * @param	flist
 * @param	data		The memory area to copy from
 * @return	true on success, false on allocation error
 */
bool flist_push_front(flist_t flist, void *data);

/**
 * @brief	Allocates memory for a new node at the front of the list
 * @param	flist
 * @return	iterator to the inserted element or flist_end(flist) iterator if init failed
 */
flist_iterator_t flist_init_front(flist_t flist);

/**
 * @brief	Deletes a node from the front of the list
 * @param	flist
 */
void flist_pop_front(flist_t flist);

/**
 * @brief	Returns the data at the front of the list
 * @param	flist 
 * @return	pointer to the data at the front or NULL if the list is empty
*/
void *flist_front(flist_t flist);

/**
 * @brief	Deletes all nodes from the list
 * @param	flist
 * @note	Invalidates all iterators
 */
void flist_clear(flist_t flist);

/**
 * @brief	Free the allocated list and delete all nodes
 * @param	flist
 * @note	Invalidates flist and all iterators
 */
void flist_free(flist_t flist);

/**
 * @brief	Returns an iterator to the beginning
 * @param	flist
 * @return	iterator to the first element, if the flist is empty the returned iterator will be equal to flist_end(flist)
 */
flist_iterator_t flist_begin(flist_t flist);

/**
 * @brief	Returns an iterator to the end
 * @param	flist
 * @return	iterator to the element following the last element
 */
flist_iterator_t flist_end(flist_t flist);

/**
 * @brief	Increment an iterator
 * @param	iterator
 * @return	the next iterator in the list or flist_end(flist) if we reached the end
 * @note	calling this function on an unitialized iterator is an error
 */
flist_iterator_t flist_it_next(flist_iterator_t iterator);

/**
 * @brief	Advances an iterator by given distance
 * @param	iterator
 * @return	the nth iterator in the list or flist_end(flist) if we reached the end
 * @note	calling this function on an unitialized iterator is an error
 */
flist_iterator_t flist_it_advance(flist_iterator_t iterator, size_t distance);

/**
 * @brief	iterator != flist_end_iterator
 * @param	iterator 
 * @return	return true if the iterator is valid
*/
bool flist_it_valid(flist_iterator_t iterator);

/**
 * @brief	Access the data from the iterator
 * @param	iterator
 * @return	a pointer to the iterator data or NULL if iterator isn't pointing to a valid element
 * @note	calling this function on an iterator pointing to an invalid element is an error
 */
void *flist_get(flist_iterator_t iterator);

/**
 * @brief	Copy data into the list node
 * @param	iterator
 * @param	data		The memory area to copy from
 * @note	calling this function on an iterator pointing to an invalid element is an error
 */
void flist_set(flist_iterator_t iterator, void *data);

#endif // !FORWARD_LIST_H
