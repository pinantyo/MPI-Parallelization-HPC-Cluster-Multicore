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

int main(int argc, char **argv){
    int myrank;
    int row, SIZE;

    MPI_Status status;
    MPI_Comm parentcomm;

    int numtasks,taskid,source,dest,rows,offset,i,j,k;
    struct timeval start, stop;


    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_get_parent(&parentcomm);
    MPI_Comm_rank(parentcomm, &myrank);
    // printf("worker %d is up!\n", myrank);

    MPI_Bcast(&SIZE, 1, MPI_INT, 0, parentcomm);

    double a[SIZE][SIZE],b[SIZE][SIZE],c[SIZE][SIZE];

  /*---------------------------- worker----------------------------*/
  if (myrank > 0) {
    source = 0;
    double start_communication_worker = MPI_Wtime();
    MPI_Recv(&offset, 1, MPI_INT, source, MPI_ANY_TAG, parentcomm, &status);
    MPI_Recv(&rows, 1, MPI_INT, source, MPI_ANY_TAG, parentcomm, &status);
    MPI_Recv(&a, rows*SIZE, MPI_DOUBLE, source, MPI_ANY_TAG, parentcomm, &status);
    MPI_Recv(&b, SIZE*SIZE, MPI_DOUBLE, source, MPI_ANY_TAG, parentcomm, &status);
    printf(" ********** time MPI Recv Worker = %f \n", MPI_Wtime() - start_communication_worker);

    while ( status.MPI_TAG > 0 ) {
        row = status.MPI_TAG - 1;

        /* Matrix multiplication */
        for (k=0; k<SIZE; k++)
          for (i=0; i<rows; i++) {
            c[i][k] = 0.0;
            for (j=0; j<SIZE; j++)
              c[i][k] = c[i][k] + a[i][j] * b[j][k];
          }


        start_communication_worker = MPI_Wtime();
        MPI_Send(&offset, 1, MPI_INT, 0, row, parentcomm);
        MPI_Send(&rows, 1, MPI_INT, 0, row, parentcomm);
        MPI_Send(&c, rows*SIZE, MPI_DOUBLE, 0, row, parentcomm);
    }
  }

  MPI_Comm_free(&parentcomm);
  MPI_Finalize();
  return 0;
}









