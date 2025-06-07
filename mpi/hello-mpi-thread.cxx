#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <errno.h>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
  int rank;
  char msg[1024];
  char fname[1024];
  MPI_Status status;
  int num_proc;
  int i;
  int dest = 0;
  int tag = 0;
  int fd;
  FILE *fp;

  // Init MPI
  int provided = MPI_THREAD_MULTIPLE;
  MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
  if (provided != MPI_THREAD_MULTIPLE)
  {
    std::cout << "Provided MPI is not : MPI_THREAD_MULTIPLE " << provided << std::endl;
  }

  MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Get the name of the processor
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(processor_name, &name_len);

  printf("MPI rank %d of %d %s\n", rank, num_proc, processor_name);
  if(num_proc < 2) {
    MPI_Finalize();
    return 0;
  }

  if (rank!=0) {
    sprintf(msg,"Hello World from %s",processor_name);
    MPI_Send(msg, strlen(msg), MPI_CHAR, dest, tag, MPI_COMM_WORLD);
    printf ("Sending   Hostname = \"%s\"\n", processor_name);
  }
  else {
    printf ("Receiving Hostname = \"%s\"\n", processor_name);
    for (i=1; i < num_proc ; i++) {
      MPI_Recv(msg, 256, MPI_CHAR, i, tag, MPI_COMM_WORLD, &status);
      int count = 0;
      MPI_Get_count(&status, MPI_CHAR, &count);
      msg[count] = 0;
      printf ("The message is: %s\n", msg);
    }
    printf ("Received messages from %d processes.\n", num_proc-1);
  }

/*
  sprintf(fname,"/scratch/bgas/biddisco/file%05d.txt", rank);
  printf ("Writing to %s\n", fname);
  fd = open(fname, O_CREAT|O_APPEND|O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP);
  write(fd,"Hello\n",6);
  close(fd);

  if (fd < 1) {
    perror("Error");
  }
*/

  MPI_Finalize();
}
