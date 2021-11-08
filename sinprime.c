#include "sinprime.h"
#include <stdlib.h>

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
    generate_prime_table(n);
}