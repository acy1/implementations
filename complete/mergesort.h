#ifndef  _MSORT_
#define _MSORT_

#include <stdlib.h>
#include <stdio.h>

int msort(void *base, size_t num, size_t size, int (*compar)(const void*a, const void*b));

typedef enum {
	MERGE_ERR_NO_MEM,
	MERGE_COMPLETE
} msort_return_codes;

#endif /* _MSORT_ */
