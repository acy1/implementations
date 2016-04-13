#include "hashmap.h"

#define DEFAULTBUCKETS 8
#define DEFAULTBUCKETSIZE 4

struct hashmap {
  unsigned int numb;
  unsigned int load;
  unsigned int bsize;
  int (*comp)(void *a, void *b);
  unsigned int (*hash)(void *data);
  struct bucket **buckets;
};	

struct bucket {
  int size;
  void **data;
};

typedef struct bucket bucket;

void hresize(hmap *h);
int hinsert(hmap *h, void *data);

bucket *
binit()
{
  struct bucket *b = malloc(sizeof(bucket));
  if(b) {
    b->size = 0;
    b->data = NULL;
  }
  return b;
}

/* 
  This function only called when the bucket has room for insertion
*/
int 
binsert(bucket *b, void *data)
{
  int i;
  void **old = b->data;
  b->data = malloc(sizeof(void*) * (b->size + 1));
  if(b->data) {
    for(i = 0; i < b->size; i++) {
      b->data[i] = old[i];
    }
    b->data[b->size] = data;
    b->size++;
    return 1;
  } else {
    b->data = old;
    return 0;
  }
}

hmap *
hinit(int (*comp)(void *a, void*b), unsigned int (*hash)(void *data))
{
  int i,j;
  hmap *h = malloc(sizeof(hmap));
  if(h) {
    h->numb = DEFAULTBUCKETS;
    h->load = 0;
    h->bsize = DEFAULTBUCKETSIZE;
    h->comp = comp;
    h->hash = hash;
    h->buckets = malloc(sizeof(bucket*) * DEFAULTBUCKETS);
    for(i = 0; i < DEFAULTBUCKETS; i++) {
      if(!(h->buckets[i] = binit())) {
        goto fail;
      }
    }
  } 
  return h;

  fail:
  for(j = 0; j < i; j++) {
    free(h->buckets[j]);
  }
  free(h->buckets);
  free(h);
  return NULL;
}

int 
hinsert(hmap *h, void *data)
{
  if(h && data) {
    unsigned int slot = h->hash(data) % h->numb;
    if(h->buckets[slot]->size < h->bsize) {
      return binsert(h->buckets[slot], data);
    } else {
      hresize(h);
      return hinsert(h, data);
    }
  }
  return 0;
}

int
hdelete(hmap *h, void *data)
{
  return 0;
}

void
hresize(hmap *h) 
{

}



int 
main(int argc, char *argv[])
{
  return 0;
}


