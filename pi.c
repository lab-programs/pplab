#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi/mpi.h>

#define SEED 847965282

int main( int argc, char **argv )
{
    int n_iter = 100000;
    int i, id;
    double x, y, z;
    int count = 0;
    double pi;
    int n_proc;

    if( argc == 2 )
    {
        n_iter = atoi(argv[1]);
    }
    printf("number of iterations = %d\n", n_iter);
    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &n_proc );
    MPI_Comm_rank( MPI_COMM_WORLD, &id );

    srand(SEED + id); 
    int received[n_proc];
    int received_n_iter[n_proc];

    /* if not the root process */
    if( id != 0 )
    {
        for( i=0; i<n_iter; i++ )
        {
            x = (double)rand() / (double)RAND_MAX;
            y = (double)rand() / (double)RAND_MAX;
            /* calculate distance from the center */
            z = sqrt(x*x + y*y);
            /* if z <= 1, the point is inside the circle */
            if( z <= 1 )
                count++;
        }

        for( i=0; i<n_proc; i++ )
        {
            MPI_Send(&count, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
            MPI_Send(&n_iter, 1, MPI_LONG, 0, 2, MPI_COMM_WORLD);
        }
    }
    else {
        /* if it is the root process */
        for( int i=0; i<n_proc; i++ )
        {
            MPI_Recv(&received[i], 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&received_n_iter[i], 1, MPI_LONG, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    if( id == 0 )
    {
        int final_count = 0;
        long final_n_iter = 0;
        for( i=0; i<n_proc; i++ )
        {
            final_count += received[i];
            final_n_iter += received_n_iter[i];
        }
        pi = ((double)final_count / (double)final_n_iter) * 4.0;
        printf("PI = %lf\n", pi);
    }
    /* terminate */
    MPI_Finalize();
}