/*
 * Expression Calculation Module
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include "calculate.h"
#include "common.h"

/*
 * Constants
 */

/* Operations */
#define PLUS_OPERATION      '+'
#define MINUS_OPERATION     '-'
#define MULTIPLY_OPERATION  '*'
#define DIVIDE_OPERATION    '/'
#define SUM_RANGE_OPERATION '$'

/*
 * Internal Function Declarations
 */

double evaluateNumberExpression(Tree* tree);
double evaluatePlusExpression(Tree* tree);
double evaluateMinusExpression(Tree* tree);
double evaluateMultiplyExpression(Tree* tree);
double evaluateDivideExpression(Tree* tree);
double evaluateSumRangeExpression(Tree* tree);
double evaluateMinExpression(Tree* tree);
double evaluateMaxExpression(Tree* tree);
long rangeSum(long a, long b);
bool isNumber(const char* string);
bool isDigit(char c);

/*
 * Module Functions
 */

double evaluateExpressionTree(Tree* tree)
{
    VERIFY(tree != NULL);

    if (!hasChildren(tree)) {
        return evaluateNumberExpression(tree);
    }

    char* operator_string = getValue(tree);
    
    if (strlen(operator_string) == 1) {
        char operator = *operator_string;

        switch (operator)
        {
            case PLUS_OPERATION:
                return evaluatePlusExpression(tree);
            case MINUS_OPERATION:
                return evaluateMinusExpression(tree);
            case MULTIPLY_OPERATION:
                return evaluateMultiplyExpression(tree);
            case DIVIDE_OPERATION:
                return evaluateDivideExpression(tree);
            case SUM_RANGE_OPERATION:
                return evaluateSumRangeExpression(tree);
            default:
                panic();
        }    
    } else {
        if (strcmp(operator_string, "min"))
            return evaluateMinExpression(tree);
        else if (strcmp(operator_string, "max"))
            return evaluateMaxExpression(tree);
        else
            panic();
    }
}

/*
 * Internal Functions
 */

/**
 * Evaluate a number expression sub-tree (tree leaf).
 *
 * @param
 * 		Tree* tree - Expression sub-tree to evaluate.
 *
 * @preconditions
 *      - tree != NULL
 *      - tree is a leaf node.
 *      - The value of tree is a valid number string
 *
 * @return
 *		Evaluation result.
 */
double evaluateNumberExpression(Tree* tree)
{
    VERIFY(tree != NULL);
    VERIFY(!hasChildren(tree));
    char* number_string = getValue(tree);
    VERIFY(isNumber(number_string));
    return (double)atoi(number_string);
}

/**
 * Evaluate a plus expression sub-tree.
 *
 * @param
 * 		Tree* tree - Expression sub-tree to evaluate.
 *
 * @preconditions
 *      - tree != NULL
 *      - tree has 1 or 2 children.
 *
 * @return
 *		Evaluation result.
 */
double evaluatePlusExpression(Tree* tree)
{
    VERIFY(tree != NULL);
    switch (childrenCount(tree)) {
        case 1:
            return evaluateExpressionTree(firstChild(tree));
        case 2:
        {
            double a = evaluateExpressionTree(firstChild(tree));
            double b = evaluateExpressionTree(lastChild(tree));
            return a + b;
        }
        default:
            panic();
    }
}

/**
 * Evaluate a minus expression sub-tree.
 *
 * @param
 * 		Tree* tree - Expression sub-tree to evaluate.
 *
 * @preconditions
 *      - tree != NULL
 *      - tree has 1 or 2 children.
 *
 * @return
 *		Evaluation result.
 */
double evaluateMinusExpression(Tree* tree)
{
    VERIFY(tree != NULL);
    switch (childrenCount(tree))
    {
        case 1:
            return -evaluateExpressionTree(firstChild(tree));
        case 2:
        {
            double a = evaluateExpressionTree(firstChild(tree));
            double b = evaluateExpressionTree(lastChild(tree));
            return a - b;
        }
        default:
            panic();
    }
}

/**
 * Evaluate a multiply expression sub-tree.
 *
 * @param
 * 		Tree* tree - Expression sub-tree to evaluate.
 *
 * @preconditions
 *      - tree != NULL
 *      - tree has 2 children.
 *
 * @return
 *		Evaluation result.
 */
double evaluateMultiplyExpression(Tree* tree)
{
    VERIFY(tree != NULL);
    VERIFY(childrenCount(tree) == 2);
    double a = evaluateExpressionTree(firstChild(tree));
    double b = evaluateExpressionTree(lastChild(tree));
    return a * b;
}

/**
 * Evaluate a divide expression sub-tree.
 * If the value of the divisor is 0, then NAN is returned.
 *
 * @param
 * 		Tree* tree - Expression sub-tree to evaluate.
 *
 * @preconditions
 *      - tree != NULL
 *      - tree has 2 children.
 *
 * @return
 *		Evaluation result.
 */
double evaluateDivideExpression(Tree* tree)
{
    VERIFY(tree != NULL);
    VERIFY(childrenCount(tree) == 2);
    double a = evaluateExpressionTree(firstChild(tree));
    double b = evaluateExpressionTree(lastChild(tree));
    if (b == 0) {
        return NAN;
    } else {
        return a / b;
    }
}

/**
 * Evaluate a sum-range expression sub-tree.
 * If the value of the first operand is greater the second operand, then NAN is returned.
 *
 * @param
 * 		Tree* tree - Expression sub-tree to evaluate.
 *
 * @preconditions
 *      - tree != NULL
 *      - tree has 2 children.
 *
 * @return
 *		Evaluation result.
 */
double evaluateSumRangeExpression(Tree* tree)
{
    VERIFY(tree != NULL);
    VERIFY(childrenCount(tree) == 2);
    long a = (long)evaluateExpressionTree(firstChild(tree));
    long b = (long)evaluateExpressionTree(lastChild(tree));
    if (a > b) {
        return NAN;
    } else {
        return rangeSum(a, b);
    }
}

double evaluateMinExpression(Tree* tree)
{
    VERIFY(NULL != tree);
    VERIFY(hasChildren(tree));
    Tree* child = firstChild(tree);
    double minValue = DBL_MAX;
    
    while (child) {
        double current = evaluateExpressionTree(child);
        minValue = fmin(minValue, current);
        child = nextBrother(child);
    }
    
    return minValue;
}

double evaluateMaxExpression(Tree* tree)
{
    VERIFY(NULL != tree);
    VERIFY(hasChildren(tree));
    Tree* child = firstChild(tree);
    double maxValue = -DBL_MAX;
    
    while (child) {
        double current = evaluateExpressionTree(child);
        maxValue = fmax(maxValue, current);
        child = nextBrother(child);
    }
    
    return maxValue;
}

/**
 * Calculate the sum of all integers in range [a, b].
 *
 * @param
 *      long a - Range start
 *      long b - Range end
 *
 * @preconditions
 *      a <= b
 *
 * @return
 *      Calculation result.
 */
long rangeSum(long a, long b)
{
    VERIFY(a <= b);
    return (a + b) * (b - a + 1) / 2;
}

/**
 * Check if the given string is a number made of digits only.
 * Note: this function returns false for strings such as: "-123", "+123", "1.0".
 *
 * @param
 *      const char* string - String to check.
 *
 * @preconditions
 *      string != NULL
 *
 * @return
 *      true iff string is a number made of digits only.
 */
bool isNumber(const char* string)
{
    VERIFY(string != NULL);
    const char* c = string;

    if (*c == '\0') {
        return false;
    }

    do {
        if (!isDigit(*c)) {
            return false;
        }
        c++;
    } while (*c != '\0');

    return true;
}

/**
 * Check if the given character is a digit.
 *
 * @param
 *      char c - Character to check.
 *
 * @return
 *      true iff c is a digit.
 */
bool isDigit(char c)
{
    return (c >= '0' && c <= '9');
}