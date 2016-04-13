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

struct heap {
	unsigned bound;
	unsigned size;
	int (*comp)(void *, void*);
	int (*setIdx)(void *, unsigned);
	void **arr;
};

#define swap(a, b) {\
	(a) ^= (b); \
	(b) ^= (a); \
	(a) ^= (b); \
}

/* requires a tempswap pointer */
#define swapvoid(a, b) {\
	tempswap = (a); \
	(a) = (b); \
	(b) = tempswap; \
}

#define HFACTOR 4
#define parent(i) (((i)-1)/HFACTOR)
#define firstchild(i) ((HFACTOR*(i))+1)



heap *heapinit(unsigned bound, int (*comp)(void *, void *), 
		int (*setIndex)(void *, unsigned)) 
{
	heap *h = malloc(sizeof(heap));
	if(h) {
		h->bound = bound; //guaranteed to be nonzero
		h->size = 0;
		h->comp = comp;
		h->setIdx = setIndex;
		h->arr = malloc(sizeof(void*)*bound);
		if(!h->arr) {
			free(h);
			return NULL;
		}
	}
	return h;
}

void heapinsert(heap *h, void *data) {

	if(!data) {
		return;
	}

	if(h->size == h->bound) {
		h->arr = realloc(h->arr, 2*(h->bound+1)*sizeof(void*));
		h->bound = 2*(h->bound + 1);
	}

	h->arr[h->size] = data;
	h->setIdx(data, h->size);

	unsigned cur = h->size;
	unsigned par = parent(cur);
	void **a = h->arr;
	void *tempswap;

	/* bubble up */
	while(cur > 0 && h->comp(a[cur], a[par]) < 0) {
		swapvoid(a[cur], a[par]);
		h->setIdx(a[cur], cur);
		h->setIdx(a[par], par);
		swap(par, cur);
		par = parent(cur);
	}
	h->size++;
}

void *heappop(heap *h) {
	if(h->size == 0) {
		return NULL;
	}

	void *ret = h->arr[0];
	void *tempswap;
	h->setIdx(ret, INFTY);
	h->size--;

	/* size was decremented to be accurate */
	if(h->size ==  0) {
		return ret;
	}

	void **a = h->arr;
	swapvoid(a[0], a[h->size]);
	h->setIdx(a[0], 0);

	unsigned cur = 0;
	unsigned prioritize, i;

	/* bubble down */
	do {
	 	prioritize = cur;

	 	/* select child to swap with, if any is of higher priority */
	 	for(i = firstchild(cur); i < (firstchild(cur) + HFACTOR) && i < h->size; i++) {
	 		if(h->comp(a[i], a[prioritize]) < 0) {
	 			prioritize = i;
	 		}
	 	}
	 	if(cur != prioritize) {
	 		swapvoid(a[cur], a[prioritize]);
	 		h->setIdx(a[cur], cur);
	 		h->setIdx(a[prioritize], prioritize);
	 		swap(cur, prioritize);
	 	}
	} while(cur != prioritize);

	return ret;
}

/* update the heap after external priority change to node at index idx */
void heapupdateexternal(heap *h, unsigned idx) {	
	if(idx >= h->size) {
		return;
	}

	unsigned cur = idx;
	unsigned prioritize, i, par;
	void **a = h->arr;
	void *tempswap;

	/* bubble down */
	do {
	 	prioritize = cur;

	 	/* select child to swap with, if any is of higher priority */
	 	for(i = firstchild(cur); i < (firstchild(cur) + HFACTOR) && i < h->size; i++) {
	 		if(h->comp(a[i], a[prioritize]) < 0) {
	 			prioritize = i;
	 		}
	 	}
	 	if(cur != prioritize) {
	 		swapvoid(a[cur], a[prioritize]);
	 		h->setIdx(a[cur], cur);
	 		h->setIdx(a[prioritize], prioritize);
	 		swap(cur, prioritize);
	 	}
	} while(cur != prioritize);

	/* bubble up */
	par = parent(cur);
	while(cur > 0 && h->comp(a[cur], a[par]) < 0) {
		swapvoid(a[cur], a[par]);
		h->setIdx(a[cur], cur);
		h->setIdx(a[par], par);
		swap(par, cur);
		par = parent(cur);
	}
}

int assertHeap(heap *h) {
	unsigned i, j;
	/* check that each elem has higher priority than all it's children */
	for(i = 0; i < h->size; i++) {
		for(j = firstchild(i); j < (firstchild(i) + HFACTOR) && j < h->size; j++) {
			if(h->comp(h->arr[i], h->arr[j]) > 0) {
				return FALSE;
			}
		}
	}
	return TRUE;
}

unsigned heapsize(heap *h) {
	if(!h) {
		return 0;
	} else {
		return h->size;
	}
}