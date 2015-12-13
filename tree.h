/*
 * Tree Module
 */

#ifndef TREE_H_
#define TREE_H_

/*
 * Types
 */

typedef struct Tree Tree;
typedef struct ChildrenIterator ChildrenIterator;

/*
 * Functions
 */

Tree* createTree(void* value);
void destroyTree(Tree* tree);
unsigned int childrenCount(Tree* tree);
bool hasChildren(Tree* tree);
Tree* firstChild(Tree* tree);
Tree* lastChild(Tree* tree);
Tree* nextBrother(Tree* child);
Tree* previousBrother(Tree* child);
Tree* getChild(Tree* tree, int index);
void addChild(Tree* tree, Tree* child);

#endif /* TREE_H_ */
