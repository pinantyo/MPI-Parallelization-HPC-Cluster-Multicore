#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define N 256

double a[N][N], b[N][N], c[N][N];

int main(int argc, char **argv) {
    int numtasks, taskid, numworkers, rows, offset, i, j, k;
    struct timeval start, stop;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    numworkers = numtasks - 1;

    if (taskid == 0) {
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                a[i][j] = 1.0;
                b[i][j] = 2.0;
            }
        }

        gettimeofday(&start, 0);

        rows = N / numworkers;
        offset = 0;
        printf("RANK TASK ID == 0");

        MPI_Scatter(a, rows * N, MPI_DOUBLE, MPI_IN_PLACE, rows * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(b, N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    } else {
        rows = N / numworkers;
        printf("RANK TASKID != 0");
        MPI_Scatter(NULL, rows * N, MPI_DOUBLE, a, rows * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(b, N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    for (k = 0; k < N; k++) {
        for (i = 0; i < rows; i++) {
            c[i][k] = 0.0;
            for (j = 0; j < N; j++) {
                c[i][k] += a[i][j] * b[j][k];
            }
        }
    }

    if (taskid == 0) {
        MPI_Gather(MPI_IN_PLACE, rows * N, MPI_DOUBLE, c, rows * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        gettimeofday(&stop, 0);

        printf("Here is the result matrix:\n");
        /*for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                printf("%6.2f   ", c[i][j]);
            }
            printf("\n");
        }*/

        fprintf(stdout, "Time = %.6f\n\n",
                (stop.tv_sec + stop.tv_usec * 1e-6) - (start.tv_sec + start.tv_usec * 1e-6));
    } else {
        printf("RANK Gather");
        MPI_Gather(c, rows * N, MPI_DOUBLE, NULL, rows * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}


