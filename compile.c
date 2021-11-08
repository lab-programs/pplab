#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char **argv ) {

    if( argc <= 1 ) {
        printf("No file provided");
        return 0;
    }

    char s[256] = "gcc ";
    strcat( s, argv[1] );
    strcat( s, ".c -o " );
    strcat( s, argv[1] );
    strcat( s, " -fopenmp" );
    strcat( s, " -lm" );
    system( s );
    
}