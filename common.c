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

bool isNumber(const char* string)
{
    VERIFY(string != NULL);
    const char* c = string;

    if (*c == '\0') {
        return false;
    }

    do {
        if (!isDigit(*c)) {
            return false;
        }
        c++;
    } while (*c != '\0');

    return true;
}

bool isDigit(char c)
{
    return (c >= '0' && c <= '9');
}

bool isName(char* string)
{
    VERIFY(string != NULL);
    for (char *c = string; *c != '\0'; c++)
    {
        if (!isLetter(*c)) {
            return false;
        }
    }
    return true;
}

bool isLetter(char c)
{
    return (c >= 'A') && (c <= 'z');
}
