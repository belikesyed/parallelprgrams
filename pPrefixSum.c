#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>


void pPrefix(int *numArr, int *_ppSum, unsigned long size)
{
  int nthr, *z, *x = _ppSum;
  omp_set_num_threads(4);
  #pragma omp parallel
  {
    int i;
    #pragma omp single
    {
      nthr = omp_get_num_threads();
      z = malloc(sizeof(int)*nthr+1);
      z[0] = 0;
    }
    int tid = omp_get_thread_num();
    int sum = 0;
    #pragma omp for schedule(static)
    for(i=0; i<size; i++) {
      sum += numArr[i];
      x[i] = sum;
    }
    z[tid+1] = sum;
    #pragma omp barrier

    int offset = 0;
    for(i=0; i<(tid+1); i++) {
        offset += z[i];
    }

    #pragma omp for schedule(static)
    for(i=0; i<size; i++) {
      x[i] += offset;
    }
  }
  free(z);
}

int main(void ) {

  int *numArr, *ppSum,n,i,sum=0;

  printf("Enter total number:");
  scanf("%d",&n);

  numArr = (int*) malloc(sizeof(int) * n);
  ppSum = (int*) malloc(sizeof(int) * n);

  for(i=0; i<n; i++) numArr[i] = i+1;
  for(i=0; i<n; i++) printf("%d \n", numArr[i]);

  pPrefix(numArr, ppSum, n);
  for(i=0; i<n; i++) printf("%d \n", ppSum[i]);
  for(i=0; i<n; i++) sum+=numArr[i];

  printf("%d \n", sum);
  return 0;
}
