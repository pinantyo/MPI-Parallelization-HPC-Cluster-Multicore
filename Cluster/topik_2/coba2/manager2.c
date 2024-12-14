/**********************************************************************
 * MPI-based matrix multiplication AxB=C                                                                                                                     
 *********************************************************************/
#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

int main(int argc, char **argv)
{
    int SIZE, numworkers, rows, offset, i, j, k;
    struct timeval start, stop;
    double comm_time,exec_time;

    // Input arguments
    if (argc != 3) {
        printf("Usage: %s <number of workers> <square matrix/vector row size>\n", argv[0]);
        return -1;
    }

    numworkers = atoi(argv[1]);
    SIZE = atoi(argv[2]);
    exec_time -= MPI_Wtime();
    // Allocate memory for matrices dynamically
    double **a = (double **)malloc(SIZE * sizeof(double *));
    double **b = (double **)malloc(SIZE * sizeof(double *));
    double **c = (double **)malloc(SIZE * sizeof(double *));
    for (i = 0; i < SIZE; i++) {
        a[i] = (double *)malloc(SIZE * sizeof(double));
        b[i] = (double *)malloc(SIZE * sizeof(double));
        c[i] = (double *)malloc(SIZE * sizeof(double));
    }

    MPI_Status status;
    MPI_Comm workercomm;
    comm_time -= MPI_Wtime();
    MPI_Init(&argc, &argv);
    comm_time += MPI_Wtime();
    MPI_Comm_spawn("worker2", MPI_ARGV_NULL, numworkers, MPI_INFO_NULL, 0, MPI_COMM_SELF, &workercomm, MPI_ERRCODES_IGNORE);

    // Initialize matrix in the manager process
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            a[i][j] = 1.0;
            b[i][j] = 2.0;
        }
    }

    gettimeofday(&start, 0);
    comm_time -= MPI_Wtime();
    /* Send SIZE to each worker */
    MPI_Bcast(&SIZE, 1, MPI_INT, MPI_ROOT, workercomm);

    /* Send matrix data to the worker tasks */
    rows = SIZE / numworkers;
    offset = 0;
    comm_time += MPI_Wtime();
    for (int dest = 0; dest < numworkers; dest++) {
        comm_time -= MPI_Wtime();
	MPI_Send(&offset, 1, MPI_INT, dest, 1, workercomm);
        MPI_Send(&rows, 1, MPI_INT, dest, 1, workercomm);
        MPI_Send(&a[offset][0], rows * SIZE, MPI_DOUBLE, dest, 1, workercomm);
        MPI_Send(&b[0][0], SIZE * SIZE, MPI_DOUBLE, dest, 1, workercomm);
        comm_time += MPI_Wtime();
	offset += rows;
    }

    /* Wait for results from all worker tasks */
    for (int i = 0; i < numworkers; i++) {
	comm_time -= MPI_Wtime();
        MPI_Recv(&offset, 1, MPI_INT, i, 2, workercomm, &status);
        MPI_Recv(&rows, 1, MPI_INT, i, 2, workercomm, &status);
        MPI_Recv(&c[offset][0], rows * SIZE, MPI_DOUBLE, i, 2, workercomm, &status);
	comm_time += MPI_Wtime();
       }

   // gettimeofday(&stop, 0);

    //printf("Result matrix:\n");
    /*
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            printf("%6.2f   ", c[i][j]);
        }
        printf("\n");
    }
    */
    //exec_time += MPI_Wtime();
    //fprintf(stdout, "%d\t%d\t%f\t%f\tTime = %.6f seconds\n", numworkers,SIZE,comm_time,exec_time,(stop.tv_sec + stop.tv_usec * 1e-6) - (start.tv_sec + start.tv_usec * 1e-6));
    MPI_Finalize();
    gettimeofday(&stop, 0);
    exec_time += MPI_Wtime();
    fprintf(stdout, "%d\t%d\t%f\t%f\tTime = %.6f seconds\n", numworkers,SIZE,comm_time,exec_time,(stop.tv_sec + stop.tv_usec * 1e-6) - (start.tv_sec + start.tv_usec * 1e-6)); 
    for (i = 0; i < SIZE; i++) {
        free(a[i]);
        free(b[i]);
        free(c[i]);
    }
    free(a);
    free(b);
    free(c);

    return 0;
}
