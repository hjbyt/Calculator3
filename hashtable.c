
#include <stdlib.h>
#include "hashtable.h"
#include "common.h"
#include "SPList.h"
#include "SPListElement.h"

#define NUMBER_OF_ENTRIES (100)
#define FIRST_PRIME (571)
#define COEFFICIENT_PRIME (31)

struct HashTable_t {
    SPList buckets[NUMBER_OF_ENTRIES];
};

typedef enum HashTableResult_e {
    HT_SUCCESS = 0,
    HT_OUT_OF_MEMORY,
    HT_ELEMENT_ALREADY_EXSITS,
    HT_VALUE_DOES_NOT_EXIST,
} HashTableResult;

typedef enum LookupOperation_e {
    CREATE,
    DELETE,
    GET,
    SET,
} LookupOperation;

HashTableResult lookupElementByName(HashTable table, char* name, LookupOperation operation, OUT SPListElement* element);
int hash(const char* str);


HashTable createHashTable() {
    struct HashTable_t* table = malloc(sizeof(*table));
    VERIFY(NULL != table);
    
    for (int i = 0; i < NUMBER_OF_ENTRIES; i++) {
        table->buckets[i] = listCreate();
        VERIFY(NULL != table->buckets[i]);
    }
    
    return table;
}

void insertValue(HashTable table, char* name, double value) {
    SPListElement newElement;
    HashTableResult result = lookupElementByName(table, name, CREATE, &newElement);
    VERIFY(HT_SUCCESS == result);
    
    setELementValue(newElement, value);
}

double getValueByName(HashTable table, char* name) {
    SPListElement foundElement;
    HashTableResult result = lookupElementByName(table, name, GET, &foundElement);
    VERIFY(HT_SUCCESS == result);
    
    double* foundValue = getElementValue(foundElement);
    VERIFY(NULL != foundValue);
    return *foundValue;
}

void setValue(HashTable table, char* name, double value) {
    SPListElement foundElement;
    HashTableResult result = lookupElementByName(table, name, SET, &foundElement);
    VERIFY(HT_SUCCESS == result);
    
    SPElementResult elementResult = setELementValue(foundElement, value);
    VERIFY(SP_ELEMENT_SUCCESS == elementResult);
}

void deleteByName(HashTable table, char* name) {
    SPListElement foundElement;
    HashTableResult result = lookupElementByName(table, name, DELETE, &foundElement);
    VERIFY(HT_SUCCESS == result);
}

bool nameExists(HashTable table, char* name) {
    SPListElement foundElement;
    HashTableResult result = lookupElementByName(table, name, GET, &foundElement);
    return (HT_SUCCESS == result);
}

void destroyHashTable(HashTable table) {
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


int hash(const char* str) {
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

HashTableResult lookupElementByName(HashTable table, char* name, LookupOperation operation, OUT SPListElement* element) {
    int nameHash = hash(name);
    SPList bucket = table->buckets[nameHash];
    *element = NULL;
    
    LIST_FOREACH(SPListElement, i, bucket) {
        if (isElementStrEquals(i, name)) {
            if (CREATE == operation) {
                return HT_ELEMENT_ALREADY_EXSITS;
            }
            else if (DELETE == operation) {
                listRemoveCurrent(bucket);
                return HT_SUCCESS;
            }
            else if (GET == operation || SET == operation) {
                *element = listGetCurrent(bucket);
                return HT_SUCCESS;   
            }
        }
    }
    
    if (CREATE != operation) {
        return HT_VALUE_DOES_NOT_EXIST;
    }
    
    *element = createElement(name, 0);
    VERIFY(NULL != element);
    
    ListResult listResult = listInsertFirst(bucket, *element);
    VERIFY(SP_LIST_SUCCESS == listResult);
    listGetFirst(bucket);
    *element = listGetCurrent(bucket);
    
    return HT_SUCCESS;
}