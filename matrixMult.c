#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

struct timeval stop, start;

int main()
{
	int marr[10][10], varr[10], farr[10];
	srand (time(NULL));
	double ptod;
	printf("\nMatrix\n");
	int i,j;
	for(i=0;i<10; i++){
		for(j=0; j<10; j++){
			marr[i][j]=rand() % 20;
			printf("%d\t",marr[i][j]);
		}
		printf("\n");
		varr[i]=rand() % 20;
	}
	printf("\nVector\n");
	//
	for(i=0; i<10; i++){
		printf("%d\t",varr[i]);
	}

	gettimeofday(&start, NULL);
	#pragma omp parallel num_threads(2)
	{
		int id = omp_get_thread_num();
		int sum=0;
		for(i=id;i<10;i+=2){
			sum=0;
			#pragma omp parallel num_threads(2)
			{
				for(j=0;j<10;j++){
					sum=marr[i][j]*varr[j]+sum;
				}
			}

			farr[i]=sum;
		}
	}
    gettimeofday(&stop, NULL);
    ptod=(double) (stop.tv_usec - start.tv_usec) / 1000000 +(double) (stop.tv_sec - start.tv_sec);
	for(i=0;i<10;i++){
		printf("%d\t",farr[i]);
	}

	printf("\n%lf time enlapsed \n",ptod*1000);

	return 0;
}
