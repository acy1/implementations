#include "boundedqueue.h"

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

struct bddintqueue {
	unsigned size;
	unsigned bound;
	unsigned in;
	unsigned out;
	unsigned *q;
};

bq *bqinit(unsigned size) {
	bq *q = malloc(sizeof(bq));
	if(q && size < ((unsigned) -1)) {
		q->size = 0;
		q->bound = size;
		q->in = 0;
		q->out = 0;
		q->q = malloc(sizeof(unsigned)*size);
		if(!q->q) {
			free(q);
			return NULL;
		}
		return q;
	} 
	free(q);
	return NULL;
}

unsigned bqsize(bq *q) {
	if(q) {
		return q->size;
	}
	return ((unsigned) -1);
}
unsigned bqpeek(bq *q) {
	if(q->size) {
		return q->q[q->out];
	}
	return ((unsigned) -1);
}

unsigned bqdequeue(bq *q) {
	if(q->size > 0) {
		q->size--;
		unsigned ret = q->q[q->out];
		q->out++;
		return ret;
	}
	return ((unsigned) -1);
}

int bqenqueue(bq *q, unsigned i) {
	if(q->size == q->bound) {
		return FALSE;
	}

	q->q[q->in] = i;
	q->in = (q->in + 1) % q->bound;
	q->size++;
	return TRUE;
}

void bqprint(bq *q) {
	unsigned i = q->out;
	unsigned size = q->size;
	while(size) {
		printf("%u ", q->q[i]);
		i = (i+1) % q->bound;
		size--;
	}
	printf("\n");
}


