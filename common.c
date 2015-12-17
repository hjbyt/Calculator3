/*
 * Common Utilities
 */

#include <stdio.h>
#include <stdlib.h>
#include "common.h"

void panic()
{
    printf("Unexpected error occurred!\n");
    exit(EXIT_FAILURE);
}
