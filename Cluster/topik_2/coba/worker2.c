#include <mpi.h>
#include <stdio.h>

int main( int argc, char *argv[] )
{
    int numprocs, myrank;
    int i,j,k, row, rows, SIZE;
    MPI_Status status;
    MPI_Comm parentcomm;

    MPI_Init(&argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &numprocs );

    MPI_Comm_get_parent( &parentcomm );
    MPI_Comm_rank( parentcomm, &myrank );
    // printf("worker %d is up!\n", myrank);

    MPI_Bcast(&SIZE, 1, MPI_INT, 0, parentcomm );
    // printf("worker %d get SIZE: %d\n", myrank, SIZE);

    MPI_Bcast(&rows, 1, MPI_INT, 0, parentcomm);

    double b[SIZE][SIZE], a[SIZE][SIZE], c[SIZE][SIZE];
    MPI_Bcast(b, SIZE*SIZE, MPI_DOUBLE, 0, parentcomm );

    if ( myrank < SIZE ) {
        MPI_Recv(&c, SIZE, MPI_DOUBLE, 0, MPI_ANY_TAG, parentcomm, &status );
        while ( status.MPI_TAG > 0 ) {
            row = status.MPI_TAG - 1;
            for (k=0; k<SIZE; k++){
                for (i=0; i<rows; i++){
                    c[i][k] = 0.0;
                    for (j=0; j<SIZE; j++){
                        c[i][k] = c[i][k] + a[i][j] * b[j][k];
                    }

                }
            }

            // for ( i = 0; i < SIZE; i++ ){
            //     for (j=0;j<SIZE;j++){
            //         d[i] += c[j] * b[j][i];
            //     }
            // }
            MPI_Send(c, SIZE*SIZE, MPI_DOUBLE, 0, row+1, parentcomm);
            MPI_Recv(a, SIZE*SIZE, MPI_DOUBLE, 0, MPI_ANY_TAG, parentcomm, &status);
	    // printf("p %d receive MPI_TAG %d\n", myrank, status.MPI_TAG);
        }
    }

    MPI_Comm_free( &parentcomm );
    MPI_Finalize( );

    // printf("worker %d out\n", myrank);
    return 0;
}

