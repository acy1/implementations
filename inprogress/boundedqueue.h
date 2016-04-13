#ifndef _BDDQUEUE_
#define _BDDQUEUE_

#include <stdlib.h>
#include <stdio.h>

typedef struct bddintqueue bq;

bq *bqinit(unsigned size);
unsigned bqsize(bq *q);
unsigned bqpeek(bq *q);
unsigned bqdequeue(bq *q);
int bqenqueue(bq *q, unsigned i);
void bqprint(bq *q);

#endif /* _BDDQUEUE_ */