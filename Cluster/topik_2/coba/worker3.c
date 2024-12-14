#include <mpi.h>
#include <stdio.h>

int main( int argc, char *argv[] )
{
    int numprocs, myrank;
    int i,j, row, SIZE;
    double dotp;
    MPI_Status status;
    MPI_Comm parentcomm;

    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &numprocs );

    MPI_Comm_get_parent( &parentcomm );
    MPI_Comm_rank( parentcomm, &myrank );
    // printf("worker %d is up!\n", myrank);

    MPI_Bcast( &SIZE, 1, MPI_INT, 0, parentcomm );
    // printf("worker %d get SIZE: %d\n", myrank, SIZE);
    
    //double (*b)[SIZE];
    //b = (double (*)[SIZE])malloc(sizeof(*b)*SIZE);

    //double **b = (double **)malloc(SIZE * sizeof(double *));
    //for (i = 0; i < SIZE; i++)
    //    b[i] = (double *)malloc(SIZE * sizeof(double));
    //double *b =  malloc(SIZE*SIZE*sizeof(double));
    //double *c = (double *)malloc(SIZE * sizeof(double));
    //double *d = (double *)malloc(SIZE * sizeof(double));
    //double* b[SIZE];
    //for (i = 0; i < SIZE; i++){
    //b[i] = (double*)malloc(SIZE * sizeof(double));
    //}
    //int *b =(int*)malloc(SIZE * SIZE * sizeof(int));
    //int *c =(int*)malloc(SIZE * sizeof(int));;
    //int *d =(int*)malloc(SIZE * sizeof(int));
    double b[SIZE][SIZE], c[SIZE], d[SIZE];
    MPI_Bcast( b, SIZE*SIZE, MPI_DOUBLE, 0, parentcomm );

    if ( myrank < SIZE ) {
        MPI_Recv(c, SIZE, MPI_DOUBLE, 0, MPI_ANY_TAG, parentcomm, &status );
        while ( status.MPI_TAG > 0 ) {
            row = status.MPI_TAG - 1;
            dotp = 0.0;
            for ( i = 0; i < SIZE; i++ ){
                for (j=0;j<SIZE;j++){
                    d[i] += c[j] * b[i][j];
                }
            }
            MPI_Send( d, SIZE, MPI_DOUBLE, 0, row+1, parentcomm );
            MPI_Recv( c, SIZE, MPI_DOUBLE, 0, MPI_ANY_TAG, parentcomm, &status );
	    // printf("p %d receive MPI_TAG %d\n", myrank, status.MPI_TAG);
        }
    }
    //for (i = 0; i < SIZE; i++){
     //   free(b[i]);
    //}
    //free(b);
    //free(c);
    //free(d);
    MPI_Comm_free( &parentcomm );
    MPI_Finalize( );

    // printf("worker %d out\n", myrank);
    return 0;
}

