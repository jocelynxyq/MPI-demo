#include "mpi.h"
void begin(argc, argv, comm, np, iam)
MPI_Comm *comm;
int *argc, *np, *iam;
char ***argv;
{
  MPI_Init( argc, argv );
  MPI_Comm_dup( MPI_COMM_WORLD, comm );
  MPI_Comm_size( *comm, np );
  MPI_Comm_rank( *comm, iam );

  return;
}
