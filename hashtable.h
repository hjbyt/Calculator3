
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

void hashInsert(HashTable table, char* name, double value);

double hashGetValue(HashTable table, char* name);

void hashDelete(HashTable table, char* name);

bool hashContains(HashTable table, char* name);

int hashGetSize(HashTable table);

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