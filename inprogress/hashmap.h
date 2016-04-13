#include <stdlib.h>

typedef struct hashmap hmap;

hmap *hinit(int (*comp)(void *a, void*b), unsigned int (*hashfnc)(void *data));
int hinsert(hmap *h, void *data);
int hdelete(hmap *h, void *data);

