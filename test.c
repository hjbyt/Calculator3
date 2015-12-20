/*
 * Unit Test Module
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "tree.h"
#include "parse.h"
#include "calculate.h"

#define FAIL(msg)                                                       \
    do {                                                                \
		fprintf(stderr,"%s Line %d: %s\n", __FILE__, __LINE__, msg);    \
		exit(EXIT_FAILURE);                                             \
	} while(0)

#define ASSERT(expression)                      \
    do {                                        \
        if(!(expression)) {                     \
            FAIL("expression is false :: ");    \
        }                                       \
    } while (0)

#define ASSERT_EQ_STR(string1, string2) ASSERT(strcmp(string1, string2) == 0)

/*
 * Internal Functions
 */

Tree* createTreeFromLiteral(const char* string);
double evaluateLispExpression(char* expression);
bool fp_eq(double a, double b);

/*
 * Tests
 */

void test_tree()
{
    Tree* root = createTreeFromLiteral("root");
    Tree* child1 = createTreeFromLiteral("c1");
    Tree* child2 = createTreeFromLiteral("c2");
    Tree* child3 = createTreeFromLiteral("c3");

    ASSERT(childrenCount(root) == 0);
    ASSERT(!hasChildren(root));

    ASSERT(isRoot(root));
    ASSERT(isRoot(child1));
    ASSERT(isRoot(child2));
    ASSERT(isRoot(child3));

    addChild(root, child1);
    addChild(root, child2);
    addChild(root, child3);
    ASSERT(childrenCount(root) == 3);
    ASSERT(hasChildren(root));

    ASSERT_EQ_STR(getValue(root), "root");
    ASSERT_EQ_STR(getValue(child1), "c1");
    ASSERT_EQ_STR(getValue(child2), "c2");
    ASSERT_EQ_STR(getValue(child3), "c3");

    ASSERT(isRoot(root));
    ASSERT(!isRoot(child1));
    ASSERT(!isRoot(child2));
    ASSERT(!isRoot(child3));

    ASSERT(getParent(root) == NULL);
    ASSERT(getParent(child1) == root);
    ASSERT(getParent(child2) == root);
    ASSERT(getParent(child3) == root);

    ASSERT(firstChild(root) == child1);
    ASSERT(lastChild(root) == child3);

    ASSERT(getChild(root, 0) == child1);
    ASSERT(getChild(root, 1) == child2);
    ASSERT(getChild(root, 2) == child3);
    ASSERT(getChild(root, -1) == child3);
    ASSERT(getChild(root, -2) == child2);
    ASSERT(getChild(root, -3) == child1);

    ASSERT(previousBrother(root) == NULL);
    ASSERT(nextBrother(root) == NULL);
    ASSERT(previousBrother(child1) == NULL);
    ASSERT(nextBrother(child1) == child2);
    ASSERT(previousBrother(child2) == child1);
    ASSERT(nextBrother(child2) == child3);
    ASSERT(previousBrother(child3) == child2);
    ASSERT(nextBrother(child3) == NULL);

    destroyTree(root);
}

void test_parse()
{
    Tree* parse_tree = parseLispExpression("()");
    ASSERT(isRoot(parse_tree));
    ASSERT(!hasChildren(parse_tree));
    ASSERT_EQ_STR(getValue(parse_tree), "");
    destroyTree(parse_tree);

    parse_tree = parseLispExpression("(123)");
    ASSERT(!hasChildren(parse_tree));
    ASSERT_EQ_STR(getValue(parse_tree), "123");
    destroyTree(parse_tree);

    parse_tree = parseLispExpression("(f1(a1)(a2)(f2(a3)(a4)))");
    ASSERT(hasChildren(parse_tree));
    ASSERT_EQ_STR(getValue(parse_tree), "f1");
    ASSERT_EQ_STR(getValue(getChild(parse_tree, 0)), "a1");
    ASSERT_EQ_STR(getValue(getChild(parse_tree, 1)), "a2");
    ASSERT_EQ_STR(getValue(getChild(parse_tree, 2)), "f2");
    Tree* subtree = getChild(parse_tree, 2);
    ASSERT(hasChildren(subtree));
    ASSERT_EQ_STR(getValue(getChild(subtree, 0)), "a3");
    ASSERT_EQ_STR(getValue(getChild(subtree, 1)), "a4");
    destroyTree(parse_tree);

    parse_tree = parseLispExpression("((a))");
    ASSERT(hasChildren(parse_tree));
    ASSERT_EQ_STR(getValue(parse_tree), "");
    ASSERT_EQ_STR(getValue(firstChild(parse_tree)), "a");
    destroyTree(parse_tree);
}

void test_calculate()
{
    /* Note: it would be better to test calculate directly, without using parseLispExpression,
     *       but it is would be much more cumbersome. */

    ASSERT(evaluateLispExpression("(1)") == 1);
    ASSERT(evaluateLispExpression("(+(1))") == 1);
    ASSERT(evaluateLispExpression("(-(1))") == -1);
    ASSERT(evaluateLispExpression("(-(-(1)))") == 1);
    ASSERT(evaluateLispExpression("(+(1)(2))") == 3);
    ASSERT(evaluateLispExpression("(-(1)(2))") == -1);
    ASSERT(evaluateLispExpression("(*(3)(2))") == 6);
    ASSERT(evaluateLispExpression("(/(3)(2))") == 1.5);
    ASSERT(isnan((float)evaluateLispExpression("(/(3)(0))")));
    ASSERT(evaluateLispExpression("($(2)(3))") == 5);
    ASSERT(isnan((float)evaluateLispExpression("($(3)(2))")));

    // the following expressions is equivalent to: "1 + --+2 * 3 $ 5 * (-6) - 4 / 2 $ 2 / (1 + 4)".
    // (checks proper evaluation of complex expression)
    ASSERT(fp_eq(evaluateLispExpression("(-(+(1)(*(*(-(-(+(2))))($(3)(5)))(-(6))))(/(/(4)($(2)(2)))(+(1)(4))))"), -143.4));
    // the following expressions is equivalent to: "1 + --+2 * 3 $ 5 * (-6) - 4 / 2 $ 0 / (1 + 4)".
    // (checks proper propagation of NAN result from "2 $ 0")
    ASSERT(isnan((float)evaluateLispExpression("(-(+(1)(*(*(-(-(+(2))))($(3)(5)))(-(6))))(/(/(4)($(2)(1)))(+(1)(4))))")));
    // the following expressions is equivalent to: "1 + --+2 * 3 $ 5 * (-6) - 4 / -2 $ 2 / (1 + 4)".
    // (checks proper propagation of NAN result from division by zero)
    ASSERT(isnan((float)evaluateLispExpression("(-(+(1)(*(*(-(-(+(2))))($(3)(5)))(-(6))))(/(/(4)($(2)(1)))(+(1)(4))))")));
}

int main()
{
    printf("Running Tests...\n");
    test_tree();
    test_parse();
    test_calculate();
    printf("All Tests Passed.\n");

    return EXIT_SUCCESS;
}

/*
 * Internal Functions
 */

Tree* createTreeFromLiteral(const char* string)
{
    size_t string_length = strlen(string);
    char* string_copy = malloc(string_length + 1);
    ASSERT(string_copy != NULL);
    memcpy(string_copy, string, string_length + 1);
    return createTree(string_copy);
}

double evaluateLispExpression(char* expression)
{
    Tree* expression_tree = parseLispExpression(expression);
    double res = evaluateExpressionTree(expression_tree);
    destroyTree(expression_tree);
    return res;
}

/* Check floating point equality up to small error */
bool fp_eq(double a, double b)
{
    const double epsilon = 0.000001;
    return (fabs(a-b) <= epsilon);
}
