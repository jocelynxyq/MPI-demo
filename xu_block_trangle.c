/*
 * 将一个N×N的方阵的下三角矩阵取出来传送给另一个矩阵（块元素的形式）
 * 
 */


#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include <math.h>  

#define N 6  //方阵的行数
#define M 2  //方阵分块的阶数
#define K N/M //方阵的块数


int main(int argc, char** argv)

{

	int iam, np;

	MPI_Comm comm;
	int a[N*N];

	int i, j;
	MPI_Datatype nt, rownt[K], trant;

	int tag = 1;
	MPI_Status st;

	MPI_Init(&argc, &argv); 
	MPI_Comm_dup(MPI_COMM_WORLD, &comm); 
	MPI_Comm_rank(comm, &iam);
	MPI_Comm_size(comm, &np);

	//块的定义
	MPI_Type_vector(M, M, N, MPI_INT, &nt);
	MPI_Type_commit(&nt);

	MPI_Aint extent;
	MPI_Type_extent(MPI_INT, &extent);

	int blklen[K];
	MPI_Aint disp[K];

	//每行的定义
	for (i = 0; i < K; i++)
	{
		MPI_Type_hvector(i + 1, 1, M*extent, nt, &rownt[i]);
		MPI_Type_commit(&rownt[i]);
		blklen[i] = 1;
		disp[i] = i * M * N * extent;
	}

	//下三角的定义
	MPI_Type_struct(K, blklen, disp, rownt, &trant);

	MPI_Type_commit(&trant);


	if (iam == 0)
	{
		for (i = 0; i < N*N; i++)
			a[i] = i;
		MPI_Send(a, 1, trant, 1, tag, comm);
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
		MPI_Recv(a, 1, trant, 0, tag, comm, &st);
		printf("The process %d is running!\n", iam);

		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++)
				printf("a[%d]=%d ", i*N + j, a[N*i + j]);
			printf("\n");
		}

	}

	MPI_Type_free(&nt);
	for (i = 0; i < K; i++)
		MPI_Type_free(&rownt[i]);
	MPI_Type_free(&trant);

	MPI_Finalize();

	return 0;
}