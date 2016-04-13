#include "graph.h"

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef INFTY
#define INFTY (~(1<<31))
#endif

graph *initEmptyGraph(unsigned n);
int addDirectedEdge_(graph *g, unsigned i, unsigned j, int weight);
int addUndirectedEdge_(graph *g, unsigned i, unsigned j, int weight);
int checkForDirectedEdge_(graph *g, unsigned i, unsigned j);
int checkForUndirectedEdge_(graph *g, unsigned i, unsigned j);

/*  Initialize a completely disconnected graph 
	with n nodes */
graph *initEmptyGraph(unsigned n) {
	graph *g = malloc(sizeof(graph));
	if(g) {
		g->size = n;
		g->v = malloc(sizeof(vertex)*n);
		if(!g->v) {
			free(g);
			return NULL;
		}
		unsigned i;
		for(i = 0; i < n; i++) {
			g->v[i].id = i;
			g->v[i].indeg = 0;
			g->v[i].outdeg = 0;
			g->v[i].heapIdx = INFTY;
			g->v[i].dist = INFTY;
			g->v[i].edge = NULL;
		}
	}
	return g;
}

unsigned graphSize(graph *g) {
	if(g) {
		return g->size;
	}
	return 0;
}

vertex *graphVertex(graph *g, unsigned v) {
	if(v < g->size) {
		return &g->v[v];
	}
	return NULL;
}

/* recursive implementation rather than */
edge *removeFromAdjList_(edge *adj, unsigned i) {
	edge *ret, *prev, *next;
  if(!adj) {
    return NULL;
  }
  if(adj && adj->vertex == i) {
    ret = adj->next;
    free(adj);
    return ret;
  }

  ret = adj;
  prev = adj;
  adj = adj->next;

  while(adj) {
    next = adj->next;
    if(adj->vertex == i) {
      prev->next = next;
      free(adj);
      break;
    }
    prev = adj;
    adj = next;
	}
	return ret;
}

edge *searchAdjList_(edge *adj, unsigned j) {
	while(adj) {
		if(adj->vertex == j) {
			return adj;
		} else {
			adj = adj->next;
		}
	}
	return NULL;
}

void removeDirectedEdge_(graph *g, unsigned i, unsigned j) {
	if(i < g->size && j < g->size) {
		g->v[i].edge = removeFromAdjList_(g->v[i].edge, j);
	}
}

void removeUndirectedEdge_(graph *g, unsigned i, unsigned j) {
	removeDirectedEdge_(g, i, j);
	removeDirectedEdge_(g, j, i);
}

int addDirectedEdge_(graph *g, unsigned i, unsigned j, int weight) {
	if(i < g->size && j < g->size) {
		edge *list = g->v[i].edge;
		edge *elem = malloc(sizeof(edge));
		if(elem) {
			elem->vertex = j;
			elem->weight = weight;
			elem->next = list;
			g->v[i].edge = elem;
			g->v[i].outdeg++;
			g->v[j].indeg++;
			return TRUE;
		}
		return FALSE;
	} else {
		return FALSE;
	}
}

int addUndirectedEdge_(graph *g, unsigned i, unsigned j, int weight) {
	int ret = addDirectedEdge_(g, i, j, weight);
	if(ret) {
		ret &= addDirectedEdge_(g, j, i, weight);
		if(!ret) {
			removeDirectedEdge_(g, i, j);
		}
	}
	return ret;
}

int checkForDirectedEdge(graph *g, unsigned i, unsigned j) {
	if(i < g->size && j < g->size && searchAdjList_(g->v[i].edge, j)) {
		return TRUE;
	} 
	return FALSE;
}

int checkForUndirectedEdge(graph *g, unsigned i, unsigned j) {
	if(i < g->size && j < g->size) {
		return ((searchAdjList_(g->v[i].edge, j) && searchAdjList_(g->v[j].edge, i)) ? TRUE : FALSE);
	}
	return FALSE;
}

int Undirected_Unweighted_Edge(graph *g, unsigned i, unsigned j) {
	if(!checkForUndirectedEdge(g, i, j)) {
		return addUndirectedEdge_(g, i, j, 1);
	}
	return FALSE;
}

int Directed_Unweighted_Edge(graph *g, unsigned i, unsigned j) {
	if(!checkForDirectedEdge(g, i, j)) {
		return addDirectedEdge_(g, i, j, 1);
	}
	return FALSE;
}

int Undirected_Weighted_Edge(graph *g, unsigned i, unsigned j, int weight) {
	if(!checkForUndirectedEdge(g, i, j)) {
		return addUndirectedEdge_(g, i, j, weight);
	}
	return FALSE;
}

int Directed_Weighted_Edge(graph *g, unsigned i, unsigned j, int weight) {
	if(!checkForDirectedEdge(g, i, j)) {
		return addDirectedEdge_(g, i, j, weight);
	}
	return FALSE;
}




