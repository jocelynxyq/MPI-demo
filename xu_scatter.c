/*
 * 用MPI_SEND和MPI_RECV实现MPI_Scatter
 * 
 */

#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include <math.h> 
 
#define N 10

int scatter(MPI_Comm comm, int blk, int *a)
{
	int iam, np;
	MPI_Comm tcom, scom;
	MPI_Status st;
	int len, color, key;

	MPI_Comm_size(comm, &np);
	MPI_Comm_rank(comm, &iam);
	MPI_Comm_dup(comm, &tcom);

	//每次都把进程组划分为两个 把后1/2的数据发送给第二个进程组的首个进程。以此类推。
	while (np > 1) {

		len = np / 2;

		if (iam == 0) {
			MPI_Send(&a[len*blk], (np - len)*blk, MPI_INT, len, 1, tcom);
		}
		if (iam == len) {
			MPI_Recv(&a[0], (np - len)*blk, MPI_INT, 0, 1, tcom, &st);
		}

		if (iam < len) {
			color = 0;
		}
		else {
			color = 1;
		}

		key = iam;
		MPI_Comm_split(tcom, color, key, &scom);

		MPI_Comm_dup(scom, &tcom);
		MPI_Comm_size(tcom, &np);
		MPI_Comm_rank(tcom, &iam);
	}

	return 0;
}

int main(int argc, char** argv)
{
	MPI_Comm comm;
	int a[N];

	int i, iam, np;

	MPI_Init(&argc, &argv);
	MPI_Comm_dup(MPI_COMM_WORLD, &comm);
	MPI_Comm_rank(comm, &iam);
	MPI_Comm_size(comm, &np);

	if (iam == 0)
	{
		for (i = 0; i < N; i++) {
			a[i] = i + 1;
		}
	}
	else {
		for (i = 0; i < N; i++)
			a[i] = 0;
	}

	scatter(comm, 1, &a[0]);

	printf("The process %d is running!\n", iam);
	for (i = 0; i < np; i++)
		printf("a[%d]=%d ", i, a[i]);
	printf("\n");

	MPI_Finalize();

	return 0;
}