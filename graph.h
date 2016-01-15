
#pragma once

#include "common.h"
#include "tree.h"

/* TODO - Doc */
typedef struct Graph_t * Graph;

Graph makeGraph(const char * outputFileName, int graphSize);

void addFunctionToGraph(Graph graph, Tree* function);

bool drawGraph(Graph graph);

void destroyGraph(Graph graph);