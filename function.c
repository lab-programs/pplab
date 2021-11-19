#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>

long F(long a){
  return 2*a;
}


int search( long a, long b, long c, int id, int n_proc )
{
    for( long i=a+id; 0<=i && i<=b; i+=n_proc ) {
        if( F(i) == c ) {
            printf("process id %d found at location %ld!\n", id, i);
            return 1;
        }
    }
    return 0;
}


int main(){
    int rank,n_proc;
    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&n_proc);
    search(1, 1000, 22, rank, n_proc);
    search(1, 1000, 90, rank, n_proc);
    MPI_Finalize();
    return 0;
}