#include "graph.h"
#include "heap.h"

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef INFTY
#define INFTY (~(1<<31))
#endif

#define conv(arg1) ((void*) &(g->v[arg1]))

#define extractMin(arg1) ((vertex *) heappop(arg1))


int vertexComp(void *a_, void *b_) {
	vertex *a = (vertex*) a_;
	vertex *b = (vertex*) b_;
	if(a->dist < b->dist) {
		return -1;
	} else if (a->dist == b->dist) {
		return 0;
	} else {
		return 1;
	}
}

int setIndex(void *a_, unsigned idx) {
	if(!a_) {
		return FALSE;
	}
	vertex *a = (vertex*) a_;
	a->heapIdx = idx;
	return TRUE;
}

void djikstra(graph *g, unsigned root) {

	if(root >= g->size) {
		return;
	}
	unsigned i;

	unsigned *pred = malloc(sizeof(unsigned)*g->size);
	heap *h = heapinit(g->size, vertexComp, setIndex);
	vertex *cur;
	pred[root] = root;
	g->v[root].dist = 0;
	heapinsert(h, conv(root));

	for(i = 0; i < g->size; i++) {
		if(i != root) {
			g->v[i].dist = INFTY;
			pred[i] = INFTY;
			heapinsert(h, conv(i));
		}
	}

	while(heapsize(h)) {
		cur = extractMin(h);
		edge *iter = cur->edge;
		while(iter) {
			if(cur->dist + iter->weight < graphVertex(g, iter->vertex)->dist) {
				graphVertex(g, iter->vertex)->dist = cur->dist + iter->weight;
				heapupdateexternal(h, graphVertex(g, iter->vertex)->heapIdx);
				pred[iter->vertex] = cur->id;
			}
			iter = iter->next;
		}
	}

	for(i = 0; i < g->size; i++) {
		printf("(%u) pred: %u dist: %d\n", i, pred[i], g->v[i].dist);
	}
}

int main() {
	graph *g = initEmptyGraph(6);

	Undirected_Weighted_Edge(g, 1, 2, 1);
	Undirected_Weighted_Edge(g, 1, 3, 3);
	Undirected_Weighted_Edge(g, 2, 3, 1);
	Undirected_Weighted_Edge(g, 2, 4, 2);
	Undirected_Weighted_Edge(g, 4, 5, 1);
	Undirected_Weighted_Edge(g, 1, 5, 5);

	djikstra(g, 1);

	Undirected_Unweighted_Edge(g, 0, 1);
	Undirected_Unweighted_Edge(g, 0, 5);

	djikstra(g, 1);

	//djikstra(g, 7);

}