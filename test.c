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
double evaluateLispExpressionWithVars(char* expression, HashTable variables);
bool checkSingleExpressionToString(const char* lisp_expression,
                                       const char* expected_string);
bool fpEq(double a, double b);

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
    ASSERT(fpEq(evaluateLispExpression("(1)"), 1));
    ASSERT(fpEq(evaluateLispExpression("(+(1))"), 1));
    ASSERT(fpEq(evaluateLispExpression("(-(1))"), -1));
    ASSERT(fpEq(evaluateLispExpression("(-(-(1)))"), 1));
    ASSERT(fpEq(evaluateLispExpression("(+(1)(2))"), 3));
    ASSERT(fpEq(evaluateLispExpression("(-(1)(2))"), -1));
    ASSERT(fpEq(evaluateLispExpression("(*(3)(2))"), 6));
    ASSERT(fpEq(evaluateLispExpression("(/(3)(2))"), 1.5));
    ASSERT(isnan((float)evaluateLispExpression("(/(3)(0))")));
    ASSERT(fpEq(evaluateLispExpression("($(2)(3))"), 5));
    ASSERT(fpEq(evaluateLispExpression("($(-(5))(10))"), 40));
    ASSERT(isnan((float)evaluateLispExpression("($(3)(2))")));
    ASSERT(isnan((float)evaluateLispExpression("($(2)(/(11)(5)))")));
    ASSERT(isnan((float)evaluateLispExpression("($(/(11)(5))(2))")));

    // the following expressions is equivalent to: "1 + --+2 * 3 $ 5 * (-6) - 4 / 2 $ 2 / (1 + 4)".
    // (checks proper evaluation of complex expression)
    ASSERT(fpEq(evaluateLispExpression("(-(+(1)(*(*(-(-(+(2))))($(3)(5)))(-(6))))(/(/(4)($(2)(2)))(+(1)(4))))"), -143.4));
    // the following expressions is equivalent to: "1 + --+2 * 3 $ 5 * (-6) - 4 / 2 $ 0 / (1 + 4)".
    // (checks proper propagation of NAN result from "2 $ 0")
    ASSERT(isnan((float)evaluateLispExpression("(-(+(1)(*(*(-(-(+(2))))($(3)(5)))(-(6))))(/(/(4)($(2)(1)))(+(1)(4))))")));
    // the following expressions is equivalent to: "1 + --+2 * 3 $ 5 * (-6) - 4 / -2 $ 2 / (1 + 4)".
    // (checks proper propagation of NAN result from division by zero)
    ASSERT(isnan((float)evaluateLispExpression("(-(+(1)(*(*(-(-(+(2))))($(3)(5)))(-(6))))(/(/(4)($(2)(1)))(+(1)(4))))")));

    ASSERT(fpEq(evaluateLispExpression("(max(3)(-(2))(4))"), 4));
    ASSERT(fpEq(evaluateLispExpression("(min(3)(-(2))(4))"), -2));
    ASSERT(isnan((float)(evaluateLispExpression("(max(3)(/(1)(0))(4))"))));
    ASSERT(isnan((float)(evaluateLispExpression("(min(3)($(5)(2))(4))"))));
    ASSERT(fpEq(evaluateLispExpression("(average(3)(-(2))(4))"), 1.6666666666666667));
    ASSERT(isnan((float)(evaluateLispExpression("(average(3)(/(1)(0))(4))"))));
    ASSERT(fpEq(evaluateLispExpression("(median(3)(-(2))(4))"), 3));
    ASSERT(fpEq(evaluateLispExpression("(median(3)(-(2))(5)(4))"), 3.5));
    ASSERT(isnan((float)(evaluateLispExpression("(median(3)(/(1)(0))(4))"))));
    ASSERT(fpEq(evaluateLispExpression("(median(8)(7)(4)(5)(9)(1)(2)(3)(6))"), 5));
    ASSERT(fpEq(evaluateLispExpression("(median(8)(7)(4)(5)(9)(1)(2)(3)(6)(0))"), 4.5));

    HashTable variables = createHashTable();

    hashInsert(variables, "a", 3);
    hashInsert(variables, "b", -7);

    ASSERT(fpEq(evaluateLispExpressionWithVars("(*(a)(2))", variables), 6));
    ASSERT(fpEq(evaluateLispExpressionWithVars("(+(a)(b))", variables), -4));
    ASSERT(isnan((float)evaluateLispExpressionWithVars("(+(4)(c))", variables)));
    ASSERT(fpEq(evaluateLispExpressionWithVars("(=(c)(8))", variables), 8));
    ASSERT(fpEq(evaluateLispExpressionWithVars("(+(4)(c))", variables), 12));
    ASSERT(fpEq(evaluateLispExpressionWithVars("(=(a)(/(1)(2)))", variables), 0.5));

    ASSERT(isnan((float)evaluateLispExpressionWithVars("(=(c)(/(5)(0)))", variables)));
    ASSERT(isnan((float)evaluateLispExpressionWithVars("(=(e)(/(5)(0)))", variables)));

    ASSERT(fpEq(evaluateLispExpressionWithVars("(a)", variables), 0.5));
    ASSERT(fpEq(evaluateLispExpressionWithVars("(b)", variables), -7));
    ASSERT(fpEq(evaluateLispExpressionWithVars("(c)", variables), 8));
    ASSERT(!hashContains(variables, "e"));

    destroyHashTable(variables);

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
    ASSERT(fpEq(2.71828, hashGetValue(table, "e")));
    ASSERT(fpEq(3.1415, hashGetValue(table, "pi")));
    ASSERT(fpEq(5, hashGetValue(table, "first")));
    ASSERT(fpEq(5.5, hashGetValue(table, "second")));
    ASSERT(hashContains(table, "first"));
    hashInsert(table, "first", 6);
    hashInsert(table, "second", 9);
    ASSERT(fpEq(4, hashGetSize(table)));
    ASSERT(fpEq(2.71828, hashGetValue(table, "e")));
    ASSERT(fpEq(3.1415, hashGetValue(table, "pi")));
    ASSERT(fpEq(6, hashGetValue(table, "first")));
    ASSERT(fpEq(9, hashGetValue(table, "second")));
    ASSERT(hashContains(table, "first"));
    hashDelete(table, "first");
    hashDelete(table, "second");
    ASSERT(2 == hashGetSize(table));
    ASSERT(!hashIsEmpty(table));
    ASSERT(fpEq(2.71828, hashGetValue(table, "e")));
    ASSERT(fpEq(3.1415, hashGetValue(table, "pi")));
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
    ASSERT(fpEq(1, hashGetValue(table2, "0UDBZE")));
    ASSERT(fpEq(2, hashGetValue(table2, "YYHMYZ")));
    ASSERT(fpEq(3, hashGetValue(table2, "U4BQBV")));
    ASSERT(fpEq(4, hashGetValue(table2, "2V66VL")));
    ASSERT(fpEq(5, hashGetValue(table2, "PNTKJK")));
    hashInsert(table2, "0UDBZE", 10);
    hashInsert(table2, "YYHMYZ", 20);
    ASSERT(hashContains(table2, "YYHMYZ"));
    ASSERT(fpEq(10, hashGetValue(table2, "0UDBZE")));
    ASSERT(fpEq(20, hashGetValue(table2, "YYHMYZ")));
    ASSERT(fpEq(3, hashGetValue(table2, "U4BQBV")));
    ASSERT(fpEq(4, hashGetValue(table2, "2V66VL")));
    ASSERT(fpEq(5, hashGetValue(table2, "PNTKJK")));
    hashDelete(table2, "2V66VL");
    hashDelete(table2, "PNTKJK");
    ASSERT(fpEq(10, hashGetValue(table2, "0UDBZE")));
    ASSERT(fpEq(20, hashGetValue(table2, "YYHMYZ")));
    ASSERT(fpEq(3, hashGetValue(table2, "U4BQBV")));
    destroyHashTable(table2);
}

void test_variable_file_parsing()
{
    HashTable table = createHashTable();

    ASSERT(hashGetSize(table) == 0);

    char line[100] = "aaa = 111";
    parseVariableAssignmentLine(line, table);
    ASSERT(hashGetSize(table) == 1);
    ASSERT(fpEq(111, hashGetValue(table, "aaa")));

    strcpy(line, "aaa  \t  = \t 222");
    parseVariableAssignmentLine(line, table);
    ASSERT(hashGetSize(table) == 1);
    ASSERT(fpEq(222, hashGetValue(table, "aaa")));

    strcpy(line, "bBb = 333\n");
    parseVariableAssignmentLine(line, table);
    ASSERT(hashGetSize(table) == 2);
    ASSERT(fpEq(333, hashGetValue(table, "bBb")));

    strcpy(line, "aaa = -444");
    parseVariableAssignmentLine(line, table);
    ASSERT(hashGetSize(table) == 2);
    ASSERT(fpEq(-444, hashGetValue(table, "aaa")));

    destroyHashTable(table);
}

void test_expression_to_string()
{
    ASSERT(checkSingleExpressionToString("(+(5)(2))", "(5+2)"));
    ASSERT(checkSingleExpressionToString("(=(a)(3))", "(a=3)"));
    ASSERT(checkSingleExpressionToString("(+(a)(*(3)(2)))", "(a+(3*2))"));
    ASSERT(checkSingleExpressionToString("(=(b)(+(a)(1)))", "(b=(a+1))"));
    ASSERT(checkSingleExpressionToString("(<>)", "(<>)"));
    ASSERT(checkSingleExpressionToString("(1)", "(1)"));
    ASSERT(checkSingleExpressionToString("(-(1))", "(-1)"));
    ASSERT(checkSingleExpressionToString("(+(+(-(+(-(2))))))", "(+(+(-(+(-2)))))"));
    ASSERT(checkSingleExpressionToString("(max(5)(32)(+(17)(5)))", "(max(5,32,(17+5)))"));
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
    HashTable variables = createHashTable();
    double res = evaluateLispExpressionWithVars(expression, variables);
    destroyHashTable(variables);
    return res;
}

double evaluateLispExpressionWithVars(char* expression, HashTable variables)
{
    Tree* expression_tree = parseLispExpression(expression);
    double res = evaluateExpressionTree(expression_tree, variables);
    destroyTree(expression_tree);
    return res;
}

bool checkSingleExpressionToString(const char* lisp_expression,
                                       const char* expected_string)
{
    char buffer[MAX_LINE_LENGTH + 1];
    Tree* tree = parseLispExpression(lisp_expression);
    expressionToString(tree, buffer, sizeof(buffer));
    bool test_passed = (strcmp(buffer, expected_string) == 0);
    destroyTree(tree);
    return test_passed;
}

/* Check floating point equality up to small error */
bool fpEq(double a, double b)
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