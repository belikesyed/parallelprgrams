#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

struct timeval stop, start;

int main()
{
    int n,i;
    printf("Enter number of numbers to add:");
    scanf("%d",&n);
    if (n <= 0)
    {
	printf("Invalid value!!");
	exit(1);
    }
    float arr[n],sarr[n],psum,ssum;
    double ptod,stod;


    for(i=0;i<n;i++)
    {
      arr[i]=rand()%20;
      sarr[i]=arr[i];
      printf("%f ",arr[i]);

    }

    psum=0.0;
    gettimeofday(&start, NULL);
    #pragma omp parallel for shared(sum)
    {
        for (i = 0; i < n; i++)
        {
            #pragma omp critical
            psum += arr[i];
        }
    }
    gettimeofday(&stop, NULL);
    ptod=(double) (stop.tv_usec - start.tv_usec) / 1000000 +(double) (stop.tv_sec - start.tv_sec);


    ssum=0.0;
    for(i=0;i<n;i++)
    {
        ssum +=sarr[i];
    }

    if(psum==ssum)
    {
        printf("\nAnswer:%f with parallel time = %lf\n",psum,ptod*1000);
    }
    else
    {
        printf("Something went wrong!!\n");
    }

    return 0;
}

