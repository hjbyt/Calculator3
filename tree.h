/*
 * Tree Module
 */

#ifndef TREE_H_
#define TREE_H_

#include <stdbool.h>

/*
 * Types
 */

typedef struct Tree Tree;
typedef struct ChildrenIterator ChildrenIterator;

/*
 * Functions
 */

Tree* createTree(char* value);
void destroyTree(Tree* tree);
char* getValue(Tree* tree);
unsigned int childrenCount(Tree* tree);
bool hasChildren(Tree* tree);
Tree* firstChild(Tree* tree);
Tree* lastChild(Tree* tree);
Tree* nextBrother(Tree* child);
Tree* previousBrother(Tree* child);
Tree* getChild(Tree* tree, int index);
void addChild(Tree* tree, Tree* child);

#endif /* TREE_H_ */
