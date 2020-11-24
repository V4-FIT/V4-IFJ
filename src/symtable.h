#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "forward_list.h"

#include <stdbool.h>

#include "hash_map.h"
#include "tokens.h"

/**
 * @brief	handle for the symtable
*/
typedef struct symtable *symtable_t;

/**
 * @brief	symbol type
*/
typedef enum
{
	ST_VAR,
	ST_FUNC
} symbol_type_t;

/**
 * @brief	data types for variables, parameters, returns
*/
typedef enum
{
	DT_INTEGER,
	DT_FLOAT64,
	DT_STRING,
	DT_BOOL,
	DT_UNDEFINED
} data_type_t;

/**
 * @brief	stored information about variables
*/
typedef struct
{
	data_type_t data_type;
	//TODO: SYMTABLE - add required attributes for variables
} sym_var_t;

/**
 * @brief	stored information about functions
*/
typedef struct
{
	unsigned int param_count;
	flist_t param_list;
	unsigned int return_count;
	flist_t return_list;
	bool defined;
} sym_func_t;

/**
 * @brief	common data type for storing information about symbols
*/
typedef struct
{
	const char *name;
	symbol_type_t type;
	union
	{
		sym_var_t var;
		sym_func_t func;
	};
} symbol_t;

/**
 * @brief	handle for a concrete symbol
*/
typedef struct
{
	hmap_iterator_t it;
	symbol_t *symbol;
	symtable_t symtable;
} symbol_ref_t;

/**
 * @brief	Allocates and initializes the symbol table, enters global scope
 * @return	the initialized symtable or NULL if allocation fails
*/
symtable_t symtable_init();

/**
 * @brief	Creates a new scope for storing symbols
 * @param	symtable
 * @return	true on success, false on allocation error
*/
bool symtable_enter_scope(symtable_t symtable);

/**
 * @brief	Exits the current scope and destroys all data in it
 * @param	symtable 
*/
void symtable_exit_scope(symtable_t symtable);

/**
 * @brief	checks if there is an entry for the symbol in the whole table
 * @param	symtable 
 * @param	id_token	TK_IDENTIFIER 
 * @return	true if symbol found
*/
bool symtable_has_symbol(symtable_t symtable, token_t id_token);

/**
 * @brief	checks if there is an entry for the symbol of type func in the whole table
 * @param	symtable 
 * @param	id_token	TK_IDENTIFIER 
 * @return	true if symbol found
*/
bool symtable_has_func(symtable_t symtable, token_t id_token);

/**
 * @brief	checks if there is an entry for the symbol of type var in the whole table
 * @param	symtable 
 * @param	id_token	TK_IDENTIFIER 
 * @return	true if symbol found
*/
bool symtable_has_var(symtable_t symtable, token_t id_token);

/**
 * @brief	Searches for a given symbol in the whole table
 * @param	symtable 
 * @param	id_token	TK_IDENTIFIER
 * @return	a reference to the potentionally found symbol, must be checked with symbol_valid() to determine if found
*/
symbol_ref_t symtable_find(symtable_t symtable, token_t id_token);

/**
 * @brief	Inserts a symbol into the current scope of the symbol table
 * @param	symtable 
 * @param	id_token	TK_IDENTIFIER
 * @param	symbol_type ST_VAR or ST_FUNC
 * @return	a reference to the inserted symbol, must be checked with symbol_valid() for allocation errors
*/
symbol_ref_t symtable_insert(symtable_t symtable, token_t id_token, symbol_type_t symbol_type);

/**
 * @brief	Add a parameter to the functions parameter list
 *			!!! Only call on ST_FUNC type symbols !!!
 * @param	symbol_ref 
 * @param	data_type	data type of the parameter 
 * @return	true on success, false on allocation error
*/
bool symbol_func_add_param(symbol_ref_t symbol_ref, data_type_t data_type);

/**
 * @brief	Add a return to the functions return list
 *			!!! Only call on ST_FUNC type symbols !!!
 * @param	symbol_ref 
 * @param	data_type	data type of the return value 
 * @return	true on success, false on allocation error
*/
bool symbol_func_add_return(symbol_ref_t symbol_ref, data_type_t data_type);

/**
 * @brief	Set the data for a variable symbol in the symbol table
 *			!!! Only call on ST_VAR type symbols !!!
 * @param	symbol_ref 
 * @param	sym_var_data 
*/
void symbol_var_set_data(symbol_ref_t symbol_ref, sym_var_t sym_var_data);

/**
 * @brief	Checks if the symbol refernce is valid
 * @param	symbol_ref 
 * @return	true if valid
*/
bool symbol_valid(symbol_ref_t symbol_ref);

/**
 * @brief	Checks if the symbol was declared in the current scope
 * @param	symbol_ref 
 * @return	True if the symbol is from the current scope
*/
bool symbol_current_scope(symbol_ref_t symbol_ref);

/**
 * @brief	Delete all symbols and free the allocated memory
 * @param	symtable 
 * @note	Invalidates all symbol references
*/
void symtable_free(symtable_t symtable);

#endif // !SYMTABLE_H
