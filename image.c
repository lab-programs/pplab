#include "image.h"
#include <stdlib.h>

int main( int argc, char** argv )
{
    if( argc != 4 )
    {
        printf("Usage: ./image in.png out.png n_threads\n");
        return 0;
    }

    else 
    {
        char *input_file_name = argv[1];
        char *output_file_name = argv[2];
        int num_threads = atoi(argv[3]);
        color_to_bw(input_file_name, output_file_name, num_threads);
    }
}