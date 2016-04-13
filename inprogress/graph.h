#ifndef _GRAPH_
#define _GRAPH_

#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

typedef struct graph graph;
typedef struct graph_node vertex;
typedef struct adjlist edge;

struct graph {
	unsigned size;
	vertex *v;
};

struct graph_node {
	unsigned id;
	unsigned indeg;
	unsigned outdeg;
	unsigned heapIdx;
	int dist;
	edge *edge;
};

struct adjlist {
	unsigned vertex;
	int weight;
	edge *next;
};

graph *initEmptyGraph(unsigned n);
unsigned graphSize(graph *g);
vertex *graphVertex(graph *g, unsigned v);

int Undirected_Unweighted_Edge(graph *g, unsigned i, unsigned j);
int Directed_Unweighted_Edge(graph *g, unsigned i, unsigned j);
int Undirected_Weighted_Edge(graph *g, unsigned i, unsigned j, int weight);
int Directed_Weighted_Edge(graph *g, unsigned i, unsigned j, int weight);
int checkForDirectedEdge(graph *g, unsigned i, unsigned j);
int checkForUndirectedEdge(graph *g, unsigned i, unsigned j);

#endif /* _GRAPH_ */





