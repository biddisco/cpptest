#include <stdio.h>
#include "mpi.h"

static int totalBytes = 0;

int MPI_Init(int* argc, char*** argv)
{
    int err = PMPI_Init(argc, argv);
    return err;
}

int MPI_Send(const void* buffer, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
{
    int size;
    printf("Sending from profile ...\n");
    int result = PMPI_Send(buffer, count, datatype, dest, tag, comm);
    MPI_Type_size(datatype, &size);
    totalBytes += count * size;
    return result;
}

int MPI_Finalize()
{
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0)
    {
        printf("Total bytes: %d\n", totalBytes);
    }
    return PMPI_Finalize();
}
