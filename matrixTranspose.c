#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

struct timeval stop, start;

int main()
{
    int i,j,rows,column;
    double ptod;

    printf("Matrix Transpose\n");
    printf("Enter number of rows:");
    scanf("%d",&rows);
    printf("Enter number of column:");
    scanf("%d",&column);
    if (rows <= 0|| column <= 0)
    {
	printf("Invalid value!!");
	exit(1);
    }

    int matrix[rows][column],Tmatrix[rows][column];


    for(i=0;i<rows;i++)
    {
      for(j=0;j<column;j++)
      {
        matrix[i][j]=rand()%100;
        printf("%d ",matrix[i][j]);
      }
      printf("\n");

    }

    gettimeofday(&start, NULL);
    #pragma omp parallel for private(i) shared(j) schedule(runtime)
    {
        for (i = 0; i < rows; i++)
        {
        #pragma omp parallel for private(i) shared(j) schedule(dynamic)
            for(j=0;j<column;j++)
            {
                Tmatrix[i][j]=matrix[j][i];
            }
        }
    }
    gettimeofday(&stop, NULL);
    ptod=(double) (stop.tv_usec - start.tv_usec) / 1000000 +(double) (stop.tv_sec - start.tv_sec);

    printf("\n\n");

     for(i=0;i<rows;i++)
    {
      for(j=0;j<column;j++)
      {
        printf("%d ",Tmatrix[i][j]);
      }
      printf("\n");

    }
    printf("time taken:%lfms\n",ptod);

    return 0;
}

