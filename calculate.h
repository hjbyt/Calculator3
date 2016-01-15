/*
 * Expression Calculation Module
 */

#ifndef CALCULATE_H_
#define CALCULATE_H_

#include "tree.h"
#include "hashtable.h"

/**
 * Evaluate (calculate) an arithmetic or assignment expression tree and variables.
 * If the result of the evaluation is invalid, then NAN is returned.
 * If the expression is an assignment, the given variables table is updated.
 *
 * @param
 * 		Tree* tree - Expression tree to evaluate.
 * 		HashTable variables - variables to use for evaluation,
 * 		                      and to update after assignment.
 *
 * @preconditions
 *      - tree != NULL
 *      - tree is a valid arithmetic expression tree.
 *      - variables != NULL
 *
 * @return
 *		Evaluation result.
 */
double evaluateExpressionTree(Tree* tree, HashTable variables);

#endif /* CALCULATE_H_ */
