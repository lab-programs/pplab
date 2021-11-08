#ifndef PPLAB_SINPRIME_H
#define PPLAB_SINPRIME_H

#include <math.h>
#include "array.h"

static inline
void generate_prime_table( int n )
{
    array* sieve = new_arr(n+1);
    for( int i=1; i<sieve->n; i++ )
    {
        sieve->v[i] = 1;
    }
    sieve->v[0] = 0;
    sieve->v[1] = 0;
    int end = 1 + sieve->n/2;
    for( int i=2; i<end; i++ ) 
    {
        for( int j=2*i; j<sieve->n; j += i )
        {
            sieve->v[j] = 0;
        }
    }
    for( int i=1; i<sieve->n; i++ )
    {
        if( sieve->v[i] == 1 )
        {
            printf("%d ", i);
        }
    }
    printf("\n\n");
}

#endif