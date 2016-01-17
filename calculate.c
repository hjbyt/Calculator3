/*
 * Expression Calculation Module
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "calculate.h"
#include "common.h"

/*
 * Types
 */

/* This defines the interface we need to use for all functions that are used
   for evaluating different parts of the calculator */
typedef double (*EvaluatorFunc)(Tree*, HashTable);

/* An internal structure that maps between a string representing a calculation in
   the calculator, and the function that implements it internally */
typedef struct OperationAndEvaluator_
{
    char* operation_string;
    EvaluatorFunc evaluator;
} OperationAndEvaluator;

/*
 * Internal Function Declarations
 */

EvaluatorFunc getEvaluator(char* operation_string);
double evaluateTerminalExpression(Tree* tree, HashTable variables);
double evaluatePlusExpression(Tree* tree, HashTable variables);
double evaluateMinusExpression(Tree* tree, HashTable variables);
double evaluateMultiplyExpression(Tree* tree, HashTable variables);
double evaluateDivideExpression(Tree* tree, HashTable variables);
double evaluateSumRangeExpression(Tree* tree, HashTable variables);
double evaluateAssignmentExpression(Tree* tree, HashTable variables);
double evaluateMinExpression(Tree* tree, HashTable variables);
double evaluateMaxExpression(Tree* tree, HashTable variables);
double evaluateAverageExpression(Tree* tree, HashTable variables);
double evaluateMedianExpression(Tree* tree, HashTable variables);
long long int  rangeSum(long long int  a, long long int  b);
bool isNumber(const char* string);
bool isDigit(char c);
int compareDouble(const void* a, const void* b);

/*
 * Constants
 */

/* An internal epsilon by which we determine if a given double actually represents
   an integer (and not a floating point number) for the sum-range function */
#define EQUALITY_THRESHOLD 0.000001

/* This struct maps between the string representation of the operations
   and the functions that implement their calculation */
const OperationAndEvaluator OPERATIONS[] = {
        {"+",       evaluatePlusExpression      },
        {"-",       evaluateMinusExpression     },
        {"*",       evaluateMultiplyExpression  },
        {"/",       evaluateDivideExpression    },
        {"$",       evaluateSumRangeExpression  },
        {"=",       evaluateAssignmentExpression},
        {"min",     evaluateMinExpression       },
        {"max",     evaluateMaxExpression       },
        {"average", evaluateAverageExpression   },
        {"median",  evaluateMedianExpression    },
};

/*
 * Module Functions
 */

double evaluateExpressionTree(Tree* tree, HashTable variables)
{
    VERIFY(tree != NULL);
    VERIFY(variables != NULL);

    if (!hasChildren(tree)) {
        return evaluateTerminalExpression(tree, variables);
    }

    char* operation_string = getValue(tree);
    EvaluatorFunc evaluator = getEvaluator(operation_string);
    VERIFY(evaluator != NULL);

    /* Evaluate operation */
    return evaluator(tree, variables);
}

/*
 * Internal Functions
 */

/**
 * Returns the relevant evaluating function for a given evaluation literal
 *
 * @param
 * 		char* operation_string - The name of the operation to evaluate.
 *
 * @preconditions
 *      - operation_string != NULL
 *
 * @return
 *		The function that evaluates the given type of calculation,
 *    or NULL if the given operation is not supported.
 */
EvaluatorFunc getEvaluator(char* operation_string)
{
    VERIFY(operation_string != NULL);
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
 * Evaluate a terminal (number or variable) expression sub-tree (tree leaf).
 *
 * @param
 * 		Tree* tree - Expression sub-tree to evaluate.
 * 		HashTable variables - variables to use for evaluation.
 *
 * @preconditions
 *      - tree != NULL
 *      - tree is a leaf node.
 *      - The value of tree is a valid number string
 *      - variables != NULL
 *
 * @return
 *		Evaluation result.
 */
double evaluateTerminalExpression(Tree* tree, HashTable variables)
{
    VERIFY(tree != NULL);
    VERIFY(!hasChildren(tree));
    VERIFY(variables != NULL);
    char* terminal = getValue(tree);
    if (isName(terminal)) {
        if (hashContains(variables, terminal)) {
            return hashGetValue(variables, terminal);
        } else {
            return NAN;
        }
    } else if (isNumber(terminal)) {
        return (double)atoi(terminal);
    } else {
        panic();
    }
}

/**
 * Evaluate a plus expression sub-tree.
 *
 * @param
 * 		Tree* tree - Expression sub-tree to evaluate.
 * 		HashTable variables - variables to use for evaluation.
 *
 * @preconditions
 *      - tree != NULL
 *      - tree has 1 or 2 children.
 *      - variables != NULL
 *
 * @return
 *		Evaluation result.
 */
double evaluatePlusExpression(Tree* tree, HashTable variables)
{
    VERIFY(tree != NULL);
    VERIFY(variables != NULL);
    switch (childrenCount(tree)) {
        case 1:
            return evaluateExpressionTree(firstChild(tree), variables);
        case 2:
        {
            double a = evaluateExpressionTree(firstChild(tree), variables);
            double b = evaluateExpressionTree(lastChild(tree), variables);
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
 * 		HashTable variables - variables to use for evaluation.
 *
 * @preconditions
 *      - tree != NULL
 *      - tree has 1 or 2 children.
 *      - variables != NULL
 *
 * @return
 *		Evaluation result.
 */
double evaluateMinusExpression(Tree* tree, HashTable variables)
{
    VERIFY(tree != NULL);
    VERIFY(variables != NULL);
    switch (childrenCount(tree))
    {
        case 1:
            return -evaluateExpressionTree(firstChild(tree), variables);
        case 2:
        {
            double a = evaluateExpressionTree(firstChild(tree), variables);
            double b = evaluateExpressionTree(lastChild(tree), variables);
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
 * 		HashTable variables - variables to use for evaluation.
 *
 * @preconditions
 *      - tree != NULL
 *      - tree has 2 children.
 *      - variables != NULL
 *
 * @return
 *		Evaluation result.
 */
double evaluateMultiplyExpression(Tree* tree, HashTable variables)
{
    VERIFY(tree != NULL);
    VERIFY(childrenCount(tree) == 2);
    VERIFY(variables != NULL);
    double a = evaluateExpressionTree(firstChild(tree), variables);
    double b = evaluateExpressionTree(lastChild(tree), variables);
    return a * b;
}

/**
 * Evaluate a divide expression sub-tree.
 * If the value of the divisor is 0, then NAN is returned.
 *
 * @param
 * 		Tree* tree - Expression sub-tree to evaluate.
 * 		HashTable variables - variables to use for evaluation.
 *
 * @preconditions
 *      - tree != NULL
 *      - tree has 2 children.
 *      - variables != NULL
 *
 * @return
 *		Evaluation result.
 */
double evaluateDivideExpression(Tree* tree, HashTable variables)
{
    VERIFY(tree != NULL);
    VERIFY(childrenCount(tree) == 2);
    VERIFY(variables != NULL);
    double a = evaluateExpressionTree(firstChild(tree), variables);
    double b = evaluateExpressionTree(lastChild(tree), variables);
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
 * 		HashTable variables - variables to use for evaluation.
 *
 * @preconditions
 *      - tree != NULL
 *      - tree has 2 children.
 *      - variables != NULL
 *
 * @return
 *		Evaluation result.
 */
double evaluateSumRangeExpression(Tree* tree, HashTable variables)
{
    VERIFY(tree != NULL);
    VERIFY(childrenCount(tree) == 2);
    VERIFY(variables != NULL);

    double a_ = evaluateExpressionTree(firstChild(tree), variables);
    double b_ = evaluateExpressionTree(lastChild(tree), variables);

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

/**
 * Evaluate an assignment expression.
 * If the value of the expression to assign is invalid, then NAN is returned.
 *
 * @param
 * 		Tree* tree - Assignment sub-tree to evaluate.
 *    HashTable variables - variables to use for evaluation,
 * 		                      and to update after assignment.
 *
 * @preconditions
 *      - tree != NULL
 *      - tree has 2 children.
 *      - First child of the tree is a valid variable sub-expression,
 *        i.e. it has no children, and it's value is a valid variable name.
 *      - variables != NULL
 *
 * @return
 *		Evaluation result (the value assigned to the variable).
 */
double evaluateAssignmentExpression(Tree* tree, HashTable variables)
{
    VERIFY(tree != NULL);
    VERIFY(childrenCount(tree) == 2);
    VERIFY(variables != NULL);

    double value = evaluateExpressionTree(lastChild(tree), variables);
    if (isnan((float)value)) {
        return NAN;
    }

    Tree* var_expression = firstChild(tree);
    VERIFY(!hasChildren(var_expression));
    char* name = getValue(var_expression);
    VERIFY(isName(name));
    hashInsert(variables, name, value);

    return value;
}

/**
 * Evaluate a min expression sub-tree.
 * Returns the minimum value from the given children.
 * If any of the sub-expressions is invalid, then NAN is returned.
 *
 * @param
 * 		Tree* tree - Expression sub-tree to evaluate.
 *    HashTable variables - variables to use for evaluation.
 *
 * @preconditions
 *      - tree != NULL
 *      - tree has children.
 *      - variables != NULL
 *
 * @return
 *		Evaluation result.
 */
double evaluateMinExpression(Tree* tree, HashTable variables)
{
    VERIFY(NULL != tree);
    VERIFY(hasChildren(tree));
    VERIFY(variables != NULL);

    double minValue = NAN;
    for (Tree* child = firstChild(tree);
         child != NULL;
         child = nextBrother(child))
    {
        double current = evaluateExpressionTree(child, variables);
        if (isnan((float)current)) {
            return NAN;
        }
        minValue = fmin(minValue, current);
    }
    
    return minValue;
}

/**
 * Evaluate a max expression sub-tree.
 * Returns the maximum value from the given children.
 * If any of the sub-expressions is invalid, then NAN is returned.
 *
 * @param
 * 		Tree* tree - Expression sub-tree to evaluate.
 * 		HashTable variables - variables to use for evaluation.
 *
 * @preconditions
 *      - tree != NULL
 *      - tree has children.
 *      - variables != NULL
 *
 * @return
 *		Evaluation result.
 */
double evaluateMaxExpression(Tree* tree, HashTable variables)
{
    VERIFY(NULL != tree);
    VERIFY(hasChildren(tree));
    VERIFY(variables != NULL);

    double maxValue = NAN;
    for (Tree* child = firstChild(tree);
         child != NULL;
         child = nextBrother(child))
    {
        double current = evaluateExpressionTree(child, variables);
        if (isnan((float)current)) {
            return NAN;
        }
        maxValue = fmax(maxValue, current);
    }
    
    return maxValue;
}

/**
 * Evaluate an average expression sub-tree.
 * Returns the average value from the given children.
 * If any of the sub-expressions is invalid, then NAN is returned.
 *
 * @param
 * 		Tree* tree - Expression sub-tree to evaluate.
 * 		HashTable variables - variables to use for evaluation.
 *
 * @preconditions
 *      - tree != NULL
 *      - tree has children.
 *      - variables != NULL
 *
 * @return
 *		Evaluation result.
 */
double evaluateAverageExpression(Tree* tree, HashTable variables)
{
    VERIFY(NULL != tree);
    VERIFY(hasChildren(tree));
    VERIFY(variables != NULL);

    double sum = 0;
    for (Tree* child = firstChild(tree);
         child != NULL;
         child = nextBrother(child))
    {
        double current = evaluateExpressionTree(child, variables);
        if (isnan((float)current)) {
            return NAN;
        }
        sum += current;
    }

    return sum / (double)childrenCount(tree);
}

/**
 * Evaluate a median expression sub-tree.
 * Returns the median value from the given children.
 * If any of the sub-expressions is invalid, then NAN is returned.
 *
 * @param
 * 		Tree* tree - Expression sub-tree to evaluate.
 * 		HashTable variables - variables to use for evaluation.
 *
 * @preconditions
 *      - tree != NULL
 *      - tree has children.
 *      - variables != NULL
 *
 * @return
 *		Evaluation result.
 */
double evaluateMedianExpression(Tree* tree, HashTable variables)
{
    VERIFY(NULL != tree);
    VERIFY(hasChildren(tree));
    VERIFY(variables != NULL);

    unsigned int operands_count = childrenCount(tree);
    double* operands = malloc(operands_count * sizeof(double));
    VERIFY(operands != NULL);

    int i = 0;
    for (Tree* child = firstChild(tree);
         child != NULL;
         child = nextBrother(child))
    {
        double current = evaluateExpressionTree(child, variables);
        if (isnan((float)current)) {
            free(operands);
            return NAN;
        }
        operands[i] = current;
        i += 1;
    }

    /* Note: using quickselect would be better,
     * but because it's promised that the number of operands is not greater than 10,
     * it doesn't really matter. */
    qsort(operands, operands_count, sizeof(*operands), compareDouble);

    double result;
    if (operands_count % 2 == 1) {
        result = operands[(operands_count - 1)/2];
    } else {
        result = (operands[operands_count/2] + operands[operands_count/2 - 1]) / 2;
    }

    free(operands);

    return result;
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
 * Compares the values in two given doubles pointed by the arguments.
 * (used as a callback for the qsort function)
 *
 * @param
 *      const void* a - The first value to compare
 *      const void* b - The second value to compare
 *
 * @preconditions
 *      a != NULL
 *      b != NULL
 *
 * @return
 *      -1 if the second value is greater than the first
 *      0 if the values are equall
 *      1 if the first value is greater than the second
 */
int compareDouble(const void* a, const void* b)
{
    double a_ = *(double*)a;
    double b_ = *(double*)b;
    if (a_ < b_) {
        return -1;
    } else if (a_ == b_) {
        return 0;
    } else {
        return 1;
    }
}
