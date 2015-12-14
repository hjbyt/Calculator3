/*
 * Lisp Parsing Module
 */

#ifndef PARSE_H_
#define PARSE_H_

#include "tree.h"

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
 *      - string != NULL
 *      - There is enough memory for the allocated tree.
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

#endif /* PARSE_H_ */
