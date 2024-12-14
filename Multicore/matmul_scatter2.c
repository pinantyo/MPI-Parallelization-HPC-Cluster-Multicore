#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stddef.h>
#include <mpi.h>

#define N 1024

int main(int argc, char *argv[]) {
    int i, j, k, rank, size, tag = 99, sum = 0;
    int a[N][N] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 1, 2, 3}, {4, 5, 6, 7}};
    int b[N][N] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 1, 2, 3}, {4, 5, 6, 7}};
    int c[N][N];
    int aa[N / size][N], cc[N / size][N];

    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Ensure the matrix size is divisible by the number of processes
    if (N % size != 0) {
        if (rank == 0) {
            fprintf(stderr, "Matrix size N must be divisible by the number of processes.\n");
        }
        MPI_Finalize();
        return 1;
    }

    // Scatter rows of the first matrix
    MPI_Scatter(a, N * N / size, MPI_INT, aa, N / size * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Broadcast the second matrix
    MPI_Bcast(b, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Perform matrix multiplication
    for (i = 0; i < N / size; i++) {
        for (j = 0; j < N; j++) {
            sum = 0;
            for (k = 0; k < N; k++) {
                sum += aa[i][k] * b[k][j];
            }
            cc[i][j] = sum;
        }
    }

    // Gather results from all processes
    MPI_Gather(cc, N / size * N, MPI_INT, c, N / size * N, MPI_INT, 0, MPI_COMM_WORLD); 

    if (rank == 0) {
        end_time = MPI_Wtime();
        printf("Execution time: %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}
