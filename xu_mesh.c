/*
 * 将p×q个处理器处理成二维（p×q）的处理器阵列。
 * 列出每个处理器在自己的通信组里的编号
 * 
 */

#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include <math.h>  

#define p 4
#define q 3

int mesh(MPI_Comm comm)
{
	int iam, key, color;
	int rowid, colid;
	MPI_Comm rowcomm, colcomm;

	MPI_Comm_rank(comm, &iam);

	color = iam / q;
	key = iam;
	MPI_Comm_split(comm, color, key, &rowcomm);
	MPI_Comm_rank(rowcomm, &colid);

	color = iam % q;
	MPI_Comm_split(comm, color, key, &colcomm);
	MPI_Comm_rank(colcomm, &rowid);

	printf("process %d will be meshed at [%d,%d] \n", iam, rowid, colid);

	return 0;
}

int main(int argc, char** argv)
{
	MPI_Comm  comm;
	MPI_Init(&argc, &argv);
	MPI_Comm_dup(MPI_COMM_WORLD, &comm);

	mesh(comm);

	MPI_Finalize();

	return 0;
}