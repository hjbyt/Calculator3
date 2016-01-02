#ifndef LISTELEMENT_H_
#define LISTELEMENT_H_

#include <stdbool.h>
/**
 * List Element
 *
 * Implements a list element which contains a string value and a double value.
 * An element contains:
 * 		String str
 * 		double Value
 * Two elements e1 and e2 are said to be equal iff:
 * 		(e1.str == e2.str) AND (e1.Value == e2.Value)
 *
 * The following functions are available
 *	createElement   - Creates a new element with a copy of a specific string and double value
 *	copyElement     - Creates a new copy of the target element
 *	destroyElement  - Free all memory allocations associated with an element
 *	areElementsEqual- Checks if two elements are equal
 *	setElementStr   - Sets a new string value to the target element. The target element will contain
 *					  a copy of the new string.
 *  getElementStr   - Gets a copy of the string of the target element
 *  setElementValue - Sets a new value to the target element.
 *  getElementValue - Gets a copy of the value of the target element
 */

/** Type used for returning error codes from element functions */
typedef enum SP_LIST_ELEMENT_RESULT_t{
	SP_ELEMENT_SUCCESS,
	SP_ELEMENT_INVALID_ARGUMENT,
	SP_ELEMENT_OUT_OF_MEMORY

} SPElementResult;

/** Type used represent an element in the list **/
typedef struct SPListElement_t * SPListElement;


/**
 * Creates a new element with the specific string and value.
 *
 * @param str The string value of the element (Must be != NULL)
 * @param value The value of the element
 * @return
 * NULL in case str==NULL or memory allocation fails.
 * A new element with a COPY the string and a COPY of the value otherwise.
 */
SPListElement createElement(char* str, double value);

/** Creates a copy of target element.
 *
 * The new copy will contain the same string and value as in the target element.
 *
 * @param data The target element to copy
 * @return
 * NULL if a NULL was sent or a memory allocation failed.
 * An element containing the same string and value as the target element.
 **/
SPListElement copyElement(SPListElement data);

/* Destroys an element.
 * All memory allocation associated with the element will be freed
 *
 * @param data the target element which will be free'd
 */
void destroyElement(SPListElement data);

/** Compares two elements and check if they are equal
 *  Two elements data1 and data2 are said to be equal iff:
 * 		(data1.str == data2.str) AND (data1.Value == data2.Value)
 * 		That is if they both contain the same string and value
 *  @param data1 The target element to be compared to
 *  @param data2 The target element to be compared with
 *  @return
 *  false in case data1==NULL or data2==NULL or data1 and data2 are not equal
 *  true otherwise
 */
bool areElementsEqual(SPListElement data1,SPListElement data2);

/** Checks if the element's string equals to the given string
 *  Element data1 contains string str iff.
 * 		data1.str == str
 *  @param data1 The target element to be compared to
 *  @param str The string to be compared with
 *  @return
 *  false in case data1==NULL or str==NULL or data1.str != str
 *  true otherwise
 */
bool isElementStrEquals(SPListElement data1,char* str);

/** Checks if the element's value equals to the given value
 *  Element data1 contains the value 'value' iff.
 * 		data1.value == value
 *  @param data1 The target element to be compared to
 *  @param value The value to be compared with
 *  @return
 *  false in case data1==NULL or data1.value != value
 *  true otherwise
 */
bool isElementValueEquals(SPListElement data1,double value);

/** A setter of the string value of the target element.
 *  A copy of the string will be stored in the target element.
 *
 *  @param data the target element
 *  @param str  The new string value. The value will be copied to the target element
 *  @return
 *  SP_ELEMENT_INVALID_ARGUMENT in case data==NULL || str==NULL
 *  SP_ELEMENT_OUT_OF_MEMORY in case memory allocation fails
 *  SP_ELEMENT_SUCCESS otherwise
 */
SPElementResult setElementStr(SPListElement data, char* str);

/* A getter of the string element of the target element.
 *
 * @param data The target element
 * @return
 * NULL in case data==NULL or memory allocation fails
 * A copy of the element's string otherwise
 */
char* getElementStr(SPListElement data);

/* A setter of the value of the target element
 *
 * @param newValue The new value of the target element
 * @return
 * SP_ELEMENT_INVALID_ARGUMENT in case data==NULL
 * SP_ELEMENT_OUT_OF_MEMORY in case of memory allocation failure
 * SP_ELEMENT_SUCCESS otherwise
 */
SPElementResult setELementValue(SPListElement data, double newValue);

/* A getter of the value of the target element
 * @param data the target element
 * @return
 * NULL in case data==NULL or memory allocation fails.
 * A copy of the element value otherwise.
 */
double* getElementValue(SPListElement data);

#endif /* LISTELEMENT_H_ */
