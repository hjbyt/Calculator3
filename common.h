/*
 * Common Utilities
 */

#ifndef COMMON_H_
#define COMMON_H_

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

/*
 * Functions
 */

/**
 * Exit the program with EXIT_FAILURE after printing an error message.
 */
void panic() __attribute__ ((noreturn));

#endif /* COMMON_H_ */
