#ifndef SPLIST_H_
#define SPLIST_H_

#include <stdbool.h>
#include "SPListElement.h"
/**
 * List Container
 *
 * Implements a list container type.
 * The list has an internal iterator for external use. For all functions
 * where the state of the iterator after calling that function is not stated,
 * the state of the iterator is undefined. That is you cannot assume anything about it.
 *
 * The following functions are available:
 *
 *   listCreate               - Creates a new empty list
 *   listDestroy              - Deletes an existing list and frees all resources
 *   listCopy                 - Copies an existing list
 *   listSize                 - Returns the size of a given list
 *   listFirst                - Sets the internal iterator to the first element
 *                              in the list, and returns it.
 *   listNext                 - Advances the internal iterator to the next
 *                              element and returns it.
 *   listInsertFirst          - Inserts an element in the beginning of the list
 *   listInsertLast           - Inserts an element in the end of the list
 *   listInsertBeforeCurrent  - Inserts an element right before the place of
 *                              internal iterator
 *   listInsertAfterCurrent   - Inserts an element right after the place of the
 *                              internal iterator
 *   listRemoveCurrent        - Removes the element pointed by the internal
 *                              iterator
 *   listGetCurrent           - Return the current element (pointed by the
 *                              internal iterator)
 *   listGetFirst             - Sets the internal iterator (also called current
 *                              element) to be the first element in the list and
 *                              return it.
 *   listGetNext              - Advances the list's iterator to the next element
 *                              and return it
 *   listClear		      	  - Clears all the data from the list
 */

/** Type for defining the list */
typedef struct List_t *SPList;

/** Type used for returning error codes from list functions */
typedef enum ListResult_t {
	SP_LIST_SUCCESS,
	SP_LIST_NULL_ARGUMENT,
	SP_LIST_OUT_OF_MEMORY,
	SP_LIST_INVALID_CURRENT,
} ListResult;

/**
 * Allocates a new List.
 *
 * This function creates a new empty list.
 * @return
 * 	NULL - If allocations failed.
 * 	A new List in case of success.
 */
SPList listCreate();

/**
 * Creates a copy of target list.
 *
 * The new copy will contain all the elements from the source list in the same
 * order. The internal iterator for both the new copy and the target list will not be
 * defined afterwards.
 *
 * @param list The target list to copy
 * @return
 * NULL if a NULL was sent or a memory allocation failed.
 * A List containing the same elements with same order as list otherwise.
 */
SPList listCopy(SPList list);

/**
 * Returns the number of elements in a list. the iterator state will not change.
 *
 * @param list The target list which size is requested.
 * @return
 * -1 if a NULL pointer was sent.
 * Otherwise the number of elements in the list.
 */
int listGetSize(SPList list);

/**
 * Sets the internal iterator to the first element and retrieves it.
 *
 * The list has an internal iterator (also called current element) to allow
 * iteration over the list's elements. This function sets the iterator to point
 * to the first element in the list and return it.
 * Use this to start iterating over the list, searching from the beginning of
 * the list and/or get the first element in the list.
 * (To continue iteration use listGetNext)
 * @code
 * void f(SPList list) {
 *   ListElement first = listGetFirst(list);
 *   printf("The first element is at address %x\n", first);
 * }
 * @endcode
 *
 * @param list The list for which to set the iterator and return the first
 * element.
 * @return
 * NULL is a NULL pointer was sent or the list is empty.
 * The first element of the list otherwise
 */
SPListElement listGetFirst(SPList list);

/**
 * Advances the list's iterator to the next element and return it. In case
 * the return value is NULL, the state of the iterator will not be defined, otherwise
 * it will point to the next element in the list.
 *
 * @param list The list for which to advance the iterator
 * @return
 * NULL if reached the end of the list, the iterator is at an invalid state or
 * a NULL sent as argument
 * The next element on the list in case of success
 */
SPListElement listGetNext(SPList list);

/**
 * Returns the current element (pointed by the iterator)
 *
 * @param list The list for which to get the iterator
 * @return
 * NULL if the iterator is at an invalid state or a NULL sent as argument
 * The current element on the list in case of success
 */
SPListElement listGetCurrent(SPList list);

/**
 * Adds a new element to the list, the new element will be the first element. The state
 * of the iterator will not be changed
 *
 * @param list The list for which to add an element in its start
 * @param element The element to insert. A copy of the element will be
 * inserted
 * @return
 * SP_LIST_NULL_ARGUMENT if a NULL was sent as list
 * SP_LIST_OUT_OF_MEMORY if an allocation failed
 * SP_LIST_SUCCESS the element has been inserted successfully
 */
ListResult listInsertFirst(SPList list, SPListElement element);

/**
 * Adds a new element to the list, the new element will be the last element. The state
 * of the iterator will not be changed.
 *
 * @param list The list for which to add an element in its end
 * @param element The element to insert. A copy of the element will be
 * inserted
 * @return
 * SP_LIST_NULL_ARGUMENT if a NULL was sent as list
 * SP_LIST_OUT_OF_MEMORY if an allocation failed
 * SP_LIST_SUCCESS the element has been inserted successfully
 */
ListResult listInsertLast(SPList list, SPListElement element);

/**
 * Adds a new element to the list, the new element will be placed right before
 * the current element (As pointed by the inner iterator of the list). The state
 * of the iterator will not be changed
 *
 * @param list The list for which to add an element before its current element
 * @param element The element to insert. A copy of the element will be
 * inserted
 * @return
 * SP_LIST_NULL_ARGUMENT if a NULL was sent as list or element
 * SP_LIST_INVALID_CURRENT if the list's iterator is in an invalid state (Does
 * not point to a legal element in the list)
 * SP_LIST_OUT_OF_MEMORY if an allocation failed
 * SP_LIST_SUCCESS the element has been inserted successfully
 */
ListResult listInsertBeforeCurrent(SPList list, SPListElement element);

/**
 * Adds a new element to the list, the new element will be place right after
 * the current element (As pointed by the inner iterator be of the list). The state
 * of the iterator will not be changed.
 *
 * @param list The list for which to add an element after its current element
 * @param element The element to insert. A copy of the element will be
 * inserted
 * @return
 * SP_LIST_NULL_ARGUMENT if a NULL was sent as list
 * SP_LIST_INVALID_CURRENT if the list's iterator is in an invalid state (Does
 * not point to a legal element in the list)
 * SP_LIST_OUT_OF_MEMORY if an allocation failed
 * SP_LIST_SUCCESS the element has been inserted successfully
 */
ListResult listInsertAfterCurrent(SPList list, SPListElement element);

/**
 * Removes the currently pointed element of the list using the stored freeing
 * function. The state of the current element will not be defined afterwards.
 *
 * @param list The list for which the current element will be removed
 * @return
 * SP_LIST_NULL_ARGUMENT if list is NULL
 * SP_LIST_INVALID_CURRENT if the current pointer of the list is in invalid state
 * SP_LIST_SUCCESS the current element was removed successfully
 */
ListResult listRemoveCurrent(SPList list);

/**
 * Removes all elements from target list. The state of the current element will not be defined afterwards.
 *
 * The elements are deallocated using the stored freeing function
 * @param list Target list to remove all element from
 * @return
 * SP_LIST_NULL_ARGUMENT - if a NULL pointer was sent.
 * SP_LIST_SUCCESS - Otherwise.
 */
ListResult listClear(SPList list);

/**
 * listDestroy: Deallocates an existing list. Clears all elements by using the
 * stored free function.
 *
 * @param list Target list to be deallocated. If list is NULL nothing will be
 * done
 */
void listDestroy(SPList list);

/**
 * Macro for iterating over a list.
 *
 * Declares a new variable to hold each element of the list.
 * Use this macro for iterating through the list conveniently.
 * Note that this mcaro modifies the internal iterator.
 * For example, the following code will go through a list of strings and print
 * them to the standard output:
 * @code
 * void printListStrings(SPList list) {
 *   LIST_FOREACH(SPListElement, e, list) {
 *     char* eStr = getElementStr(e)
 *     assert(eStr != NULL);
 *     printf("%s\\n", eStr);
 *     free(eStr);
 *   }
 * }
 * @endcode
 *
 * @param type The type of the elements in the list
 * @param iterator The name of the variable to hold the next list element
 * @param list the list to iterate over
 */
#define LIST_FOREACH(type,iterator,list) \
	for(type iterator = listGetFirst(list) ; \
		iterator ;\
		iterator = listGetNext(list))


#endif /* SPLIST_H_ */
