#ifndef PPLAB_DOTPRODUCT_H
#define PPLAB_DOTPRODUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>

typedef struct {
    int *v;
    int n;
} vector;

static inline
vector* new_vector( int n )
{
    vector* vect = (vector*) malloc( sizeof(vector) );
    vect->v = (int*) malloc( n * sizeof(int) );
    vect->n = n;
    return vect;
}

static inline
void free_vector( vector *vect )
{
    if( vect == NULL ) return;
    if( vect->v == NULL )
    {
        free( vect );
        return;
    }
    free(vect->v);
    free(vect);
} 

#endif