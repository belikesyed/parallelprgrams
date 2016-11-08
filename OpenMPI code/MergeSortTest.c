#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi/mpi.h>

#define N 100000

int * merge(int *A, int asize, int *B, int bsize);
void m_sort(int *A, int min, int max);

double startT, stopT,startTime;

/*function to merge vectors*/
int * merge(int *A, int asize, int *B, int bsize) {
	int ai=0, bi=0, ci=0, i;
	int *C;
	int csize = asize+bsize;

	C = (int *)malloc(csize*sizeof(int));
	while ((ai < asize) && (bi < bsize)) {
		if (A[ai] <= B[bi])
			C[ci++] = A[ai++];
		else
			C[ci++] = B[bi++];
	}

	if (ai >= asize)			/*if A is shorter*/
		for (i = ci; i < csize; i++, bi++)
			C[i] = B[bi];
	else if (bi >= bsize)			/*if B is shorter*/
		for (i = ci; i < csize; i++, ai++)
			C[i] = A[ai];

	for (i = 0; i < asize; i++)
		A[i] = C[i];
	for (i = 0; i < bsize; i++)
		B[i] = C[asize+i];

	return C;
}

void m_sort(int *A, int min, int max)
{
	int *C;		/* dummy, just to fit the function */
	int mid = (min+max)/2;
	int lowerCount = mid - min + 1;
	int upperCount = max - mid;

	if (max == min) {
		return;
	} else {
		m_sort(A, min, mid);
		m_sort(A, mid+1, max);
		C = merge(A + min, lowerCount, A + mid + 1, upperCount);
	}
}

main(int argc, char **argv)
{
	int *data,*chunk,*other;
	int m,n=N,id,p,i,s=0,step;
	MPI_Status status;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	MPI_Comm_size(MPI_COMM_WORLD,&p);

	startT = MPI_Wtime();

	if(id == 0)
	{
		int r;
		srandom(MPI_Wtime());
		s = n/p;
		r = n%p;
		data = (int *)malloc((n+s-r)*sizeof(int));
		for(i=0;i<n;i++)
			data[i] = random();
		if(r!=0)
		{
			for(i=n;i<n+s-r;i++)
				data[i]=0;
			s=s+1;
		}

		MPI_Bcast(&s,1,MPI_INT,0,MPI_COMM_WORLD);
		chunk = (int *)malloc(s*sizeof(int));
		MPI_Scatter(data,s,MPI_INT,chunk,s,MPI_INT,0,MPI_COMM_WORLD);
		m_sort(chunk, 0, s-1);
	}
	else
	{
		MPI_Bcast(&s,1,MPI_INT,0,MPI_COMM_WORLD);
		chunk = (int *)malloc(s*sizeof(int));
		MPI_Scatter(data,s,MPI_INT,chunk,s,MPI_INT,0,MPI_COMM_WORLD);
		m_sort(chunk, 0, s-1);
	}

	step = 1;
	while(step<p)
	{
		if(id%(2*step)==0)
		{
			if(id+step<p)
			{
				MPI_Recv(&m,1,MPI_INT,id+step,0,MPI_COMM_WORLD,&status);
				other = (int *)malloc(m*sizeof(int));
				MPI_Recv(other,m,MPI_INT,id+step,0,MPI_COMM_WORLD,&status);
				chunk = merge(chunk,s,other,m);
				s = s+m;
			}
		}
		else
		{
			int near = id-step;
			MPI_Send(&s,1,MPI_INT,near,0,MPI_COMM_WORLD);
			MPI_Send(chunk,s,MPI_INT,near,0,MPI_COMM_WORLD);
			break;
		}
		step = step*2;
	}

	stopT = MPI_Wtime();
	if(id==0)
	{
		FILE * fout;

		printf("%d random numbers to sort; %d processors; %f secs\n", s, p, (stopT-startT));
		printf("Sorted numbers in file result.txt\n\n");

		fout = fopen("result.txt","w");
		for(i=0;i<s;i++)
			fprintf(fout,"%d\n",chunk[i]);
		fclose(fout);
	}
	MPI_Finalize();
	return 0;
}

