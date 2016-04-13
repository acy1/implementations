#include "boundedqueue.h"
#include "graph.h"

void bfs(graph *g, unsigned root) {
	if(!g || root >= g->size) {
		return;
	}

	int *dist = malloc(sizeof(int)*g->size);
	unsigned *pred = malloc(sizeof(unsigned)*g->size);
	int i;
	for(i = 0; i < g->size; i++) {
		dist[i] = -1;
		pred[i] = g->size;
	}
	
	bq *q = bqinit(g->size);

	if(!q) {
		return;
	}

	unsigned cur;
	dist[root] = 0;
	pred[root] = root;

	bqenqueue(q, root);

	while(bqsize(q) > 0) {
		cur = bqdequeue(q);
		nbr *iter = g->v[cur].nbr;
		while(iter) {
			if(dist[iter->vertex] < 0) {
				bqenqueue(q, iter->vertex);
				dist[iter->vertex] = dist[cur] + 1;
				pred[iter->vertex] = cur;
			}
			iter = iter->next;
		}
	}

	for(i = 0; i < g->size; i++) {
		printf("V: %d Dist: %d Pred: %u\n", i, dist[i], pred[i]);
	}
	printf("\n");
	free(dist);
	free(pred);
}


int main() {
	graph *g = initEmptyGraph(11);

	Undirected_Unweighted_Edge(g, 1, 2);
	Undirected_Unweighted_Edge(g, 1, 3);
	Undirected_Unweighted_Edge(g, 2, 4);
	Undirected_Unweighted_Edge(g, 2, 5);
	Undirected_Unweighted_Edge(g, 2, 8);
	Undirected_Unweighted_Edge(g, 3, 6);
	Undirected_Unweighted_Edge(g, 3, 7);
	Undirected_Unweighted_Edge(g, 10, 3);
	Undirected_Unweighted_Edge(g, 8, 6);
	Undirected_Unweighted_Edge(g, 4, 9);
	Undirected_Unweighted_Edge(g, 9, 8);
	Undirected_Unweighted_Edge(g, 10, 7);

	bfs(g, 1);

	Undirected_Unweighted_Edge(g, 0, 9);

	bfs(g, 1);

	bfs(g, 7);

}