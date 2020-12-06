#ifndef IFJ_TOKENS_H
#define IFJ_TOKENS_H

#include <stdint.h>

#include "char_sequence.h"
#include "forward_list.h"

/*tokens produced by scanner*/
typedef enum
{
	// Special purpose
	TK_ERROR,
	TK_EOF,

	// Separators
	TK_L_PARENTHESIS,
	TK_R_PARENTHESIS,
	TK_L_CURLY,
	TK_R_CURLY,
	TK_COMMA,
	TK_SEMICOLON,
	TK_EOL,

	// Literals
	TK_STR_LIT,
	TK_INT_LIT,
	TK_FLOAT_LIT,

	// Arithmetic operators
	TK_PLUS,
	TK_MINUS,
	TK_MULTIPLY,
	TK_DIVIDE,

	// Relational operators
	TK_EQUAL,
	TK_NOT_EQUAL,
	TK_LESS,
	TK_LESS_EQUAL,
	TK_GREATER,
	TK_GREATER_EQUAL,

	// Logical operators
	TK_OR,
	TK_AND,
	TK_NOT,

	// Assignment operators
	TK_VAR_INIT,
	TK_ASSIGN,
	TK_PLUS_ASSIGN,
	TK_MINUS_ASSIGN,
	TK_MULTIPLY_ASSIGN,
	TK_DIVIDE_ASSIGN,

	// Identifiers
	TK_IDENTIFIER,
	TK_UNDERSCORE,

	// Keywords
	TK_KEYW_PACKAGE,
	TK_KEYW_FUNC,
	TK_KEYW_MAIN,
	TK_KEYW_RETURN,
	TK_KEYW_IF,
	TK_KEYW_ELSE,
	TK_KEYW_FOR,
	TK_KEYW_TRUE,
	TK_KEYW_FALSE,
	TK_KEYW_INT,
	TK_KEYW_FLOAT64,
	TK_KEYW_STRING,
	TK_KEYW_BOOL,
} token_type_t;

extern const char *tk2str_map[];

typedef union
{
	int64_t i;
	double f;
} tk_param_t;

/*data type to represent tokens created by scanner*/
struct token
{
	token_type_t type;
	tk_param_t param;
	int line_number;
	const char *lexeme;
};

typedef struct token *token_t;

// token_list is a specialization of forward_list
typedef flist_t tklist_t;
typedef flist_iterator_t tklist_iterator_t;

/**
 * @brief	Allocates and initializes a token list
 * @return	the initialized forward list or NULL if allocation fails
 */
tklist_t tklist_init();

/**
 * @brief	Deletes a token from the front of the list
 * @param	tklist
 */
void tklist_pop_front(tklist_t tklist);

/**
 * @brief	Inserts a new token to the back of the list
 * @param	tklist
 * @param	token
 * @return	true on success, false on allocation error
 */
bool tklist_push_back(tklist_t tklist, token_t token);

/**
 * @brief	Returns the token at the front of the list
 * @param	tklist 
 * @return	pointer to the token at the front or NULL if the list is empty
*/
token_t tklist_front(tklist_t tklist);

/**
 * @brief	Returns the second token from the front of the list
 * @param	tklist 
 * @return	pointer to the second token from the front or NULL if the list is empty or there is no second
*/
token_t tklist_second(tklist_t tklist);

/**
 * @brief	Returns an iterator to the beginning
 * @param	tklist
 * @return	iterator to the first element, if the tklist is empty the returned iterator will be equal to tklist_end(tklist)
 */
tklist_iterator_t tklist_begin(tklist_t tklist);

/**
 * @brief	Returns an iterator to the end
 * @param	tklist
 * @return	iterator to the element following the last element
 */
tklist_iterator_t tklist_end(tklist_t tklist);

/**
 * @brief	Increment an iterator
 * @param	iterator
 * @return	the next iterator in the list or tklist_end(flist) if we reached the end
 * @note	calling this function on an unitialized iterator is an error
 */
tklist_iterator_t tklist_it_next(tklist_iterator_t iterator);

/**
 * @brief	iterator != tklist_end_iterator
 * @param	iterator 
 * @return	return true if the iterator is valid
*/
bool tklist_it_valid(tklist_iterator_t iterator);

/**
 * @brief	Access the data from the iterator
 * @param	iterator
 * @return	a pointer to the iterator data or NULL if iterator isn't pointing to a valid element
 * @note	calling this function on an iterator pointing to an invalid element is an error
 */
token_t tklist_get(tklist_iterator_t iterator);

/**
 * @brief	Free the allocated list and delete all nodes
 * @param	flist
 * @note	Invalidates tklist and all iterators
 */
void tklist_free(tklist_t tklist);

/**
 * @brief	Create a deep copy of a token
 * @param	to_copy 
 * @return	Pointer to the new copy or NULL on allocation error
*/
token_t token_copy(token_t to_copy);

/**
 * @brief	Free allocated memory
 * @param	token 
*/
void token_free(token_t token);

#endif // !IFJ_TOKENS_H
