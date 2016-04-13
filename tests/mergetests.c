#include "../complete/mergesort.h"
#include <time.h>

#define TESTLENBOUND 600000
#define TESTLEN 100000
#define TESTNUM 100
#define CONSTANT 7

int intcomp(const void *a_, const void *b_)
{
  int *a = (int*) a_;
  int *b = (int*) b_;
  if(*a < *b) {
    return -1;
  } else if(*a > *b) {
    return 1;
  } else {
    return 0;
  }
}

int test_and_verify_permutations(int num, unsigned len)
{
  int i;
  time_t t;
  srand((unsigned) time(&t) + num);
  
  unsigned *a = malloc(sizeof(unsigned) * len);
  unsigned *precount = calloc(2*len, sizeof(unsigned));

  for(i = 0; i < len; i++) {
    a[i] = ((unsigned) rand()) % (2*len);
    precount[a[i]]++;
  }

  msort((void*) a, len, sizeof(unsigned), intcomp);

  unsigned *postcount = calloc(2*len, sizeof(unsigned));

  for(i = 0; i < len - 1; i++) {
    if(a[i] > a[i+1]) {
      printf("Test %d on length %d failed at index %d\n", num, len, i);
      printf("%d > %d\n", a[i], a[i+1]);
      free(a);
      free(precount);
      free(postcount);
      return 0;
    }
    postcount[a[i]]++;
  }
  /* Last iteration for postcount */
  postcount[a[len-1]]++;

  for(i = 0; i < 2*len; i++) {
    if(precount[i] != postcount[i]) {
      printf("Number of %u's before and after sorting do not match!\n", i);
      free(a);
      free(precount);
      free(postcount);
      return 0;
    }
  }

  free(a);
  free(precount);
  free(postcount);
  return 1;
}

void bulktester()
{
  int i, testcount, correctcount, testlen;
  testcount = correctcount = 0;
  testlen = 64;

  while(testlen < TESTLENBOUND) {
    for(i = 0; i < TESTNUM; i++) {
      testcount++;
      if(test_and_verify_permutations(i, testlen+CONSTANT)) {
        correctcount++;
        printf("Test %d on length %d correctly sorted and is a permutation\n", i+1, testlen+CONSTANT);
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
  bulktester();
  return 0;
}
