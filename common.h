/*
 * Common Utilities
 */

#ifndef COMMON_H_
#define COMMON_H_

/*
 * Macros
 */

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

void panic() __attribute__ ((noreturn));

#endif /* COMMON_H_ */
