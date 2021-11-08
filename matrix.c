#include "matrix.h"
#include "stdlib.h"
#include "timer.h"

int main( int argc, char** argv ) {
    int m1 = 4;
    int n1 = 3;
    int m2 = 3;
    int n2 = 2;
    if( argc == 5 )
    {
        m1 = atoi( argv[1] );
        n1 = atoi( argv[2] );
        m2 = atoi( argv[3] );
        n2 = atoi( argv[4] );
        if( m1 <= 0 || m2 <= 0 || n1 <= 0 || n2 <= 0 )
        {
            m1 = 4;
            n1 = 3;
            m2 = 3;
            n2 = 2;
        }
    }
    matrix* mat1 = new_matrix(m1, n1);
    matrix* mat2 = new_matrix(m2, n2);
    randomize_matrix(mat1);
    randomize_matrix(mat2);
    // print_matrix(mat1);
    // print_matrix(mat2);
    timer* s = new_timer();
    start_timer(s);
    matrix* ans = mat_mul_serial(mat1, mat2);
    end_timer(s);
    printf("serial time = %lf\n", duration(s));
    free(s);
    // print_matrix(ans);
    timer* p = new_timer();
    start_timer(p);
    matrix* ansp = mat_mul_parallel(mat1, mat2);
    end_timer(p);
    printf("parallel time = %lf\n", duration(p));
    // print_matrix(ansp);
    free(p);
    free_matrix(mat1);
    free_matrix(mat2);
    free_matrix(ans);
    free_matrix(ansp);
}