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

    ASSERT(fp_eq(evaluateLispExpression("(1)"), 1));
    ASSERT(fp_eq(evaluateLispExpression("(+(1))"), 1));
    ASSERT(fp_eq(evaluateLispExpression("(-(1))"), -1));
    ASSERT(fp_eq(evaluateLispExpression("(-(-(1)))"), 1));
    ASSERT(fp_eq(evaluateLispExpression("(+(1)(2))"), 3));
    ASSERT(fp_eq(evaluateLispExpression("(-(1)(2))"), -1));
    ASSERT(fp_eq(evaluateLispExpression("(*(3)(2))"), 6));
    ASSERT(fp_eq(evaluateLispExpression("(/(3)(2))"), 1.5));
    ASSERT(isnan((float)evaluateLispExpression("(/(3)(0))")));
    ASSERT(fp_eq(evaluateLispExpression("($(2)(3))"), 5));
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

void test_hashtable() 
{
    HashTable table = createHashTable();
    ASSERT(!hashContains(table, "first"));
    ASSERT(0 == hashGetSize(table));
    ASSERT(hashIsEmpty(table));
    hashInsert(table, "first", 5);
    hashInsert(table, "second", 5.5);
    hashInsert(table, "pi", 3.1415);
    hashInsert(table, "e", 2.71828);
    ASSERT(4 == hashGetSize(table));
    ASSERT(!hashIsEmpty(table));
    ASSERT(fp_eq(2.71828, hashGetValue(table, "e")));
    ASSERT(fp_eq(3.1415, hashGetValue(table, "pi")));
    ASSERT(fp_eq(5, hashGetValue(table, "first")));
    ASSERT(fp_eq(5.5, hashGetValue(table, "second")));
    ASSERT(hashContains(table, "first"));
    hashInsert(table, "first", 6);
    hashInsert(table, "second", 9);
    ASSERT(fp_eq(4, hashGetSize(table)));
    ASSERT(fp_eq(2.71828, hashGetValue(table, "e")));
    ASSERT(fp_eq(3.1415, hashGetValue(table, "pi")));
    ASSERT(fp_eq(6, hashGetValue(table, "first")));
    ASSERT(fp_eq(9, hashGetValue(table, "second")));
    ASSERT(hashContains(table, "first"));
    hashDelete(table, "first");
    hashDelete(table, "second");
    ASSERT(2 == hashGetSize(table));
    ASSERT(!hashIsEmpty(table));
    ASSERT(fp_eq(2.71828, hashGetValue(table, "e")));
    ASSERT(fp_eq(3.1415, hashGetValue(table, "pi")));
    ASSERT(!hashContains(table, "first"));
    destroyHashTable(table);
    
    HashTable table2 = createHashTable();
    
    ASSERT(!hashContains(table2, "YYHMYZ"));
    hashInsert(table2, "0UDBZE", 1);
    ASSERT(!hashContains(table2, "YYHMYZ"));
    hashInsert(table2, "YYHMYZ", 2);
    ASSERT(hashContains(table2, "YYHMYZ"));
    hashInsert(table2, "U4BQBV", 3);
    hashInsert(table2, "2V66VL", 4);
    hashInsert(table2, "PNTKJK", 5);
    ASSERT(fp_eq(1, hashGetValue(table2, "0UDBZE")));
    ASSERT(fp_eq(2, hashGetValue(table2, "YYHMYZ")));
    ASSERT(fp_eq(3, hashGetValue(table2, "U4BQBV")));
    ASSERT(fp_eq(4, hashGetValue(table2, "2V66VL")));
    ASSERT(fp_eq(5, hashGetValue(table2, "PNTKJK")));
    hashInsert(table2, "0UDBZE", 10);
    hashInsert(table2, "YYHMYZ", 20);
    ASSERT(hashContains(table2, "YYHMYZ"));
    ASSERT(fp_eq(10, hashGetValue(table2, "0UDBZE")));
    ASSERT(fp_eq(20, hashGetValue(table2, "YYHMYZ")));
    ASSERT(fp_eq(3, hashGetValue(table2, "U4BQBV")));
    ASSERT(fp_eq(4, hashGetValue(table2, "2V66VL")));
    ASSERT(fp_eq(5, hashGetValue(table2, "PNTKJK")));
    hashDelete(table2, "2V66VL");
    hashDelete(table2, "PNTKJK");
    ASSERT(fp_eq(10, hashGetValue(table2, "0UDBZE")));
    ASSERT(fp_eq(20, hashGetValue(table2, "YYHMYZ")));
    ASSERT(fp_eq(3, hashGetValue(table2, "U4BQBV")));
    destroyHashTable(table2);
}

void test_variable_file_parsing()
{
    HashTable table = createHashTable();

    ASSERT(hashGetSize(table) == 0);

    char line[100] = "aaa = 111";
    parseVariableAssignmentLine(line, table);
    ASSERT(hashGetSize(table) == 1);
    ASSERT(fp_eq(111, hashGetValue(table, "aaa")));

    strcpy(line, "aaa  \t  = \t 222");
    parseVariableAssignmentLine(line, table);
    ASSERT(hashGetSize(table) == 1);
    ASSERT(fp_eq(222, hashGetValue(table, "aaa")));

    strcpy(line, "bBb = 333\n");
    parseVariableAssignmentLine(line, table);
    ASSERT(hashGetSize(table) == 2);
    ASSERT(fp_eq(333, hashGetValue(table, "bBb")));

    strcpy(line, "aaa = -444");
    parseVariableAssignmentLine(line, table);
    ASSERT(hashGetSize(table) == 2);
    ASSERT(fp_eq(-444, hashGetValue(table, "aaa")));

    destroyHashTable(table);
}

void test_expression_to_string()
{
    char buffer[100];
    Tree* tree;

    tree = parseLispExpression("(+(5)(2))");
    expressionToString(tree, buffer, sizeof(buffer));
    ASSERT_EQ_STR(buffer, "(5+2)");
    destroyTree(tree);

    tree = parseLispExpression("(=(a)(3))");
    expressionToString(tree, buffer, sizeof(buffer));
    ASSERT_EQ_STR(buffer, "(a=3)");
    destroyTree(tree);

    tree = parseLispExpression("(+(a)(*(3)(2)))");
    expressionToString(tree, buffer, sizeof(buffer));
    ASSERT_EQ_STR(buffer, "(a+(3*2))");
    destroyTree(tree);

    tree = parseLispExpression("(=(b)(+(a)(1)))");
    expressionToString(tree, buffer, sizeof(buffer));
    ASSERT_EQ_STR(buffer, "(b=(a+1))");
    destroyTree(tree);

    tree = parseLispExpression("(<>)");
    expressionToString(tree, buffer, sizeof(buffer));
    ASSERT_EQ_STR(buffer, "(<>)");
    destroyTree(tree);

    tree = parseLispExpression("(-(1))");
    expressionToString(tree, buffer, sizeof(buffer));
    ASSERT_EQ_STR(buffer, "-1"); /* TODO: is this the right string to expect ? */
    destroyTree(tree);

    tree = parseLispExpression("(+(+(-(+(-(2))))))");
    expressionToString(tree, buffer, sizeof(buffer));
    ASSERT_EQ_STR(buffer, "++-+-2"); /* TODO: is this the right string to expect ? */
    destroyTree(tree);

    tree = parseLispExpression("(max(5)(32)(+(17)(5)))");
    expressionToString(tree, buffer, sizeof(buffer));
    ASSERT_EQ_STR(buffer, "max(5, 32, (17+5))"); /* TODO: is this the right string to expect ? */
    destroyTree(tree);
}

int main()
{
    printf("Running Tests...\n");
    test_tree();
    test_parse();
    test_calculate();
    test_hashtable();
    test_variable_file_parsing();
    test_expression_to_string();
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

/**
This is a python program to generate hash collisions for the hash table tests

def h(s):
    i = 571
    cp = 31
    for c in s:
        i = cp * i + ord(c)
        i = i % 100
    return i

>>> h('find_colision')
5
import random
import string
i = 0
while i < 5:
    s = ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(6))
    if h(s) == 57:
    print s
    i += 1
0UDBZE
PNTKJK
2V66VL
U4BQBV
YYHMYZ
*/