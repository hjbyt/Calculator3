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
 * Constants
 */

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
long rangeSum(long a, long b);
bool isNumber(const char* string);
bool isDigit(char c);

/*
 * Module Functions
 */

double evaluateExpressionTree(Tree* tree)
{
    CHECK(tree != NULL);

    if (!hasChildren(tree)) {
        return evaluateNumberExpression(tree);
    }

    char* operator_string = getValue(tree);
    CHECK(strlen(operator_string) == 1);
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
}

/*
 * Internal Functions
 */

double evaluateNumberExpression(Tree* tree)
{
    CHECK(tree != NULL);
    CHECK(!hasChildren(tree));
    char* number_string = getValue(tree);
    CHECK(isNumber(number_string));
    return (double)atoi(number_string);
}

double evaluatePlusExpression(Tree* tree)
{
    CHECK(tree != NULL);
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

double evaluateMinusExpression(Tree* tree)
{
    CHECK(tree != NULL);
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

double evaluateMultiplyExpression(Tree* tree)
{
    CHECK(tree != NULL);
    CHECK(childrenCount(tree) == 2);
    double a = evaluateExpressionTree(firstChild(tree));
    double b = evaluateExpressionTree(lastChild(tree));
    return a * b;
}

double evaluateDivideExpression(Tree* tree)
{
    CHECK(tree != NULL);
    CHECK(childrenCount(tree) == 2);
    double a = evaluateExpressionTree(firstChild(tree));
    double b = evaluateExpressionTree(lastChild(tree));
    if (b == 0) {
        return NAN;
    } else {
        return a / b;
    }
}

double evaluateSumRangeExpression(Tree* tree)
{
    CHECK(tree != NULL);
    long a = (long)evaluateExpressionTree(firstChild(tree));
    long b = (long)evaluateExpressionTree(lastChild(tree));
    if (a > b) {
        return NAN;
    } else {
        return rangeSum(a, b);
    }
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
    CHECK(a <= b);
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
    CHECK(string != NULL);
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