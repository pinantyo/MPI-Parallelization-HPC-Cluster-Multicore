/*
 *  mmult.c: matrix multiplication using MPI.
 * There are some simplifications here. The main one is that matrices B and C 
 * are fully allocated everywhere, even though only a portion of them is 
 * used by each processor (except for processor 0)
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 256      /* Size of matrices */

void fill_matrix(int m[SIZE][SIZE])
{
  static int n=0;
  int i, j;
  for (i=0; i<SIZE; i++)
    for (j=0; j<SIZE; j++)
      m[i][j] = n++;
}

void print_matrix(int m[SIZE][SIZE])
{
  int i, j = 0;
  for (i=0; i<SIZE; i++) {
    printf("\n\t| ");
    for (j=0; j<SIZE; j++)
      printf("%2d ", m[i][j]);
    printf("|");
  }
}


int main(int argc, char *argv[])
{
  int myrank, P, from, to, i, j, k;
  int tag = 666;    /* any value will do */
  double start, end;  // for timing measurements

  // Input arguments
  if (argc != 3) {
      printf("Usage: %s <number of workers> <square matrix/vector row size>\n", argv[0]);
      return -1;
  }

  numworkers = atoi(argv[1]);
  SIZE = atoi(argv[2]);

  // Allocate memory for matrices dynamically
  double **A = (double **)malloc(SIZE * sizeof(double *));
  double **B = (double **)malloc(SIZE * sizeof(double *));
  double **C = (double **)malloc(SIZE * sizeof(double *));
  for (i = 0; i < SIZE; i++) {
      A[i] = (double *)malloc(SIZE * sizeof(double));
      B[i] = (double *)malloc(SIZE * sizeof(double));
      C[i] = (double *)malloc(SIZE * sizeof(double));
  }
  

  MPI_Status status;
  MPI_Comm workercomm;
  MPI_Init (&argc, &argv);
  MPI_Comm_spawn("worker", MPI_ARGV_NULL, numworkers, MPI_INFO_NULL, 0, MPI_COMM_SELF, &workercomm, MPI_ERRCODES_IGNORE);

  /* Just to use the simple variants of MPI_Gather and MPI_Scatter we */
  /* impose that SIZE is divisible by P. By using the vector versions, */
  /* (MPI_Gatherv and MPI_Scatterv) it is easy to drop this restriction. */

  if (SIZE%P!=0) {
    if (myrank==0) printf("Matrix size not divisible by number of processors\n");
    MPI_Finalize();
    exit(-1);
  }

  from = myrank * SIZE/P;
  to = (myrank+1) * SIZE/P;

  /* Process 0 fills the input matrices and broadcasts them to the rest */
  /* (actually, only the relevant stripe of A is sent to each process) */

  if (myrank==0) {
    fill_matrix(A);
    fill_matrix(B);

    start = MPI_Wtime();
  }

  double start_communication = MPI_Wtime();
  MPI_Bcast (&from, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast (&B, SIZE*SIZE, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter (&A, SIZE*SIZE/P, MPI_INT, A[from], SIZE*SIZE/P, MPI_INT, 0, MPI_COMM_WORLD);
  printf(" ********** time Bcast Scatter = %f \n", MPI_Wtime() - start_communication);

  start_communication = MPI_Wtime();
  MPI_Gather (&C[from], SIZE*SIZE/P, MPI_INT, C, SIZE*SIZE/P, MPI_INT, 0, MPI_COMM_WORLD);
  printf(" ********** time Gather = %f \n", MPI_Wtime() - start_communication);

  if (myrank==0) {
    double time = MPI_Wtime() - start;
    printf(" ********** time = %f \n", time);
    // printf("\n\n");
    // print_matrix(A);
    // printf("\n\n\t       * \n");
    // print_matrix(B);
    // printf("\n\n\t       = \n");
    // print_matrix(C);
    // printf("\n\n");
  }

  MPI_Finalize();
  return 0;
}




