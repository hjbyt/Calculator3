
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "hashtable.h"
#include "tree.h"
#include "calculate.h"

/* TODO - Doc */
struct Graph_t {
    const char* outputFileName;
    int graphSize;
    char* drawing;
};

/* Internal functions declarations */
/* TODO - Doc */
void setGraphCharacter(Graph graph, int x, int y, char character);
char* findVariableNameInTree(Tree* root);
char* makeGraphString(Graph graph);

/* External Functions */
Graph makeGraph(const char * outputFileName, int graphSize) 
{
    Graph graph = malloc(sizeof(struct Graph_t));
    VERIFY(NULL != graph);
    
    /* Note - We assume that the given name string lives longer than our graph */
    graph->outputFileName = outputFileName;
    graph->graphSize = graphSize;
    int numberOfSquares = (graphSize + 1) * (graphSize + 1);
    graph->drawing = malloc(sizeof(char) * numberOfSquares);
    VERIFY(NULL != graph->drawing);
    return graph;
}

void addFunctionToGraph(Graph graph, Tree* function)
{
    VERIFY(4 == childrenCount(function));
    
    HashTable table = createHashTable();
    char* variable = findVariableNameInTree(function);
    VERIFY(NULL != variable);
    
    Tree* expression = firstChild(function);
    Tree* child = nextBrother(expression);
    int startValue = (int)evaluateExpressionTree(child, table);
    child = nextBrother(child);
    int endValue = (int)evaluateExpressionTree(child, table);
    child = nextBrother(child);
    char* functionGraphSymbol = getValue(child);
    
    for (int variableValue = startValue; variableValue <= endValue; variableValue++) {
        hashInsert(table, variable, variableValue);
        double result = evaluateExpressionTree(expression, table);
        setGraphCharacter(graph, variableValue, result, functionGraphSymbol[0]);
    }
}

bool drawGraph(Graph graph) 
{
    char* drawing = NULL;
    FILE * outputFile = NULL;
    bool returnValue = false;
    
    drawing = makeGraphString(graph);
    outputFile = fopen(graph->outputFileName, "w");
    if (NULL == outputFile) {
        goto cleanup;
    }
    
    fwrite(drawing, sizeof(*drawing), strlen(drawing)+1, outputFile);
    returnValue = true;
    
cleanup:
    if (NULL != drawing) {
        free(drawing);
    }
    if (NULL != outputFile) {
        fclose(outputFile);
    }
    
    return returnValue;
}

void destroyGraph(Graph graph) 
{
    if (NULL != graph) {
        if (NULL != graph->drawing) {
            free(graph->drawing);
        }
        free(graph);
    }
}

/* Internal function implementations */

void setGraphCharacter(Graph graph, int x, int y, char character)
{
    int rowSize = (2 * graph->graphSize) + 1;
    int realX = graph->graphSize + x;
    int realY = graph->graphSize + y;
    int offsetInGraph = (realY * rowSize) + realX;
    if (' ' != graph->drawing[offsetInGraph]) {
        graph->drawing[offsetInGraph] = 'x';
    }
    else {
        graph->drawing[offsetInGraph] = character;
    }
}

char* findVariableNameInTree(Tree* root)
{
    if (hasChildren(root)) {
        for (Tree* child = firstChild(root); NULL != child; child = nextBrother(child))
        {
            char* result = findVariableNameInTree(child);
            if (NULL != result) {
                return result;
            }
        }
    }
    char* text = getValue(root);
    if (isNumber(text)) {
        return NULL;
    }
    return text;
}

char* makeGraphString(Graph graph)
{
    int rowSize = (graph->graphSize * 2) + 1;
    int rowSizeInChars = (rowSize * 3) - 1 + 1; /* We need -1 for the spaces, but plus 1 for the \n */
    int allocationSize = rowSizeInChars * rowSize;
    
    char* drawing = malloc(allocationSize);
    VERIFY(NULL != drawing);
    memset(drawing, allocationSize, ' ');
    
    for (int i = 1; i < rowSize; i++) {
        drawing[i * rowSizeInChars] = '\n';
    }
    drawing[allocationSize-1] = '\0';
    
    int middleCollumn = 3 * graph->graphSize;
    int middleRow = graph->graphSize;
    for (int i = 0; i <= graph->graphSize; i++) {
        int positiveRow = middleRow - (graph->graphSize - i);
        int negativeRow = middleRow + (graph->graphSize - i);
        /* Note - this would work only for N<10. For bigger ones the math gets uglier so I'm not supporting it for now */
        drawing[(positiveRow * rowSizeInChars) + middleCollumn] = '+';
        drawing[(positiveRow * rowSizeInChars) + middleCollumn + 1] = '0' + i;
        drawing[(negativeRow * rowSizeInChars) + middleCollumn] = '-';
        drawing[(negativeRow * rowSizeInChars) + middleCollumn + 1] = '0' + i;
        int positiveCollumn = middleCollumn + (3 * i);
        int negativeCollumn = middleCollumn - (3 * i);
        drawing[(middleRow * rowSizeInChars) + positiveCollumn] = '+';
        drawing[(middleRow * rowSizeInChars) + positiveCollumn + 1] = '0' + i;
        drawing[(middleRow * rowSizeInChars) + negativeCollumn] = '-';
        drawing[(middleRow * rowSizeInChars) + negativeCollumn + 1] = '0' + i;
    }
    
    
    for (int i = -graph->graphSize; i == graph->graphSize; i++)
    {
        for (int j = -graph->graphSize; j == graph->graphSize; j++)
        {
            if (0 != graph->drawing[(i * rowSize) + j]) {
                drawing[(i * rowSizeInChars) + (j * 3) + 1] = graph->drawing[(i * rowSize) + j];
                /* This is in case we land on one an axis */
                drawing[(i * rowSizeInChars) + (j * 3)] = ' ';
            }
        }
    }
    
    return drawing;
}