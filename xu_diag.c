/*
 * 假设分块下三角矩阵的对角块矩阵都是M×M阶的，给出一个下三角矩阵的数据类型
 * 使得可以用于传送这个矩阵的多个对角块矩阵
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
	MPI_Datatype nt, diag;

	int tag = 1;
	MPI_Status st;

	MPI_Init(&argc, &argv); 
	MPI_Comm_dup(MPI_COMM_WORLD, &comm); 
	MPI_Comm_rank(comm, &iam);
	MPI_Comm_size(comm, &np);

	
	MPI_Type_vector(M, M, N, MPI_INT, &nt);
	MPI_Type_commit(&nt);

	MPI_Aint extent;
	MPI_Type_extent(MPI_INT, &extent);

	MPI_Type_hvector(K, 1, (M * N + M) * extent, nt, &diag);
	//MPI_Type_vector(K, 1, (M * N + M), nt, &diag);
	
	MPI_Type_commit(&diag);


	if (iam == 0)
	{
		for (i = 0; i < N*N; i++)
			a[i] = i;
		MPI_Send(a, 1, diag, 1, tag, comm);
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
		MPI_Recv(a, 1, diag, 0, tag, comm, &st);
		printf("The process %d is running!\n", iam);

		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++)
				printf("a[%d]=%d ", i*N + j, a[N*i + j]);
			printf("\n");
		}

	}

	MPI_Type_free(&nt);
	MPI_Type_free(&diag);

	MPI_Finalize();

	return 0;
}