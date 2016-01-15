/*
 * Lisp Parsing Module
 */

#ifndef PARSE_H_
#define PARSE_H_

#include "tree.h"
#include "hashtable.h"

/*
 * Constants
 */

#define MAX_LINE_LENGTH 1024

/*
 * Functions
 */

/**
 * Parse a Lisp expression of the form: (str<Child_1><Child2>...<Child_n>)
 * where <Child_x> are sub-expressions,
 * and return a parse tree.
 * The created tree has to be destroyed by destroyTree.
 *
 * @param
 * 		const char* string - string to parse.
 *
 * @preconditions
 *      string != NULL
 *
 * @return
 *		Parse tree.
 */
Tree* parseLispExpression(const char* string);

/**
 * Print the given tree as a lisp expression of the same form as in parseLispExpression.
 * Used for testing and debugging.
 * Note: the printed expression is followed by a new-line.
 *
 * @param
 * 		Tree* tree - Tree to print.
 *
 * @preconditions
 *      tree != NULL
 */
void printLisp(Tree* tree);

/**
 * Check if the given expression tree represents an assignment expression.
 *
 * @param
 *      Tree* tree - Expression tree to examine.
 *
 * @preconditions
 *      tree != NULL
 *
 * @return
 *      true iff the Expression tree represents an assignment expression.
 */
bool isAssignmentExpression(Tree* tree);

/**
 * Check if the given expression tree represents the quit command.
 *
 * @param
 *      Tree* tree - Expression tree to examine.
 *
 * @preconditions
 *      tree != NULL
 *
 * @return
 *      true iff the Expression tree represents a quit command.
 */
bool isEndCommand(Tree* tree);

/**
 * Convert and expression tree to an equivalent expression string (infix notation, not lisp).
 *
 * @param
 *      Tree* tree - Expression tree to convert.
 *      char* buffer - buffer which the resulting string is saved into.
 *      unsigned int buffer_size - size of the buffer.
 *
 * @preconditions
 *      - tree != NULL, buffer != NULL
 *      - The buffer has to be large enough for the resulting string.
 *        preferably buffer_size should be >= MAX_LINE_LENGTH + 1.
 */
void expressionToString(Tree* tree, char* buffer, unsigned int buffer_size);

/**
 * Parse variable initialization file.
 *
 * @param
 *      FILE* input_file - File to parse.
 *      HashTable table - Table into which parsed variables are inserted.
 *
 * @preconditions
 *      input_file != NULL, table != NULL
 */
void parseVariableInputFile(FILE* input_file, HashTable table);

/* Note: this function is in the interface for testing purposes. */
/**
 * Parse a single variable assignment line (as read from a variable initialization file).
 *
 * @param
 *      char* line - line to parse.
 *      HashTable table - Table into which parsed variables are inserted.
 *
 * @preconditions
 *      input_file != NULL, table != NULL
 */
void parseVariableAssignmentLine(char* line, HashTable table);

#endif /* PARSE_H_ */
