/*
 * 将一个N×N的方阵的下三角矩阵取出来传送给另一个矩阵（单个元素的形式）
 * 
 */


#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include <math.h>  

#define N 6  //方阵的行数

int main(int argc, char** argv)

{

	int iam, np; 

	MPI_Comm comm;
	int a[N*N];

	int i, j;
	MPI_Datatype nt, fnt;

	int tag = 1;
	MPI_Status st;

	MPI_Init(&argc, &argv); 
	MPI_Comm_dup(MPI_COMM_WORLD, &comm); 
	MPI_Comm_rank(comm, &iam);
	MPI_Comm_size(comm, &np);

	int blklen[N];
	int disp[N];
	for (i = 0; i < N; i++)
		blklen[i] = i + 1;
	for (i = 0; i < N; i++)
		disp[i] = i*N;


	MPI_Type_indexed(N, blklen, disp, MPI_INT, &fnt);

	MPI_Type_commit(&fnt);
	
	if (iam == 0)
	{
		for (i = 0; i < N*N; i++)
			a[i] = i+1;
		MPI_Send(a, 1, fnt, 1, tag, comm);
		printf("The process %d is running!\n", iam);
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				printf("a[%d]=%d ", i*N + j, a[N*i + j]);
			}
			printf("\n");
		}
			
				
		printf("\n");

	}
	else if (iam == 1)
	{
		for (i = 0; i < N*N; i++)
			a[i] = 0;
		MPI_Recv(a, 1, fnt, 0, tag, comm, &st);
		printf("The process %d is running!\n", iam);

		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++)
				printf("a[%d]=%d ", i*N + j, a[N*i + j]);
			printf("\n");
		}

	}

	MPI_Type_free(&fnt);

	MPI_Finalize();

	return 0;
}