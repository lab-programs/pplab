#ifndef PPLAB_FACTORIAL_H
#define PPLAB_FACTORIAL_H

#include "stdio.h"
#include "stdlib.h"
#include "omp.h"

typedef long long ll;

ll factorial ( int n )
{
    if( n < 0 ) return 0;
    if( n == 0 || n == 1 ) return 1;
    return n * factorial(n-1);
}

void factorial_private( int n, int num_threads )
{
    ll *x;
    int i, j;

    if( omp_get_dynamic() )
    {
        omp_set_dynamic(0);
    }
    omp_set_num_threads(num_threads);

    printf("using %d num threads\n\n", omp_get_max_threads());

    x = (ll*) malloc( n * sizeof(ll) );
    for( i=0; i<n; i++ )
        x[i] = factorial(i);

    j = 0;
    #pragma omp parallel for firstprivate(x,j) 
    for( i=0; i<n; i++ )
    {
        j += i;
        x[i] = j * x[i-1]; 
    }

    for( i=0; i<n; i++ )
        printf("factorial = %lld\t\t\t\t, x[%d] = %lld\n", factorial(i), i, x[i]);

    free(x);
}

#endif