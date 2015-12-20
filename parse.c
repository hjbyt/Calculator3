/*
 * Lisp Parsing Module
 */

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "parse.h"
#include "common.h"

/*
 * Internal Function Declarations
 */

Tree* parseLispExpression_(const char** sub_string_pointer);
void printLisp_(Tree* tree);

/*
 * Module Functions
 */

Tree* parseLispExpression(const char* string)
{
    VERIFY(string != NULL);
    Tree* tree = parseLispExpression_(&string);
    /* Check that the entire string was processed,
     * i.e. the parsing ended successfully at the null-terminator */
    VERIFY(*string == '\0');
    return tree;
}

void printLisp(Tree* tree)
{
    VERIFY(tree != NULL);
    printLisp_(tree);
    printf("\n");
}

/*
 * Internal Functions
 */

/**
 * Sub-routine of parseLispExpression.
 * This function gets a sub-string pointer by reference,
 * and updates it as it parses the sub-string.
 *
 * @param
 * 		const char** sub_string_pointer - Sub-string to parse.
 *                                        This value is updated as the parsing is performed.
 *
 * @preconditions
 *      sub_string_pointer != NULL && *sub_string_pointer != NULL
 *
 * @post-conditions
 *      sub_string_pointer points to the end of the parsed sub-string,
 *      i.e. after the closing ')' that matches the first opening '('.
 *
 * @return
 *		Parse tree.
 */
Tree* parseLispExpression_(const char** sub_string_pointer)
{
    VERIFY(sub_string_pointer != NULL && *sub_string_pointer != NULL);

    const char* c = *sub_string_pointer;
    VERIFY(*c == '(');
    c += 1;

    const char* expression_root_start = c;
    /* Search for first parenthesis */
    c =  strpbrk(c, "()");
    VERIFY(c != NULL);

    /* Copy the expression root to a new buffer, and create a tree node with it. */
    unsigned int expression_root_length = c - expression_root_start;
    char* expression_root = malloc(expression_root_length + 1);
    VERIFY(expression_root != NULL);
    memcpy(expression_root, expression_root_start, expression_root_length);
    expression_root[expression_root_length] = '\0';
    Tree* tree = createTree(expression_root);

    while (*c == '(') {
        *sub_string_pointer = c;
        Tree* child = parseLispExpression_(sub_string_pointer);
        addChild(tree, child);
        c = *sub_string_pointer;
    }

    VERIFY(*c == ')');
    *sub_string_pointer = c + 1;

    return tree;
}

/**
 * Print the given tree as a lisp expression of the same form as in parseLispExpression.
 *
 * @param
 * 		Tree* tree - Tree to print.
 *
 * @preconditions
 *      tree != NULL
 */
void printLisp_(Tree* tree)
{
    VERIFY(tree != NULL);
    printf("(%s", getValue(tree));
    for (Tree* child = firstChild(tree);
         child != NULL;
         child = nextBrother(child))
    {
        printLisp_(child);
    }
    printf(")");
}
