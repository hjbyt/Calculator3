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

Tree* parseLispExpression_(const char**sub_string_pointer);
void printLisp_(Tree* tree);

/*
 * Module Functions
 */

Tree* parseLispExpression(const char* string)
{
    CHECK(string != NULL);
    return parseLispExpression_(&string);
}

void printLisp(Tree* tree)
{
    CHECK(tree != NULL);
    printLisp_(tree);
    printf("\n");
}

/*
 * Internal Function Implementation
 */

Tree* parseLispExpression_(const char**sub_string_pointer)
{
    CHECK(sub_string_pointer != NULL && *sub_string_pointer != NULL);
    const char* c = *sub_string_pointer;
    CHECK(*c == '(');
    c += 1;

    const char*expression_root_start = c;
    /* Search for first parenthesis */
    c =  strpbrk(c, "()");
    CHECK(c != NULL);

    /* Copy the expression head to a new buffer, and create a tree node with it. */
    unsigned int expression_root_length = c - expression_root_start;
    char*expression_root = malloc(expression_root_length);
    CHECK(expression_root != NULL);
    memcpy(expression_root, expression_root_start, expression_root_length);
    Tree* tree = createTree(expression_root);

    while (*c == '(') {
        *sub_string_pointer = c;
        Tree* child = parseLispExpression_(sub_string_pointer);
        addChild(tree, child);
    }
    CHECK(*c == ')');

    *sub_string_pointer += 1;
    return tree;
}

void printLisp_(Tree* tree)
{
    CHECK(tree != NULL);
    printf("(%s", getValue(tree));
    for (Tree* child = firstChild(tree);
         child != NULL;
         child = nextBrother(child))
    {
        printLisp_(child);
    }
    printf(")");
}
