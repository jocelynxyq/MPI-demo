/*
 * 用MPI_SEND和MPI_RECV实现MPI_Bcast
 * 
 */

#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include <math.h>  
#define N 10

int bcast(int *a, int cnt, int root, MPI_Comm comm)
{
	int iam, np;
	int next, front;
	int n, newid, m, i, num;

	MPI_Status st;

	MPI_Comm_rank(comm, &iam);
	MPI_Comm_size(comm, &np);

	newid = (np + iam - root) % np;

	n = log(np + 0.001) / log(2.0);
	num = 1;
	for (i = 0; i < n; i++) {
		if (newid < num) {
			next = (iam + num) % np;
			MPI_Send(a, cnt, MPI_FLOAT, next, 1, comm);
		}
		else if (newid < 2 * num) {
			front = (np + iam - num) % np;
			MPI_Recv(a, cnt, MPI_FLOAT, front, 1, comm, &st);
		}
		num = 2 * num;
	}
	m = np - num;
	if (m != 0) {
		if (newid < m) {
			next = (iam + num) % np;
			MPI_Send(a, cnt, MPI_FLOAT, next, 1, comm);
		}
		else if (newid >= num) {
			front = (np + iam - num) % np;
			MPI_Recv(a, cnt, MPI_FLOAT, front, 1, comm, &st);
		}
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

	bcast(a, 2, 0, comm);

	printf("The process %d is running!\n", iam);
	for (i = 0; i < N; i++) {
		printf("a[%d]=%d ", i, a[i]);
	}
	printf("\n");

	MPI_Finalize();

	return 0;
}