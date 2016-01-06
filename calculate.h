/*
 * Expression Calculation Module
 */

#ifndef CALCULATE_H_
#define CALCULATE_H_

#include "tree.h"
#include "hashtable.h"

/* TODO: update doc */
/**
 * Evaluate (calculate) an arithmetic expression tree.
 * If the result of the evaluation is invalid, then NAN is returned.
 *
 * @param
 * 		Tree* tree - Expression tree to evaluate.
 *
 * @preconditions
 *      - tree != NULL
 *      - tree is a valid arithmetic expression tree.
 *
 * @return
 *		Evaluation result.
 */
double evaluateExpressionTree(Tree* tree, HashTable variables);

#endif /* CALCULATE_H_ */
