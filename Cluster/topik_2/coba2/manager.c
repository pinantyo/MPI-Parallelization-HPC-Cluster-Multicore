/**********************************************************************                                                                       * https://gist.github.com/shinde-rahul/0b38d58c3d5d7f7346210cdee90cc3b0               
 * MPI-based matrix multiplication AxB=C                                                                                                                     
 *********************************************************************/
#include "mpi.h"
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
  int SIZE, numtasks,taskid,numworkers,source,dest,row,rows,offset,i,j,k;
  struct timeval start, stop;

  // Input arguments
  if(argc != 3){
    printf( "usage: %s <number of workers> <square matrix/vector row size>\n", argv[0]);
  } else {
    numworkers = atoi(argv[1]);
    SIZE = atoi(argv[2]);
  }

  double a[SIZE][SIZE],b[SIZE][SIZE],c[SIZE][SIZE];
  MPI_Status status;
  MPI_Comm workercomm;


  MPI_Init(&argc, &argv);
  MPI_Comm_spawn("worker", MPI_ARGV_NULL, numworkers, MPI_INFO_NULL, 0, MPI_COMM_SELF, &workercomm, MPI_ERRCODES_IGNORE);

  numworkers = numtasks-1;

  // Initialize matrix
  for(i=0; i<SIZE; i++){
    for(j=0; j<SIZE; j++) {
        a[i][j]= 1.0;
        b[i][j]= 2.0;
    }
  }

  gettimeofday(&start, 0);

  /* send SIZE to each worker */
  MPI_Bcast(&SIZE, 1, MPI_INT, MPI_ROOT, workercomm);

  /* send matrix data to the worker tasks */
  rows = SIZE/numworkers;
  offset = 0;
  for(dest=1; dest<=numworkers; dest++){
    sender = status.MPI_SOURCE;
    row = status.MPI_TAG;
    MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
    MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
    MPI_Send(&a[offset][0], rows*SIZE, MPI_DOUBLE,dest,1, MPI_COMM_WORLD);
    MPI_Send(&b, SIZE*SIZE, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);
    offset = offset + rows;
  }

  /* wait for results from all worker tasks */
  for (i=1; i<=numworkers; i++){
    source = i;
    MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
    MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
    MPI_Recv(&c[offset][0], rows*SIZE, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
  }

  gettimeofday(&stop, 0);

  printf("Here is the result matrix:\n");

  // for (i=0; i<SIZE; i++) {
  //   for (j=0; j<SIZE; j++)
  //       printf("%6.2f   ", c[i][j]);
  //   printf ("\n");
  // }

  fprintf(stdout,"Time = %.6f\n\n", (stop.tv_sec+stop.tv_usec*1e-6)-(start.tv_sec+start.tv_usec*1e-6));

  MPI_Finalize();
  return 0;
}









