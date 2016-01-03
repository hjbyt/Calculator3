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

/* TODO: doc */
void expressionToString(Tree* tree, char* buffer, unsigned int buffer_size);

/* TODO: doc */
void parseVariableInputFile(FILE* input_file, HashTable table);

/* TODO: doc */
void parseVariableAssignmentLine(char* line, HashTable table);

#endif /* PARSE_H_ */
