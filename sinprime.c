#include "sinprime.h"
#include "stdlib.h"
#include "omp.h"
#include "timer.h"

int main( int argc, char** argv )
{
    int n = 50;
    if( argc == 2 )
    {
        n = atoi( argv[1] );
        if( n <= 1 )
        {
            n = 50;
        }
    }

    timer* p = new_timer();
    start_timer(p);
    #pragma omp parallel sections
    {
        #pragma omp section
            generate_prime_table(n);
        #pragma omp section
            generate_sin_table(n);
    }
    end_timer(p);

    printf("time taken : %lf sec\n", duration(p));
    
}