#include "array.h"
#include "timer.h"
#include "stdio.h"

int main()
{
    array *a = new_array(10);
    timer *t = new_timer();

    start_timer(t);
    
    randomise_arr(a);
    print_arr(a);
    
    end_timer(t);
    printf("duration = %lf\n", duration(t));

    free(a);
    free(t);
}