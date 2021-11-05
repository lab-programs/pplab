#ifndef PPLAB_ARRAY_H
#define PPLAB_ARRAY_H

#include "stdio.h"
#include "stdlib.h"
#include "time.h"

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
static inline 
array* new_arr( int n )
{
    array *arr = (array*) malloc( sizeof(arr) );
    arr->n = n;
    arr->v = ( type* ) malloc( n * sizeof(type) );
    return arr;
}

/* function to deallocate heap memory allocated to array */
static inline
void free_arr( array *a ) {
    if ( a != NULL ) {
        free(a->v);
        free(a);
    }
}


/* function to set the elements of the array to random */
static inline 
void randomise_arr( array *arr )
{
    /* create a random seed */
    srand(time(0));

    /* for every element in the array */
    for ( int i=0; i<arr->n; i++ )
    {
        /* set the array element to a random val */
        arr->v[i] = rand() % arr->n;
    }

}


/* function to print the array */
static inline 
void print_arr( array *arr )
{
    for( int i=0; i<arr->n; i++ )
    {
        /* change %d according to type */
        printf("%d ", arr->v[i]);
    }
    printf("\n");
}

static inline
int min( int a, int b )
{
    return (a < b) ? a : b;
}

static inline
void copy_arr( array* a, array *b )
{
    for( int i=0; i<min(a->n, b->n); i++ )
    {
        b->v[i] = a->v[i];
    }
}

#endif