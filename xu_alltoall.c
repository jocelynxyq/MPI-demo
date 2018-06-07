/*
* 用MPI_SEND和MPI_RECV实现MPI_ALLTOALL
* 或者：用MPI_SENDRECV、MPI_SEND和MPI_RECV实现MPI_ALLTOALL
*/

#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>

#define  maxlen 10

int  My_Alltoall(int  *sendBuffer, int sendcnt, MPI_Datatype sendtype,
	int *receiveBuffer, int recvcnt, MPI_Datatype recvtype, MPI_Comm comm, int rank, int size)
{
	int i;
	int j;
	MPI_Status status;

	if (size != sendcnt || sendtype != recvtype)
		return 0;
	for (i = 0; i<size; i++)
	{
		if (rank == i)
		{
			MPI_Sendrecv(&sendBuffer[i],1,sendtype,i,i,&receiveBuffer[i],1,recvtype,i,i,comm,&status);
			//若要求仅用Send和Recv两个函数实现，则上面一句可换成下面的：
			//receiveBuffer[i] = sendBuffer[i];
		}
		else
		{
			MPI_Send(&sendBuffer[i], 1, sendtype, i, i, comm);
			MPI_Recv(&receiveBuffer[i], 1, recvtype, i, rank, comm, &status);
		}

	}
	return 1;

}

int main(int argc, char *argv[])
{
	int rank, size;
	MPI_Status status;


	int sendBuffer[maxlen], receiveBuffer[maxlen];
	int i, j;
	int count;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	//判断进程数是否合法
	if (size < 1 || size > 10)
	{
		if (rank == 0) printf("Please input a  number between 1-10\n");
		MPI_Finalize();
		return 0;
	}
	count = size;

	for (i = 0; i<maxlen; i++)
	{
		sendBuffer[i] = rank*3+i; //初始化发送缓冲区
		receiveBuffer[i] = 0;	  //初始化接收缓冲区
	}


	My_Alltoall(sendBuffer, count, MPI_INT, receiveBuffer, count, MPI_INT, MPI_COMM_WORLD, rank, size);

	printf("rank %d\n", rank);
	for (i = 0; i<count; i++)
	{
		printf("%d\t", receiveBuffer[i]);
	}
	printf("\n", rank);

	MPI_Finalize();
	return (0);
}