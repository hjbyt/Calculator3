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


#define IN
#define OUT

/* TODO: doc */
#define ARRAY_LENGTH(array) (sizeof(array)/sizeof(*array))

/* TODO: doc */
#define IS_STRING_IN_ARRAY(string, array) isStringInArray(string, array, ARRAY_LENGTH(array))

/*
 * Functions
 */

/**
 * Exit the program with EXIT_FAILURE after printing an error message.
 */
void panic() __attribute__ ((noreturn));

/* TODO: doc */
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

/* TODO: doc */
bool isName(char* string);

/* TODO: doc */
bool isLetter(char c);

#endif /* COMMON_H_ */
