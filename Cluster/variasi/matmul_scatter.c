#define N 4000
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stddef.h>
#include <mpi.h>

// Fungsi print_results dihapus karena tidak diperlukan lagi

int main(int argc, char *argv[]){
    int i, j, k, rank, size, tag = 99, blksz, sum = 0;
    int a[N][N]={{1,2,3,4},{5,6,7,8},{9,1,2,3},{4,5,6,7}};
    int b[N][N]={{1,2,3,4},{5,6,7,8},{9,1,2,3},{4,5,6,7}};
    int c[N][N];
    int aa[N],cc[N];

    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Ambil waktu mulai di proses root
    if (rank == 0) {
        start_time = MPI_Wtime();
    }

    //scatter rows of first matrix to different processes
    MPI_Scatter(a, N*N/size, MPI_INT, aa, N*N/size, MPI_INT,0,MPI_COMM_WORLD);
    //broadcast second matrix to all processes
    MPI_Bcast(b, N*N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    //perform vector multiplication by all processes
    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++){
            sum = sum + aa[j] * b[j][i];
        }
        cc[i] = sum;
        sum = 0;
    }
    MPI_Gather(cc, N*N/size, MPI_INT, c, N*N/size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    
    // Ambil waktu selesai dan cetak di proses root
    if(rank == 0){
        end_time = MPI_Wtime();
        printf("Execution time: %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}
