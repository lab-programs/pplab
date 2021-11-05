#include "stdio.h"
#include "array.h"
#include "timer.h"
#include "merge_sort.h"

int main( int argc, char** argv ) {
    int n = 20;
    if( argc == 2 ) 
    {
        int temp = atoi(argv[1]);
        if( temp > 1 )
            n = temp;
    }
    array* a = new_arr(n);
    array* b = new_arr(n);
    timer* s = new_timer();
    timer* p = new_timer();
    randomise_arr(a);
    copy_arr(a, b);

    // printf("before sorting: \n");
    // print_arr(a);

    start_timer(s);
    merge_sort_serial(a, 0, n-1);
    end_timer(s);

    printf("serial time: %lf\n", duration(s));

    start_timer(p);
    merge_sort_parallel(b, 0, n-1);
    end_timer(p);

    printf("parallel time: %lf\n", duration(p));

    // printf("after sorting: \n");
    // print_arr(a);
    // print_arr(b);
    free_arr(a);
    free_arr(b);
    free(s);
    free(p);
}