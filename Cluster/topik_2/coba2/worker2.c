#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int SIZE, rows, offset, i, j, k;
    MPI_Status status;
    MPI_Comm parentcomm;

    MPI_Init(&argc, &argv);
    MPI_Comm_get_parent(&parentcomm);

    // Broadcast Size
    MPI_Bcast(&SIZE, 1, MPI_INT, 0, parentcomm);

    // Allocate matrices a, b, and c
    double **a = (double **)malloc(SIZE * sizeof(double *));
    double **b = (double **)malloc(SIZE * sizeof(double *));
    double **c = (double **)malloc(SIZE * sizeof(double *));
    for (i = 0; i < SIZE; i++) {
        a[i] = (double *)malloc(SIZE * sizeof(double));
        b[i] = (double *)malloc(SIZE * sizeof(double));
        c[i] = (double *)malloc(SIZE * sizeof(double));
    }

    MPI_Recv(&offset, 1, MPI_INT, 0, 1, parentcomm, &status);
    MPI_Recv(&rows, 1, MPI_INT, 0, 1, parentcomm, &status);
    MPI_Recv(&a[0][0], rows * SIZE, MPI_DOUBLE, 0, 1, parentcomm, &status);
    MPI_Recv(&b[0][0], SIZE * SIZE, MPI_DOUBLE, 0, 1, parentcomm, &status);

    /* Multiply matrices */
    for (i = 0; i < rows; i++) {
        for (j = 0; j < SIZE; j++) {
            c[i][j] = 0.0;
            for (k = 0; k < SIZE; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    MPI_Send(&offset, 1, MPI_INT, 0, 2, parentcomm);
    MPI_Send(&rows, 1, MPI_INT, 0, 2, parentcomm);
    MPI_Send(&c[0][0], rows * SIZE, MPI_DOUBLE, 0, 2, parentcomm);


    MPI_Finalize();
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
