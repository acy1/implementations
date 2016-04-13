#include "stack.h"

struct stackelem {
  struct stackelem *prev;
  void *data;
};

struct stack {
  unsigned int size;
  struct stackelem *top;
};

struct queue {
  unsigned int size;
  struct stack *in;
  struct stack *out;
};

stack *
sinit(void)
{
  stack *s = NULL;
  if((s = malloc(sizeof(stack)))) {
    s->size = 0;
    s->top = NULL;
  }
  return s;
}

void *
spop(stack *s)
{
  void *ret = NULL;
  if(s && s->size) {
    struct stackelem *node = s->top;
    ret = node->data;
    s->top = node->prev;
    s->size--;
    free(node);
  }
  return ret;
}

int 
spush(stack *s, void *data)
{
  struct stackelem *node;
  if(s && (node = malloc(sizeof(struct stackelem)))) {
    node->data = data;
    node->prev = s->top;
    s->top = node;
    s->size++;
    return 1;
  }
  return 0;
}

void 
sdestroy(stack *s, int freedata)
{
  if(s) {
    while(s->size) {
      void *data = spop(s);
      if(freedata && data) {
        free(data);
      }
    }
    free(s);
  }
}

int
sprint(stack *s, void (*printfunc)(void *))
{
  if(s) {
    struct stackelem *iter = s->top;
    while(iter) {
      printfunc(iter->data);
      iter = iter->prev;
    }
    return 1;
  }
  return 0;
}

queue *
qinit(void)
{
  queue *q = NULL;
  if((q = malloc(sizeof(queue)))) {
    q->in = sinit();
    q->out = sinit();
    q->size = 0;
    if(q->in && q->out) {
      return q;
    } else {
      sdestroy(q->in, 0);
      sdestroy(q->out, 0);
      free(q);
      return NULL;
    }
  }
  return NULL;
}

int
qpush(queue *q, void *data)
{
  if(q && spush(q->in, data)) {
    q->size++;
    return 1;
  }
  return 0;
}

void *
qpop(queue *q)
{
  if(q && q->out->size) {
    q->size--;
    return spop(q->out);
  }
  if (q && q->in->size) {
    while(q->in->size > 1) {
      void *data = spop(q->in);
      spush(q->out, data);
    }
    q->size--;
    return spop(q->in);
  }
  return NULL;
}

void
qdestroy(queue *q, int freedata)
{
  if(q) {
    sdestroy(q->in, freedata);
    sdestroy(q->out, freedata);
    free(q);
  }
}

int
qprint(queue *q, void (*printfunc)(void *))
{
  if (q) {
    struct stackelem *iter;
    stack *temp;

    if(q->in->size) {
      if((temp = sinit())) {
        sprint(q->out, printfunc);
        iter = q->in->top;
        while(iter) {
          spush(temp, iter->data);
          iter = iter->prev;
        }
        while(temp->size) {
          printfunc(spop(temp));
        }
        sdestroy(temp, 0);
        return 1;
      } else {
        return 0;
      }

    } else {
      return sprint(q->out, printfunc);
    }
  }
  return 0;
}

int main(int argc, char *argv[]) {
  return 0;
}


