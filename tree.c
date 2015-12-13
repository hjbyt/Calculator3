/*
 * Tree Module
 */

#include <stddef.h>
#include <stdlib.h>
#include "Tree.h"
#include "common.h"

/*
 * Types
 */

struct Tree
{
    char* value;
    unsigned childrenCount;
    Tree* firstChild;
    Tree* lastChild;
    Tree* nextBrother;
    Tree* previousBrother;
    Tree* parent;
};

/*
 * Functions
 */

Tree* createTree(char* value)
{
    CHECK(value != NULL);
    Tree* tree = malloc(sizeof(Tree));
    CHECK(tree != NULL);
    tree->value = value;
    tree->childrenCount = 0;
    tree->firstChild = NULL;
    tree->lastChild = NULL;
    tree->nextBrother = NULL;
    tree->previousBrother = NULL;
    tree->parent = NULL;
    return tree;
}

void destroyTree(Tree* tree)
{
    if (tree == NULL) {
        return;
    }
    for (Tree* child = tree->firstChild;
         child != NULL;
         child = child->nextBrother)
    {
        destroyTree(child);
    }
    free(tree->value);
    free(tree);
}

char* getValue(Tree* tree)
{
    CHECK(tree != NULL);
    return tree->value;
}

unsigned int childrenCount(Tree* tree)
{
    CHECK(tree != NULL);
    return tree->childrenCount;
}

bool hasChildren(Tree* tree)
{
    CHECK(tree != NULL);
    return (tree->childrenCount > 0);
}

/* Note: index can be negative. */
Tree* getChild(Tree* tree, int index)
{
    CHECK(tree != NULL);
    unsigned positive_index;
    if (index < 0) {
        CHECK(index >= -tree->childrenCount);
        positive_index = (tree->childrenCount + index);
    } else {
        CHECK(index < tree->childrenCount);
        positive_index = (unsigned int)index;
    }

    Tree* child = NULL;
    if (positive_index <= tree->childrenCount / 2) {
        /* Scan forward */
        child = tree->firstChild;
        for (int i = 0; i < positive_index; ++i)
        {
            child = child->nextBrother;
        }
    } else {
        /* Scan backward */
        child = tree->lastChild;
        for (int i = tree->childrenCount - 1; i > positive_index; --i)
        {
            child = child->previousBrother;
        }
    }

    return child;
}

Tree* firstChild(Tree* tree)
{
    CHECK(tree != NULL);
    return tree->firstChild;
}

Tree* lastChild(Tree* tree)
{
    CHECK(tree != NULL);
    return tree->lastChild;
}

Tree* nextBrother(Tree* child)
{
    CHECK(child != NULL);
    return child->nextBrother;
}

Tree* previousBrother(Tree* child)
{
    CHECK(child != NULL);
    return child->previousBrother;
}

void addChild(Tree* tree, Tree* child)
{
    CHECK(tree != NULL && child != NULL);
    CHECK(child->parent == NULL
          && child->nextBrother == NULL
          && child->previousBrother == NULL);

    if (!hasChildren(tree)) {
        tree->firstChild = child;
        tree->lastChild = child;
    } else {
        tree->lastChild->nextBrother = child;
        child->previousBrother = tree->lastChild;
        tree->lastChild = child;
    }
    tree->childrenCount += 1;
}
