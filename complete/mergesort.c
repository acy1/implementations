#include "mergesort.h"

/* 
 * Copy SIZE number of bytes from SRC to DST
 *
 * SRC - pointer to first byte to be copied     
 * DST - pointer to first destination byte
 * SIZE - number of bytes to copy
 */  
inline static void 
copy(uint8_t *src, uint8_t *dst, size_t size)
{
  while(size--) {
    *dst++ = *src++;
  }
}
/*
 * Merge two sorted arrays sorted into 
 *
 *
 *
 *
 *
 *    
 */
inline static void
merge(void *arr1, size_t len1, void *arr2, size_t len2, 
      size_t size, void *dst, int (*compar)(const void*a, const void*b))
{
  while(len1 && len2) {
    if(compar(arr1, arr2) <= 0) {
      copy(arr1, dst, size);
      arr1 += size;
      dst += size;
      len1--;
    } else {
      copy(arr2, dst, size);
      arr2 += size;
      dst += size;
      len2--;
    }
  }

  while(len1--) {
    copy(arr1, dst, size);
    arr1 += size;
    dst += size;
  }

  while(len2--) {
    copy(arr2, dst, size);
    arr2 += size;
    dst += size;
  }
}

int
msort(void *base, size_t num, size_t size, int (*compar)(const void*a, const void*b))
{
  void *cur, *work, *workarray;
  size_t m, len1, len2, counter, sub_array_size;

  workarray = malloc(size*num);
  if(!workarray) {
    return MERGE_ERR_NO_MEM;
  }

  sub_array_size = 1;
  counter = 0;

  while(sub_array_size < num)
  {
    m = num;
    cur = counter ? workarray : base;
    work = counter ? base : workarray;

    while(m)
    {
      len1 = (m < sub_array_size) ? m : sub_array_size;
      m -= len1;
      len2 = (m < sub_array_size) ? m : sub_array_size;
      m -= len2;
      merge(cur, len1, cur + (size * len1), len2, size, work, compar);
      cur += (size * (len1 + len2));
      work += (size * (len1 + len2));
    }
    counter = ~counter;
    sub_array_size *= 2;
  }

  if(counter) {
    copy(workarray, base, size*num);
  }
  free(workarray);
  return MERGE_COMPLETE;
}