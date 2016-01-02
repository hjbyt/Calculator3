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

#endif /* COMMON_H_ */
