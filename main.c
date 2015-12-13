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
 * Constants
 */

#define MAX_LINE_LENGTH 1024
#define END_COMMAND "<>"

/*
 * Function Declarations
 */

bool interact();
void getLine(char* buffer, int size);
bool isEndCommand(Tree* tree);

/*
 * Function Implementations
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

void getLine(char* buffer, int size)
{
    char* fgets_result = fgets(buffer, size, stdin);
    CHECK(0 == ferror(stdin));
    CHECK(fgets_result != NULL);

    size_t last_char_index = strlen(buffer) - 1;
    if (buffer[last_char_index] == '\n') {
        buffer[last_char_index] = '\0';
    }
}

bool isEndCommand(Tree* tree)
{
    CHECK(tree != NULL);
    return (!hasChildren(tree) && strcmp(getValue(tree), END_COMMAND) == 0);
}
