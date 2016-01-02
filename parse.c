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
 * Constants
 */

#define MAX_LINE_LENGTH 1024
const char* DELIMITERS = " \t";

/*
 * Internal Function Declarations
 */

Tree* parseLispExpression_(const char** sub_string_pointer);
void printLisp_(Tree* tree);
bool isName(char* string);
bool isLetter(char c);

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

void parseVariableInputFile(FILE* input_file, HashTable table)
{
    VERIFY(input_file != NULL);
    VERIFY(table != NULL);

    while (!feof(input_file))
    {
        /* Get line */
        char line[MAX_LINE_LENGTH + 1];
        fgets(line, sizeof(line), input_file);
        VERIFY(!ferror(input_file));

        parseVariableAssignmentLine(line, table);
    }
}

void parseVariableAssignmentLine(char* line, HashTable table)
{
    VERIFY(line != NULL);
    VERIFY(table != NULL);

    /* Parse name */
    char* token = strtok(line, DELIMITERS);
    VERIFY(token != NULL);
    VERIFY(isName(token));
    char* name = token;

    /* Parse '=' */
    token = strtok(NULL, DELIMITERS);
    VERIFY(token != NULL);
    VERIFY(strcmp(token, "=") == 0);

    /* Parse number */
    token = strtok(NULL, DELIMITERS);
    VERIFY(token != NULL);
    errno = 0;
    long int value = strtol(token, &token, 10);
    VERIFY(errno == 0);

    /* Make sure the number token was completely processed */
    char current_char = *token;
    VERIFY(   current_char == ' '
           || current_char == '\t'
           || current_char == '\n'
           || current_char == '\0');

    /* Make sure there are no remaining tokens */
    token = strtok(NULL, DELIMITERS);
    VERIFY(token == NULL);

    hashInsert(table, name, (double)value);
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

/* TODO: doc */
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

/* TODO: doc */
bool isLetter(char c)
{
    return (c >= 'A') && (c <= 'z');
}
