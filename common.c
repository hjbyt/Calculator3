/*
 * Common Utilities
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

void panic()
{
    printf("Unexpected error occurred!\n");
    exit(EXIT_FAILURE);
}

bool isStringInArray(const char* string, const char** string_array, unsigned int array_length)
{
    for (int i = 0; i < array_length; ++i)
    {
        const char* other_string = string_array[i];
        if (strcmp(string, other_string) == 0) {
            return true;
        }
    }
    return false;
}
