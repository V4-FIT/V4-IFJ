#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include <stdbool.h>
#include <stdlib.h>

/**
 * @brief	Handle for the forward list
*/
typedef struct flist *flist_t;

/**
 * @brief	handle for a forward list iterator
*/
typedef struct flist_node *flist_iterator_t;

/**
 * @brief	Allocates and initializes a forward list
 * @return	the initialized forward list
 */
flist_t flist_init();

/**
 * @brief	Checks if the list is empty
 * @param	flist
 * @return	true when empty
 */
bool flist_empty(flist_t flist);

/**
 * @brief	Inserts a new node to the front of the list
 * @param	flist
 * @param	*data		The memory area to copy from
 * @param	data_size	The number of bytes to copy
 * @return	true on success, false on allocation error
 */
bool flist_push_front(flist_t flist, void *data, size_t data_size);

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
 * @brief	Returns and iterator to the beginning
 * @param	flist
 * @return	iterator to the first element, if the flist is empty the returned iterator will be equal to flist_end(flist)
 */
flist_iterator_t flist_begin(flist_t flist);

/**
 * @brief	Returns and iterator to the end
 * @param	flist
 * @return	iterator to the element following the last element
 */
flist_iterator_t flist_end(flist_t flist);

/**
 * @brief	Increment an iterator
 * @param	flist_iterator
 * @return	the next iterator in the list or flist_end(flist) if we reached the end
 * @note	calling this function on an unitialized flist_iterator is an error
 */
flist_iterator_t flist_next(flist_iterator_t flist_iterator);

/**
 * @brief	Advances an iterator by given distance
 * @param	flist_iterator
 * @return	the nth iterator in the list or flist_end(flist) if we reached the end
 * @note	calling this function on an unitialized flist_iterator is an error
 */
flist_iterator_t flist_advance(flist_iterator_t flist_iterator, size_t distance);

/**
 * @brief	Access the data from the iterator
 * @param	flist_iterator
 * @return	a pointer to the iterator data or NULL if iterator isn't pointing to a valid element
 * @note	calling this function on an iterator pointing to an invalid element is an error
 */
void *flist_get(flist_iterator_t flist_iterator);

#endif // !FORWARD_LIST_H
