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

/*
 * Functions
 */

/**
 * Create a new tree node, and assign the given value to it.
 * The created tree has to be destroyed by destroyTree.
 * The created tree takes ownership of value, and will free it when destroyed.
 *
 * @param
 * 		char* value - String to store in the tree node.
 *
 * @preconditions
 *      - value != NULL, value points to memory allocated by malloc.
 *      - There is enough memory to allocate a new Tree node.
 *
 * @return
 *		Pointer to the new tree node.
 */
Tree* createTree(char* value);

/**
 * Destroy a previously created tree, and all of it's children sub-trees recursively.
 * Values assigned to tree nodes are freed as well.
 * If the given tree is NULL, then nothing is done.
 *
 * @param
 * 		Tree* tree - Tree to destroy.
 *
 * @preconditions
 *      tree was previously created by createTree, and wasn't destroyed already.
 */
void destroyTree(Tree* tree);

/**
 * Get the value stored in the tree node.
 *
 * @param
 * 		Tree* tree - Tree node to retrieve value from.
 *
 * @preconditions
 *      tree != NULL
 *
 * @return
 *		Value of the tree node.
 */
char* getValue(Tree* tree);

/**
 * Get the amount of children sub-tree the given tree has.
 *
 * @param
 * 		Tree* tree - Tree node to examine.
 *
 * @preconditions
 *      tree != NULL
 *
 * @return
 *		Amount of children.
 */
unsigned int childrenCount(Tree* tree);

/**
 * Check if the given tree has children sub-trees.
 *
 * @param
 * 		Tree* tree - Tree node to examine.
 *
 * @preconditions
 *      tree != NULL
 *
 * @return
 *		true iff the given tree has children sub-trees.
 */
bool hasChildren(Tree* tree);

/**
 * Get the first child of the given tree.
 *
 * @param
 * 		Tree* tree - Tree node to examine.
 *
 * @preconditions
 *      tree != NULL
 *
 * @return
 *		First child of the given tree.
 */
Tree* firstChild(Tree* tree);

/**
 * Get the last child of the given tree.
 *
 * @param
 * 		Tree* tree - Tree node to examine.
 *
 * @preconditions
 *      tree != NULL
 *
 * @return
 *		Last child of the given tree.
 */
Tree* lastChild(Tree* tree);

/**
 * Get the next brother of the given child node.
 * I.E. get the next child of the parent of the given node.
 *
 * @param
 * 		Tree* child - child node.
 *
 * @preconditions
 *      child != NULL
 *
 * @return
 *		Next sub-tree brother, or NULL if the given node is the last child of it's parent.
 */
Tree* nextBrother(Tree* child);

/**
 * Get the previous brother of the given child node.
 * I.E. get the previous child of the parent of the given node.
 *
 * @param
 * 		Tree* child - child node.
 *
 * @preconditions
 *      child != NULL
 *
 * @return
 *		Previous sub-tree brother, or NULL if the given node is the first child of it's parent.
 */
Tree* previousBrother(Tree* child);

/**
 * Get the a child subtree of the given tree by index.
 * The index is zero-based, and can be negative.
 * If the index is negative, then childrenCount(tree) is added to it.
 * For example, index=-1 will get the last child.
 *
 * @param
 * 		Tree* tree - tree node to get it's child.
 * 		int index  - index to the child sub-tree to retrieve
 *
 * @preconditions
 *      - child != NULL
 *      - -childrenCount(tree) <= index < childrenCount(tree)
 *
 * @return
 *		Child subtree
 */
Tree* getChild(Tree* tree, int index);

/**
 * Get the parent tree node of the given tree node.
 *
 * @param
 * 		Tree* tree - Tree node to examine.
 *
 * @preconditions
 *      tree != NULL
 *
 * @return
 *		Parent of the given tree, or NULL if there isn't one.
 */
Tree* getParent(Tree* tree);

/**
 * Check if the given tree node is a root node, i.e. it has no parent node.
 *
 * @param
 * 		Tree* tree - Tree node to examine.
 *
 * @preconditions
 *      tree != NULL
 *
 * @return
 *		true iff the given tree node is a root node.
 */
bool isRoot(Tree* tree);

/**
 * Add a tree as the last sub-tree of another tree.
 * The given child node must be a root node, it can't be a node that was added previously.
 *
 * @param
 * 		Tree* tree  - tree to which the child will be added.
 * 		Tree* child - tree to add as a sub-tree.
 *
 * @preconditions
 *      - tree != NULL
 *      - child != NULL
 *      - isRoot(child).
 */
void addChild(Tree* tree, Tree* child);

#endif /* TREE_H_ */
