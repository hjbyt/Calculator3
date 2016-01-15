/*
 * Hash Table Module
 */

#include <stdlib.h>
#include "hashtable.h"
#include "common.h"

/*
 * Constants
 */

#define NUMBER_OF_ENTRIES (100)
#define FIRST_PRIME (571)
#define COEFFICIENT_PRIME (31)

/*
 * Types
 */

struct HashTable_t {
    SPList buckets[NUMBER_OF_ENTRIES];
    int numberOfValues;
};

typedef enum LookupOperation_e {
    INSERT,
    DELETE,
    GET,
} LookupOperation;

/*
 * Internal Functions
 */

/**
 * lookupElementByName: Finds an element in the table by it's name, creates a new one, or deletes
 * an exiting one (based on the action).
 *
 * @param table Target hash table to work on
 * @param name The name of the element to work on
 * @param operation Says wheather to create, get or delete the element
 * @param element Out parameter that receives the element for the GET operation
 * @return Says wheather the operation succeeded or not
 */
bool lookupElementByName(HashTable table,
                         char* name,
                         LookupOperation operation,
                         OUT SPListElement* element);
/**
 * lookupElementByName: Hashes the given string (for the hash table)
 *
 * @param str The string to hash (untill the '\0' character is found)
 * @return The resulting hash value
 */
int hash(const char* str);

/*
 * Functions
 */

HashTable createHashTable()
{
    struct HashTable_t* table = malloc(sizeof(*table));
    VERIFY(NULL != table);
    
    for (int i = 0; i < NUMBER_OF_ENTRIES; i++) {
        table->buckets[i] = listCreate();
        VERIFY(NULL != table->buckets[i]);
    }
    table->numberOfValues = 0;
    
    return table;
}

void hashInsert(HashTable table, char* name, double value)
{
    SPListElement newElement;
    bool found = lookupElementByName(table, name, INSERT, &newElement);
    VERIFY(found);
    
    setELementValue(newElement, value);
}

double hashGetValue(HashTable table, char* name)
{
    SPListElement foundElement;
    bool found = lookupElementByName(table, name, GET, &foundElement);
    VERIFY(found);
    
    double* foundValue = getElementValue(foundElement);
    VERIFY(NULL != foundValue);
    return *foundValue;
}

void hashDelete(HashTable table, char* name)
{
    SPListElement foundElement;
    bool found = lookupElementByName(table, name, DELETE, &foundElement);
    VERIFY(found);
}

bool hashContains(HashTable table, char* name)
{
    SPListElement foundElement;
    bool found = lookupElementByName(table, name, GET, &foundElement);
    return (found);
}

int hashGetSize(HashTable table)
{
    VERIFY(NULL != table);
    return table->numberOfValues;
}

bool hashIsEmpty(HashTable table)
{
    VERIFY(NULL != table);
    return (table->numberOfValues == 0);
}

void destroyHashTable(HashTable table)
{
    if (NULL == table) {
        return;
    }
    
    for (int i = 0; i < NUMBER_OF_ENTRIES; i++) {
        if (NULL != table->buckets[i]) {
            listDestroy(table->buckets[i]);
        }
    }
    
    free(table);
}


int hash(const char* str)
{
    if (NULL == str) {
        return -1;
    }
    
    int hashValue = FIRST_PRIME;
    while ('\0' != *str) {
        hashValue = (COEFFICIENT_PRIME * hashValue) + (int)(*str);
        hashValue %= NUMBER_OF_ENTRIES;
        str++;
    }
    
    return hashValue;
}

bool lookupElementByName(HashTable table,
                         char* name,
                         LookupOperation operation,
                         OUT SPListElement* element)
{
    VERIFY(NULL != table);
    
    int nameHash = hash(name);
    SPList bucket = table->buckets[nameHash];
    *element = NULL;
    
    LIST_FOREACH(SPListElement, i, bucket) {
        if (isElementStrEquals(i, name)) {
            if (DELETE == operation) {
                table->numberOfValues--;
                listRemoveCurrent(bucket);
                return true;
            } else if (INSERT == operation || GET == operation) {
                *element = listGetCurrent(bucket);
                return true;
            }
        }
    }
    
    if (INSERT != operation) {
        return false;
    }
    
    *element = createElement(name, 0);
    VERIFY(NULL != element);
    
    ListResult listResult = listInsertFirst(bucket, *element);
    VERIFY(SP_LIST_SUCCESS == listResult);
    listGetFirst(bucket);
    *element = listGetCurrent(bucket);
    table->numberOfValues++;
    
    return true;
}