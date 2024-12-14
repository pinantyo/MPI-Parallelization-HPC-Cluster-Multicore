#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define MIN( x, y ) ((x) < (y) ? x : y)

int main( int argc, char *argv[] )
{
    int SIZE, i, j, row, numworkers, numsent, sender,rows;
    double dotp, comm_time = 0, exec_time = 0;
    if ( argc != 3 ) {
        printf( "usage: %s <number of workers> <square matrix/vector row size>\n", argv[0]);
    } else {
        numworkers = atoi( argv[1] );
        SIZE = atoi( argv[2] );
    }

    exec_time -= MPI_Wtime();
    double **a = (double **)malloc(SIZE * sizeof(double *));
    double **b = (double **)malloc(SIZE * sizeof(double *));
    double **c = (double **)malloc(SIZE * sizeof(double *));
    for (i = 0; i < SIZE; i++){
        a[i] = (double *)malloc(SIZE * sizeof(double));
        b[i] = (double *)malloc(SIZE * sizeof(double));
        c[i] = (double *)malloc(SIZE * sizeof(double));
    }

        
    //double *c = (double *)malloc(SIZE * sizeof(double));
   //double a[SIZE][SIZE],b[SIZE][SIZE],c[SIZE][SIZE]
    MPI_Status status;
    MPI_Comm workercomm;

    MPI_Init( &argc, &argv );
    comm_time -= MPI_Wtime();
    MPI_Comm_spawn( "worker_coba", MPI_ARGV_NULL, numworkers, MPI_INFO_NULL, 0, MPI_COMM_SELF, &workercomm, MPI_ERRCODES_IGNORE );
    comm_time += MPI_Wtime();

    /* initialize a and b */
    for (i = 0; i < SIZE; i++ )
        for ( j = 0; j < SIZE; j++ )
            a[i][j] = (double)1;
            b[i][j] = (double)2;

    /* send SIZE to each worker */
    comm_time -= MPI_Wtime();
    MPI_Bcast( &SIZE, 1, MPI_INT, MPI_ROOT, workercomm );

    /* send b to each worker */
    MPI_Bcast( &b, SIZE*SIZE, MPI_DOUBLE, MPI_ROOT, workercomm );

    /* send a's row to each process */
    numsent = 0;
    rows = SIZE/numworkers;

    for ( i = 0; i < MIN( numworkers, SIZE ); i++ ) {
        MPI_Send(&numsent, 1, MPI_INT, i, i+1, workercomm);
        MPI_Send(&rows, 1, MPI_INT, i, i+1, workercomm);
        MPI_Send(&a[numsent][0], rows*SIZE, MPI_DOUBLE, i, i+1, workercomm);
        numsent += rows;
    }
    comm_time += MPI_Wtime();

    //printf("numsent: %d SIZE: %d\n", numsent, SIZE);

    /* receive dot products back from workers */
    for ( i = 0; i < MIN( numworkers, SIZE ); i++ ) {
        comm_time -= MPI_Wtime();
        MPI_Recv(&numsent, 1, MPI_INT, i, i+1, workercomm, &status);
        MPI_Recv(&rows, 1, MPI_INT, i, i+1, workercomm, &status);
        MPI_Recv(&c[numsent][0], rows*SIZE , MPI_DOUBLE, i, i+1, workercomm, &status);
        comm_time += MPI_Wtime();
    }

    /* print matrix, vector & result */
    /*
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            printf("%f ", a[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    for (i = 0; i < SIZE; i++)
        printf("%f\n", b[i]);
    printf("\n");
    for (i = 0; i < SIZE; i++)
        printf("%f\n", c[i]);
    */
    for (i = 0; i < SIZE; i++){
        free(a[i]);
        free(b[i]);
        free(c[i]);

    }
    free(a);
    free(b);
    free(c);
    MPI_Finalize();
    exec_time += MPI_Wtime();

    printf("%d\t%f\t%f\n", numworkers, comm_time, exec_time);
    return 0;
}
