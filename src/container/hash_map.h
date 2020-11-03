#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdbool.h>

/**
 * @brief	handle for the hash map
*/
typedef struct hmap *hmap_t;

/**
 * @brief	typdef for string key type
*/
typedef const char *hmap_key_t;

/**
 * @brief	handle for hash map item
*/
typedef struct hmap_item *hmap_item_t;

/**
 * @brief	handle for hash map forward iterator
*/
typedef struct hmap_iterator
{
	hmap_item_t ptr;
	hmap_t hmap;
	size_t bucket_idx;
} hmap_iterator_t;

/**
 * @brief	Allocates and initializes the hash map
 * @param	bucket_count	number of buckets to allocate 
 * @param	data_size		the size of the data to be store in bytes 
 * @return	the initialized hash map or NULL if allocation fails
*/
hmap_t hmap_init(size_t bucket_count, size_t data_size);

/**
 * @brief	Returns the number of elements in the container
 * @param	hmap 
 * @return	the number of elements in the container
*/
size_t hmap_size(hmap_t hmap);

/**
 * @brief	Finds element with specific key
 * @param	hmap 
 * @param	key		the key value of the element to search for
 * @return	iterator to an element with key equivalent to key or if no such element is found hmap end iterator
*/
hmap_iterator_t hmap_find(hmap_t hmap, hmap_key_t key);

/**
 * @brief	Finds element with specific key or if it doesn't exist adds the key into the map
 * @param	hmap
 * @param	key		the key value used both to look up and to insert if not found
 * @return	iterator to an element with key equivalent to key or hmap end iterator if addition failed
*/
hmap_iterator_t hmap_find_add(hmap_t hmap, hmap_key_t key);

/**
 * @brief	Inserts an element or assigns to the current element if the key already exists
 * @param	hmap 
 * @param	key		the key value used both to look up and to insert if not found
 * @param	value	the value to insert or assign
 * @return iterator to an element with key equivalent to key or hmap end iterator if insertion failed
*/
hmap_iterator_t hmap_insert(hmap_t hmap, hmap_key_t key, void *value);

/**
 * @brief	Removes specified element from the container
 * @param	iterator	the iterator to the element to remove 
 * @return	iterator following the last removed element
*/
hmap_iterator_t hmap_erase(hmap_iterator_t iterator);

/**
 * @brief	Erases all elements from the container
 * @param	hmap 
 * @note	After this call size is zero and all iterators ale invalidated
*/
void hmap_clear(hmap_t hmap);

/**
 * @brief	Free the allocated list and delete all elements
 * @param	hmap
 * @note	Invalidates hmap and all iterators
*/
void hmap_free(hmap_t hmap);

/**
 * @brief	Return an iterator to the first element
 * @param	hmap 
 * @return	iterator to te first element or end iterator if map is empty
*/
hmap_iterator_t hmap_begin(hmap_t hmap);

/**
 * @brief	Returns in iterator following the last element
 * @param	hmap 
 * @return	past-the-end iterator
*/
hmap_iterator_t hmap_end(hmap_t hmap);

/**
 * @brief	Checks if iterator is pointing to a valid element
 * @param	iterator
 * @return	true if iterator is valid
*/
bool hmap_it_valid(hmap_iterator_t iterator);

/**
 * @brief	Test if iterators are equal
 * @param	lhs 
 * @param	rhs 
 * @return	true if lhs == rhs
*/
bool hmap_it_eq(hmap_iterator_t lhs, hmap_iterator_t rhs);

/**
 * @brief	Increment the iterator
 * @param	iterator 
 * @return	the next iterator or past-the-end iterator if we reached the end
*/
hmap_iterator_t hmap_it_next(hmap_iterator_t iterator);

/**
 * @brief	Access the key from the iterator
 * @param	iterator 
 * @return	the key of the element pointed to by the iterator
*/
hmap_key_t hmap_get_key(hmap_iterator_t iterator);

/**
 * @brief	Access the value from the iterator
 * @param	iterator 
 * @return	a pointer to the value
 * @note	calling this function on an invalidated iterator is an error
*/
void *hmap_get_value(hmap_iterator_t iterator);

/**
 * @brief	Copy the value data into the element
 * @param	iterator 
 * @param	value	the memory area to copy from
 * @note	calling thsi function on an ivalidate iterator is an error
*/
void hmap_set_value(hmap_iterator_t iterator, void *value);

#endif // !HASH_MAP_H
