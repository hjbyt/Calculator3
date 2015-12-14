/*
 * Common Utilities
 */

#ifndef COMMON_H_
#define COMMON_H_

/*
 * Macros
 */

/* TODO: doc */
/* TODO: rename to VERIFY ? */
#define CHECK(condition)    \
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
