/*
 * Common Utilities
 */

#include <stdio.h>
#include <stdlib.h>

void panic() __attribute__ ((noreturn))
{
    printf("Unexpected error occurred!\n");
    exit(EXIT_FAILURE);
}
