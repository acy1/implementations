#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define TESTLENBOUND 8000000
#define TESTNUM 100
#define TESTLEN /* 1 << 20 */ 11

static inline void
intmerge(int *a_, size_t len1_, int *b_, size_t len2_, int *dst_)
{
  int *a = a_;
  int *b = b_;
  int *dst = dst_;
  size_t len1 = len1_;
  size_t len2 = len2_;

  while(len1 && len2) {
    if(*a < *b) {
      *dst++ = *a++;
      len1--;
    } else {
      *dst++ = *b++;
      len2--;
    }
  }

  while(len1) {
    *dst++ = *a++;
    len1--;
  }

  while(len2) {
    *dst++ = *b++;
    len2--;
  }
}

void 
intmsort(int *arr_, size_t n) 
{

  int *arr = arr_;
  int *workarray = malloc(sizeof(int) * n);
 
  int *current_ = arr;
  int *work_ = workarray;
  int *swap, *first, *second, *cur, *work;
  size_t sub_array_size = 1;
  size_t counter = 0;
  size_t m, len1, len2;

  while(sub_array_size <= n)
  {
    m = n;
    cur = current_;
    work = work_;

    while(m)
    {
      first = cur;
      len1 = (m < sub_array_size) ? m : sub_array_size;
      m -= len1;
      second = cur + len1;
      len2 = (m < sub_array_size) ? m : sub_array_size;
      m -= len2;
      intmerge(first, len1, second, len2, work);
      cur += (len1 + len2);
      work += (len1 + len2);
    }

    /*
    for(int i = 0; i < n; i++) {
      printf("%d ", work_[i]);
      if(!(i % (2*sub_array_size))) {
        printf(" ");
      }
    }
    printf("\n"); */

    swap = current_;
    current_ = work_;
    work_ = swap;
    counter++;
    sub_array_size *= 2;
  }

  /* if the work array has the final result, swap the two */
  if(counter % 2) {
    m = n;
    while(m) {
      arr[m-1] = workarray[m-1];
      m--;
    }
  }

  free(workarray);
}

int test(int num, int len)
{
  int i;
  
  int *a = malloc(sizeof(int) * len);

  for(i = 0; i < len; i++) {
    a[i] = (rand() + i) % 100;
    //printf("%d ", a[i]);
  }
  //printf("\n\n");

  intmsort((void*) a, len);

  for(i = 0; i < len - 1; i++) {
    if(a[i] > a[i+1]) {
      printf("Test %d failed at index %d with array length %d\n%d > %d\n", num, i, len, a[i], a[i+1]);
      free(a);
      return 0;
    }
  }

  free(a);
  return 1;
}

void bulktester1()
{
  int testcount;
  int correctcount = 0;

  time_t t;
  srand((unsigned) time(&t));

  for(testcount = 1; testcount <= TESTNUM; testcount++) {
    if(test(testcount, TESTLEN)) {
      correctcount++;
      printf("Test %d on length %d correct\n", testcount, TESTLEN);
    } 
  }

  if(correctcount+1 < testcount) {
    printf("Ran %d tests but only %d were correctly sorted.\n", testcount-1, correctcount);
  } else {
    printf("All %d tests were correctly sorted.\n", testcount-1);
  }
}

void bulktester2()
{
  int i;

  int testcount = 0;
  int correctcount = 0;
  int testlen = 64;
  int constant = 5;

  time_t t;
  srand((unsigned) time(&t));

  while(testlen < TESTLENBOUND) {
    for(i = 0; i < TESTNUM; i++) {
      testcount++;
      if(test(i, testlen+constant)) {
        correctcount++;
        printf("Test %d on length %d correct\n", i+1, testlen+constant);
      } 
    }
    testlen <<= 1;
  }

  if(correctcount < testcount) {
    printf("Ran %d tests but only %d were correctly sorted.\n", testcount, correctcount);
  } else {
    printf("All %d tests were correctly sorted.\n", testcount);
  }
}

int main(int argc, char *argv[])
{
  //bulktester1();
  bulktester2();
  return 0;
}

