#include <stdlib.h>

typedef struct stack stack;

typedef struct queue queue;

stack *sinit(void);
void *spop(stack *s);
int spush(stack *s, void *data);
void sdestroy(stack *s, int freedata);

queue *qinit(void);
void *qpop(queue *q);
int qpush(queue *q, void *data);
void qdestroy(queue *q, int freedata);

