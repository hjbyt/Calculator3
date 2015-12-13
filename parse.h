/*
 * Lisp Parsing Module
 */

#ifndef PARSE_H_
#define PARSE_H_

#include "tree.h"

Tree* parseLispExpression(const char* string);

/* Used for testing and debugging */
void printLisp(Tree* tree);

#endif /* PARSE_H_ */
