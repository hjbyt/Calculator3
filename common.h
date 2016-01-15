/*
 * Common Utilities
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdbool.h>

/*
 * Macros
 */

/*
 * Verify that a condition holds (i.e. evaluates to non-zero).
 * If the condition doesn't hold, call panic.
 */
#define VERIFY(condition)    \
    do {                    \
        if (!(condition)) { \
            panic();        \
        }                   \
    } while (0)


/* Function Arguments Markers */
#define IN
#define OUT

/* Return length of a static array. */
#define ARRAY_LENGTH(array) (sizeof(array)/sizeof(*array))

/* Check if a given string is equal to a string in a static string array. */
#define IS_STRING_IN_ARRAY(string, array) isStringInArray(string, array, ARRAY_LENGTH(array))

/*
 * Functions
 */

/**
 * Exit the program with EXIT_FAILURE after printing an error message.
 */
void panic() __attribute__ ((noreturn));

/**
 * Check if the given string is equal (using strcmp) to a string in a given string array.
 *
 * @param
 *      const char* string - String to find.
 *      const char** string_array - String array to search in.
 *      unsigned int array_length - Amount of strings in the given array.
 *
 * @preconditions
 *      string != NULL
 *      const char** string_array != NULL, and every string in the array has to be valid aas well.
 *
 * @return
 *      true iff a matching string is found in the array.
 */
bool isStringInArray(const char* string, const char** string_array, unsigned int array_length);

/**
 * Check if the given string is a number made of digits only.
 * Note: this function returns false for strings such as: "-123", "+123", "1.0".
 *
 * @param
 *      const char* string - String to check.
 *
 * @preconditions
 *      string != NULL
 *
 * @return
 *      true iff string is a number made of digits only.
 */
bool isNumber(const char* string);

/**
 * Check if the given character is a digit.
 *
 * @param
 *      char c - Character to check.
 *
 * @return
 *      true iff c is a digit.
 */
bool isDigit(char c);

/**
 * Check if the given string is a valid name (i.e. made of letters only, a-z, A-Z).
 *
 * @param
 *      const char* string - String to check.
 *
 * @preconditions
 *      string != NULL
 *
 * @return
 *      true iff string is a valid name.
 */
bool isName(char* string);

/**
 * Check if the given character is a letter (a-z or A-Z).
 *
 * @param
 *      char c - Character to check.
 *
 * @return
 *      true iff c is a letter.
 */
bool isLetter(char c);

#endif /* COMMON_H_ */
