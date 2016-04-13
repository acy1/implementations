#include "mergesort.h"

/* 
 * DESRCIPTION: 
 * Copy SIZE number of bytes from SRC to DST
 * SRC and DST must have at least SIZE bytes, else
 * this function will have undefined behavior. 
 *
 * SRC  - pointer to first byte to be copied     
 * DST  - pointer to first destination byte
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
 * DESCRIPTION: 
 * Merge two sorted arrays sorted into DST array.
 * ARR1 and ARR2 have LEN1 and LEN2 items respectively,
 * Each item is SIZE many bytes. 
 * Sorting is with respect to the COMPAR function.
 * ARR1 must have at least LEN1*SIZE bytes, and
 * ARR2 must have at least LEN2*SIZE bytes, and
 * DST must have at least (LEN1 + LEN2)*SIZE bytes, else
 * this function will have undefined behavior.
 * 
 * ARR1 - pointer to first byte of first array
 * LEN1 - number of items in ARR1
 * ARR2 - pointer to first byte of second array
 * LEN2 - number of items in ARR2
 * SIZE - size of each item in bytes
 * DST  - array which receives merged values
 * COMP - comparison function providing ordering on items,
 *        see msort description for details
 */
inline static void
merge(void *arr1, size_t len1, void *arr2, size_t len2, 
      size_t size, void *dst, int (*comp)(const void*a, const void*b))
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

/* 
 * A bottom-up implementation of mergesort using O(num) additional space.
 * An array BASE of NUM items each of SIZE many bytes is sorted
 * with respect to the COMP function.
 * BASE must have at least NUM*SIZE bytes else behavior is undefined.
 * COMP returns -1 if the element pointed to by A should come before B,
 * 0 if A and B are equal with respect to the ordering, or 1 if B should
 * come before A.
 * 
 * RETURN VALUES:
 * MERGE_ERR_NO_MEM if memory allocation fails, else MERGE_COMPLETE.
 * 
 * INPUTS:
 * BASE - array to be sorted
 * NUM  - number of items to be sorted
 * SIZE - size of each item in BASE, in bytes
 * COMP - comparison function that defines a linear ordering on the items in BASE
 *
 * USAGE EXAMPLE:
 * int len = 20;
 * int *b = malloc(sizeof(int)*len);
 * // populate b 
 * msort(b, len, sizeof(int), intcomp);
 *
 * where intcomp may look like:
 * 
 * int intcomp(const void *a, const void *b)
 * { return (*((int*)a) <= *((int*)b)) ? -1 : 1; }
 * 
 * NOTES:
 *
 * This specific implementation merges subarrays of exponentially increasing size
 * into a work array. The role of the input array and work array are flipped each
 * iteration of the primary while loop. This means that the work array must be copied into
 * the input array prior to being freed if the number of iterations is odd.
 */
int
msort(void *base, size_t num, size_t size, int (*comp)(const void*a, const void*b))
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
      m -= (len1 = ((m < sub_array_size) ? m : sub_array_size));
      m -= (len2 = ((m < sub_array_size) ? m : sub_array_size));
      merge(cur, len1, cur + (size * len1), len2, size, work, comp);
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