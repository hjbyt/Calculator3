
#ifndef HASHTABLE_H_
#define HASHTABLE_H_


#include "SPList.h"
#include "SPListElement.h"


typedef struct HashTable_t * HashTable;

 /**
 * Allocates a new HashTable structure
 *
 * This function creates a new empty hash table.
 * @return
 * 	NULL - If allocations failed.
 * 	A new HashTable in case of success.
 */
HashTable createHashTable();


/**
 * Inserts (or modifies) a value in the hash table
 * 
 * @param table The hash table to work on
 * @param name The key of the value to set
 * @param value The value to set for the given name
 * @return
 *   No return value. In case of an error, the panic function is called
 */
void hashInsert(HashTable table, char* name, double value);

/**
 * Get the value that was previously set for a specific name
 * 
 * @param table The hash table to work on
 * @param name The key of the value to get
 * @return
 *   The value set for the given key. In case of an error, the panic function is called
 */
double hashGetValue(HashTable table, char* name);

/**
 * Deletes a value in the hash table based on a key.
 * 
 * @param table The hash table to work on
 * @param name The key of the value to delete
 * @return
 *   In case of success nothing. In case of an error, the panic function is called
 */
void hashDelete(HashTable table, char* name);

/**
 * Says weather a given key is present in the hash table
 * 
 * @param table The hash table to check
 * @param name The key of the value to find
 * @return
 *   Weather there is a value for the given key or not. 
 *   In case of an error, the panic function is called
 */
bool hashContains(HashTable table, char* name);

/**
 * Returns the number of values in the hash table
 * 
 * @param table The hash table to check
 * @return
 *   The number of values in the hash table
 */
int hashGetSize(HashTable table);

/**
 * Says weather the given hash table is empty or not
 * 
 * @param table The hash table to check
 * @return
 *   If the table is empty or not
 */
bool hashIsEmpty(HashTable table);

/**
 * destroyHashTable: Deallocates an existing hash table. Clears all elements by using the
 * stored free function.
 *
 * @param table Target hash table to be deallocated. If table is NULL nothing will be
 * done
 */
void destroyHashTable(HashTable table);


#endif /* HASHTABLE_H_ */