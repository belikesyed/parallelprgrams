#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>
#include <time.h>


int main(int argc, char **argv)
{
    int *ptr=(int *)malloc((1024)*sizeof(int));
    int i,wrank,wsize;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&wsize);
    MPI_Comm_rank(MPI_COMM_WORLD,&wrank);

    double start,end,tim;
    if(wrank==0){
        start=clock();
        for(i=0;i<wsize;i++){
            MPI_Send(ptr,1024,MPI_INT,1,0,MPI_COMM_WORLD);
        }
        end=clock();
        tim=(end-start)/CLOCKS_PER_SEC;
        printf("Time taken in p2p:%f s\t",tim);
        
        start=clock();
        MPI_Bcast(ptr,1024,MPI_INT,0,MPI_COMM_WORLD);
        end=clock();
        tim=(end-start)/CLOCKS_PER_SEC;
        printf("Time taken in collectives Bcast:%f s\n",tim);

    }
    else
        MPI_Recv(ptr,1024,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    MPI_Finalize();
    return 0;
}

