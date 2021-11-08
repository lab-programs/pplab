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
            printf("%d\n", i);
        }
    }
    printf("\n\n");
    free(sieve);
}

static inline
void generate_sin_table( int n ) 
{
    double sin_table[n+1];
    double angles[n+1];
    for( int i=0; i<=n; i++ )
    {
        angles[i] = ((double)i/(double)n) * M_PI;
        sin_table[i] = sin( ((double)i/(double)n) * M_PI );
    }
    for( int i=0; i<=n; i++ )
    {
        printf("sin(%lf) = %lf\n", angles[i], sin_table[i]);
    }
    printf("\n");
}

#endif