/*
 * Main Module
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "tree.h"
#include "parse.h"
#include "calculate.h"
#include "common.h"

/*
 * Function Declarations
 */

bool interact();
void getLine(char* buffer, unsigned int size);

/*
 * Function Implementations
 */

/**
 * Main function.
 * Interact with the user, calculating results of lisp arithmetic expressions,
 * until a quit command is received.
 *
 * @return
 *      EXIT_SUCCESS if no error was encountered,
 *      EXIT_FAILURE otherwise.
 */
int main()
{
    bool should_continue = true;

    while (should_continue)
    {
         should_continue = interact();
    }

    return EXIT_SUCCESS;
}

/*
 * Internal Functions
 */

/**
 * Make a single user interaction:
 *      - Get an input line
 *      - Parse and calculate it, and print the result.
 *
 * @return
 *      false iff the quit command was received.
 */
bool interact()
{
    char line[MAX_LINE_LENGTH + 1];
    getLine(line, sizeof(line));

    Tree* parse_tree = parseLispExpression(line);

    if (isEndCommand(parse_tree)) {
        destroyTree(parse_tree);
        printf("Exiting...\n");
        return false;
    }

    double result = evaluateExpressionTree(parse_tree);
    destroyTree(parse_tree);

    if (isnan((float)result)) {
        printf("Invalid Result\n");
    } else {
        printf("res = %.2f\n", result);
    }

    return true;
}

/**
 * Receive a single input line from the user.

 * @param
 *      char* buffer      - Pre-allocated buffer to store line in.
 *      unsigned int size - Buffer size.
 *
 * @preconditions
 *      buffer != NULL
 */
void getLine(char* buffer, unsigned int size)
{
    VERIFY(buffer != NULL);
    char* fgets_result = fgets(buffer, size, stdin);
    VERIFY(0 == ferror(stdin));
    VERIFY(fgets_result != NULL);

    size_t last_char_index = strlen(buffer) - 1;
    if (buffer[last_char_index] == '\n') {
        buffer[last_char_index] = '\0';
    }
}
