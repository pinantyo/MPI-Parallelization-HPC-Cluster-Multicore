#include <mpi.h>
#include <stdio.h>

int main( int argc, char *argv[] )
{
    int numprocs, myrank, numsent;
    int i,j, k, row, SIZE, rows;
    MPI_Status status;
    MPI_Comm parentcomm;

    MPI_Init( &argc, &argv );
    MPI_Comm_get_parent( &parentcomm );

    // printf("worker %d is up!\n", myrank);
    MPI_Bcast( &SIZE, 1, MPI_INT, 0, parentcomm );
    MPI_Bcast( &rows, 1, MPI_INT, 0, parentcomm );
    // printf("worker %d get SIZE: %d\n", myrank, SIZE);
    
    double **b = (double **)malloc(SIZE * sizeof(double *));
    double **c = (double *)malloc(SIZE * sizeof(double));
    double **d = (double *)malloc(SIZE * sizeof(double));
    for (i = 0; i < SIZE; i++){
        b[i] = (double *)malloc(SIZE * sizeof(double));
        c[i] = (double *)malloc(SIZE * sizeof(double));
        d[i] = (double *)malloc(SIZE * sizeof(double));
    }

    MPI_Bcast( &b, SIZE*SIZE, MPI_DOUBLE, 0, parentcomm );
        
        MPI_Recv(&numsent, 1, MPI_INT, 0, MPI_ANY_TAG, parentcomm, &status);
        MPI_Recv(&rows, 1, MPI_INT, 0, MPI_ANY_TAG, parentcomm, &status);
        MPI_Recv(&c[0][0], rows*SIZE, MPI_DOUBLE, 0, MPI_ANY_TAG, parentcomm, &status );
        while ( status.MPI_TAG > 0 ) {
            row = status.MPI_TAG - 1;
            for (i = 0; i < rows; i++) {
                for (j = 0; j < SIZE; j++) {
                d[i][j] = 0.0;
                    for (k = 0; k < SIZE; k++) {
                        d[i][j] += c[i][k] * b[k][j];
                    }
                }
            }
            MPI_Send(&numsent, 1, MPI_INT, 0, MPI_ANY_TAG, parentcomm);
            MPI_Send(&rows, 1, MPI_INT, 0, MPI_ANY_TAG, parentcomm);
            MPI_Send(&d[0][0], rows*SIZE  , MPI_DOUBLE, 0, MPI_ANY_TAG, parentcomm );

	    // printf("p %d receive MPI_TAG %d\n", myrank, status.MPI_TAG);
        }



    MPI_Finalize( );
    for (i = 0; i < SIZE; i++){
        free(b[i]);
    }
    free(b);
    free(c);
    MPI_Comm_free( &parentcomm );
    

    // printf("worker %d out\n", myrank);
    return 0;
}
