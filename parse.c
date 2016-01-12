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


const char* DELIMITERS = " \t";
#define END_COMMAND "<>"

const char* OPERATORS[] = {"+", "-", "*", "/", "$", "="};

/*
 * Internal Function Declarations
 */

Tree* parseLispExpression_(const char** sub_string_pointer);
void printLisp_(Tree* tree);

void expressionToString_(Tree* tree, char** buffer_pointer, char* buffer_end);
void terminalExpressionToString(Tree* tree, char** buffer_pointer, char* buffer_end);
void unaryOperatorExpressionToString(Tree* tree, char** buffer_pointer, char* buffer_end);
void binaryOperatorExpressionToString(Tree* tree, char** buffer_pointer, char* buffer_end);
void functionExpressionToString(Tree* tree, char** buffer_pointer, char* buffer_end);

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

bool isAssignmentCommnd(Tree* tree)
{
    VERIFY(tree != NULL);
    return (strcmp(getValue(tree), "=") == 0);
}

bool isEndCommand(Tree* tree)
{
    VERIFY(tree != NULL);
    return (!hasChildren(tree) && strcmp(getValue(tree), END_COMMAND) == 0);
}

void expressionToString(Tree* tree, char* buffer, unsigned int buffer_size)
{
    VERIFY(tree != NULL);
    VERIFY(buffer != NULL);

    /* TODO: explain the memset */
    memset(buffer, 0, buffer_size);
    expressionToString_(tree, &buffer, buffer + buffer_size);
}

void parseVariableInputFile(FILE* input_file, HashTable table)
{
    VERIFY(input_file != NULL);
    VERIFY(table != NULL);

    while (!feof(input_file))
    {
        /* Get line */
        char line[MAX_LINE_LENGTH + 1];
        char* fgets_retval = fgets(line, sizeof(line), input_file);
        if (fgets_retval == NULL) {
            // end of file
            break;
        }
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
void expressionToString_(Tree* tree, char** buffer_pointer, char* buffer_end)
{
    VERIFY(tree != NULL);
    VERIFY(buffer_pointer != NULL && *buffer_pointer != NULL);

    unsigned int children_count = childrenCount(tree);
    if (children_count == 0) {
        terminalExpressionToString(tree, buffer_pointer, buffer_end);
        return;
    }

    char* operation = getValue(tree);

    if (IS_STRING_IN_ARRAY(operation, OPERATORS)) {
        if (children_count == 1) {
            unaryOperatorExpressionToString(tree, buffer_pointer, buffer_end);
        } else if (children_count == 2) {
            binaryOperatorExpressionToString(tree, buffer_pointer, buffer_end);
        } else {
            panic();
        }
    } else {
        /* Assume it's a function */
        functionExpressionToString(tree, buffer_pointer, buffer_end);
    }
}

void appendToBuffer(char* appendage, char** buffer_pointer, char* buffer_end)
{
    VERIFY(buffer_pointer != NULL);
    char* buffer = *buffer_pointer;
    VERIFY(buffer != NULL);

    unsigned int buffer_size = buffer_end - buffer;
    size_t appendage_length = strlen(appendage);
    VERIFY(buffer_size > appendage_length);

    memcpy(buffer, appendage, appendage_length);
    buffer += appendage_length;
    *buffer_pointer = buffer;
}

/* TODO: doc */
void terminalExpressionToString(Tree* tree, char** buffer_pointer, char* buffer_end)
{
    VERIFY(tree != NULL);
    VERIFY(buffer_pointer != NULL);
    VERIFY(childrenCount(tree) == 0);

    if (isRoot(tree)) {
        appendToBuffer("(", buffer_pointer, buffer_end);
    }

    char* terminal = getValue(tree);
    appendToBuffer(terminal, buffer_pointer, buffer_end);

    if (isRoot(tree)) {
        appendToBuffer(")", buffer_pointer, buffer_end);
    }
}

/* TODO: doc */
void unaryOperatorExpressionToString(Tree* tree, char** buffer_pointer, char* buffer_end)
{
    VERIFY(tree != NULL);
    VERIFY(buffer_pointer != NULL);
    VERIFY(childrenCount(tree) == 1);

    appendToBuffer("(", buffer_pointer, buffer_end);
    char* operator = getValue(tree);
    appendToBuffer(operator, buffer_pointer, buffer_end);
    expressionToString_(firstChild(tree), buffer_pointer, buffer_end);
    appendToBuffer(")", buffer_pointer, buffer_end);
}

/* TODO: doc */
void binaryOperatorExpressionToString(Tree* tree, char** buffer_pointer, char* buffer_end)
{
    VERIFY(tree != NULL);
    VERIFY(buffer_pointer != NULL);
    VERIFY(childrenCount(tree) == 2);

    appendToBuffer("(", buffer_pointer, buffer_end);
    expressionToString_(firstChild(tree), buffer_pointer, buffer_end);
    char* operator = getValue(tree);
    appendToBuffer(operator, buffer_pointer, buffer_end);
    expressionToString_(lastChild(tree), buffer_pointer, buffer_end);
    appendToBuffer(")", buffer_pointer, buffer_end);
}

/* TODO: doc */
void functionExpressionToString(Tree* tree, char** buffer_pointer, char* buffer_end)
{
    VERIFY(tree != NULL);
    VERIFY(buffer_pointer != NULL);
    VERIFY(childrenCount(tree) >= 1);

    appendToBuffer("(", buffer_pointer, buffer_end);

    char* function = getValue(tree);
    appendToBuffer(function, buffer_pointer, buffer_end);

    appendToBuffer("(", buffer_pointer, buffer_end);

    Tree* child = firstChild(tree);
    expressionToString_(child, buffer_pointer, buffer_end);
    for (child = nextBrother(child); child != NULL; child = nextBrother(child))
    {
        appendToBuffer(",", buffer_pointer, buffer_end);
        expressionToString_(child, buffer_pointer, buffer_end);
    }

    appendToBuffer("))", buffer_pointer, buffer_end);
}
