/*
 * Lisp Parsing Module
 */

#ifndef PARSE_H_
#define PARSE_H_

#include "tree.h"
#include "hashtable.h"

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

/* Used for testing and debugging */

/**
 * Print the given tree as a lisp expression of the same form as in parseLispExpression.
 * Note: the printed expression is followed by a new-line.
 *
 * @param
 * 		Tree* tree - Tree to print.
 *
 * @preconditions
 *      tree != NULL
 */
void printLisp(Tree* tree);

/* TODO: doc */
void parseVariableInputFile(FILE* input_file, HashTable table);

/* TODO: doc */
void parseVariableAssignmentLine(char* line, HashTable table);

#endif /* PARSE_H_ */
