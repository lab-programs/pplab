#ifndef PPLAB_ARRAY_H
#define PPLAB_ARRAY_H

#include "stdio.h"
#include "stdlib.h"

/* define type as whatever your array should contain */
typedef int type;

/* structure that encapsulate a heap array and its size */
typedef struct array 
{
    /* pointer to the first element in the array */
    type *v;
    /* size of the array */
    int n;
} array;

/* function to allocate heap memeory to the array */
inline void alloc_arr( array *arr, int n )
{
    arr->n = n;
    free( arr->v );
    arr->v = ( type* ) malloc( n * sizeof(type) );
}

/* function to deallocate heap memory from the struct */
inline void dealloc_arr( array *arr )
{
    free( arr->v );
}

/* function to set the elements of the array to random */
inline void randomise_arr( array *arr, int n )
{
    /* allocate memory to the array */
    alloc_arr(arr, n);

    /* for every element in the array */
    for ( int i=0; i<arr->n; i++ )
    {
        /* set the array element to a random val */
        arr->v[i] = rand() % n;
    }

}


/* function to print the array */
inline void print_arr( array *arr )
{
    for( int i=0; i<arr->n; i++ )
    {
        /* change %d according to type */
        printf("%d ", arr->v[i]);
    }
    printf("\n");
}


#endif