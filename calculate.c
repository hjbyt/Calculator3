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
 * Types
 */

/* TODO: doc */
typedef double (*EvaluatorFunc)(Tree*);

/* TODO: doc */
typedef struct OperationAndEvaluator_
{
    char* operation_string;
    EvaluatorFunc evaluator;
} OperationAndEvaluator;

/*
 * Internal Function Declarations
 */

EvaluatorFunc getEvaluator(char* operation_string);
double evaluateNumberExpression(Tree* tree);
double evaluatePlusExpression(Tree* tree);
double evaluateMinusExpression(Tree* tree);
double evaluateMultiplyExpression(Tree* tree);
double evaluateDivideExpression(Tree* tree);
double evaluateSumRangeExpression(Tree* tree);
double evaluateMinExpression(Tree* tree);
double evaluateMaxExpression(Tree* tree);
long long int  rangeSum(long long int  a, long long int  b);
bool isNumber(const char* string);
bool isDigit(char c);

/*
 * Constants
 */

/* TODO: doc */
#define EQUALITY_THRESHOLD 0.000001

/* TODO: doc */
const OperationAndEvaluator OPERATIONS[] = {
        {"+",   evaluatePlusExpression      },
        {"-",   evaluateMinusExpression     },
        {"*",   evaluateMultiplyExpression  },
        {"/",   evaluateDivideExpression    },
        {"$",   evaluateSumRangeExpression  },
        {"min", evaluateMinExpression       },
        {"max", evaluateMaxExpression       },
};

/*
 * Module Functions
 */

double evaluateExpressionTree(Tree* tree)
{
    VERIFY(tree != NULL);

    if (!hasChildren(tree)) {
        return evaluateNumberExpression(tree);
    }

    char* operation_string = getValue(tree);
    EvaluatorFunc evaluator = getEvaluator(operation_string);
    VERIFY(evaluator != NULL);

    /* Evaluate operation */
    return evaluator(tree);
}

/*
 * Internal Functions
 */

/* TODO: doc */
EvaluatorFunc getEvaluator(char* operation_string)
{
    OperationAndEvaluator operation_pair;
    bool found = false;
    for (int i = 0; i < ARRAY_LENGTH(OPERATIONS); ++i)
    {
        operation_pair = OPERATIONS[i];
        if (strcmp(operation_string, operation_pair.operation_string) == 0) {
            found = true;
            break;
        }
    }
    if (found) {
        return operation_pair.evaluator;
    } else {
        return NULL;
    }
}

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

    double a_ = evaluateExpressionTree(firstChild(tree));
    double b_ = evaluateExpressionTree(lastChild(tree));

    long long int a = llround(a_);
    long long int b = llround(b_);

    if (   fabs(a_ - (double)a) > EQUALITY_THRESHOLD
        || fabs(b_ - (double)b) > EQUALITY_THRESHOLD
        || a > b) {
        return NAN;
    } else {
        return (double)rangeSum(a, b);
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
long long int rangeSum(long long int  a, long long int  b)
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