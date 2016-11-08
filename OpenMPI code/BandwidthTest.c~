#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>
#include <time.h>

#define mb 10240 //1048576 /* 1024*1024=1048576*/

int main(int argc, char **argv)
{
    int i,wrank;
    int *ptr=(int *)malloc((mb)*sizeof(int));
    printf("Transfering 100 files of %lu kbs\n",100*mb/1024*sizeof(int));
    MPI_Init(&argc,&argv);
    //MPI_Comm_size(MPI_COMM_WORLD,&wsize);
    MPI_Comm_rank(MPI_COMM_WORLD,&wrank);

    double start,end,tim,throughput;
    if(wrank==0){
        start=clock();
        for(i=0;i<100;i++){
            MPI_Send(ptr,mb,MPI_INT,1,0,MPI_COMM_WORLD);
        }
        end=clock();
        tim=(end-start)/CLOCKS_PER_SEC;
        throughput=(4*100*mb)/(tim*1024*1024*1024);
        printf("Time taken:%f s\tThoughput: %f GBps\n",tim,throughput);

    }
    else{
            MPI_Recv(ptr,mb,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUSES_IGNORE);
    }
    MPI_Finalize();
    return 0;
}

