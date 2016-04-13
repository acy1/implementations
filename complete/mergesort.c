#include "mergesort.h"


inline static void copy(uint8_t *src, uint8_t *dst, size_t size);
inline static void merge(uint8_t *arr1, size_t len1, uint8_t *arr2, size_t len2, 
      size_t size, uint8_t *dst, int (*comp)(uint8_t *a, uint8_t *b));

/* 
  Copy SIZE number of bytes from SRC to DST
  SRC - pointer to first byte to be copied. 
      
  dst: pointer to first destination byte.

 */  
inline static void 
copy(uint8_t *src, uint8_t *dst, size_t size)
{
  while(size--) {
    *dst++ = *src++;
  }
}

inline static void
merge(uint8_t *arr1, size_t len1, uint8_t *arr2, size_t len2, 
      size_t size, uint8_t *dst, int (*comp)(uint8_t *a, uint8_t *b))
{
  while(len1 && len2) {
    if(comp(arr1, arr2) <= 0) {
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

void 
msort(void *arr, size_t n, size_t size, int (*comp)(uint8_t*a, uint8_t*b)) 
{
  uint8_t *cur, *work, *workarray;
  size_t m, len1, len2, counter, sub_array_size;

  workarray = malloc(size*n);
  sub_array_size = 1;
  counter = 0;

  while(sub_array_size < n)
  {
    m = n;
    cur = counter ? workarray : arr;
    work = counter ? arr : workarray;

    while(m)
    {
      len1 = (m < sub_array_size) ? m : sub_array_size;
      m -= len1;
      len2 = (m < sub_array_size) ? m : sub_array_size;
      m -= len2;
      merge(cur, len1, cur + (size * len1), len2, size, work, comp);
      cur += (size * (len1 + len2));
      work += (size * (len1 + len2));
    }
    counter = ~counter;
    sub_array_size *= 2;
  }

  if(counter) {
    copy(workarray, arr, size*n);
  }

  free(workarray);
}