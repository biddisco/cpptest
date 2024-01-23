#include "mpi.h"
#include <stdio.h>

int main (int argc, char *argv[]) {
   
   int size;
   int rank;
   char b[100];
   MPI_Status st;
   
   MPI_Init (&argc, &argv);
   MPI_Pcontrol(2);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   
   if (rank == 0) {
      printf("Sending from example ...\n");
      MPI_Send(b, 100, MPI_CHAR, 1, 99, MPI_COMM_WORLD);
   } else {
      MPI_Recv(b, 100, MPI_CHAR, 0, 99, MPI_COMM_WORLD, &st);
   }
   
   MPI_Finalize();
   return 0;
}
