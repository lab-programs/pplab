#include "dotproduct.h"

int main( int argc, char **argv )
{
    int i, id, sum, global_sum;
    int n_proc = 2;
    int vect_size = 100;
    if( argc == 2 )
    {
        vect_size = atoi(argv[1]);
    }
    /* initialize MPI */
    MPI_Init(&argc, &argv);
    /* get the number of processors for MPI_COMM_WORLD method */
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);
    /* get the rank of MPI_COMM_WORLD method */
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    
    /* start the jobs */
    if( id == 0 )
        printf("Starting %d jobs to find the dot product:\n", n_proc);

    /* create vectors and assign values */
    vector* a = new_vector(vect_size);
    vector* b = new_vector(vect_size);
    for( i=0; i<vect_size; i++ )
    {
        a->v[i] = i*2;
        b->v[i] = i;
    }
    sum = 0;
    for( i=0; i<vect_size; i++ )
        sum += a->v[i] * b->v[i];
    
    printf("Task %d partial sum = %d\n", id, sum);

    MPI_Reduce(&sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if( id == 0 )
        printf("Done MPI, global sum = %d\n", global_sum);

    free_vector(a);
    free_vector(b);
    MPI_Finalize();
}