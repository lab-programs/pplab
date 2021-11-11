#include "factorial.h"

int main( int argc, char** argv )
{
    int num_threads = 1;
    int n = 12;
    if( argc == 2 )
    {
        num_threads = atoi(argv[1]);
    }
    factorial_private(num_threads, n);
}