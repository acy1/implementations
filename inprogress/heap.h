#ifndef _HEAP_
#define _HEAP_

#include <stdlib.h>
#include <stdio.h>

typedef struct heap heap;

heap *heapinit(unsigned bound, int (*comp)(void *, void *), 
		int (*setIndex)(void *, unsigned));
void heapinsert(heap *h, void *data);
void *heappop(heap *h);
void heapupdateexternal(heap *h, unsigned idx);
unsigned heapsize(heap *h);

#endif