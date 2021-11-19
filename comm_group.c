#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>

int main(){
    int rank, size, even_sum, odd_sum, even_id, odd_id;
    MPI_Comm even_comm,odd_comm;
    MPI_Group even_group,odd_group,world_group;

    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    MPI_Comm_group(MPI_COMM_WORLD, &world_group);

    int even_p = (size+1)/2;
    int* even_rank = (int*) malloc( even_p * sizeof(int) );

    for( int i=0,j=0; i<size; i+=2, j++ )
        even_rank[j]=i;

    MPI_Group_incl(world_group,even_p,even_rank,&even_group);
    MPI_Comm_create(MPI_COMM_WORLD,even_group,&even_comm);

    int odd_p = size/2;
    int* odd_rank = (int*) malloc( odd_p * sizeof(int) );

    for( int i=1, j=0; i<size; i+=2, j++ )
        odd_rank[j]=i;

    MPI_Group_incl(world_group,odd_p,odd_rank,&odd_group);
    MPI_Comm_create(MPI_COMM_WORLD,odd_group,&odd_comm);
    
    if(rank%2 == 0){
        MPI_Comm_rank(even_comm, &even_id);
        odd_id = -1;
    }
    else {
        MPI_Comm_rank(odd_comm, &odd_id);
        even_id = -1;
    }
    
    if( odd_id != -1 )
        MPI_Reduce(&rank, &odd_sum, 1, MPI_INT, MPI_SUM, 0, odd_comm);
    if( even_id != -1 )
        MPI_Reduce(&rank, &even_sum, 1, MPI_INT, MPI_SUM, 0, even_comm);
    
    if( odd_id == 0 )
        printf("odd sum = %d\n", odd_sum);
    if( even_id == 0 )
        printf("even sum = %d\n", even_sum);

    MPI_Finalize();
    return 0;
}