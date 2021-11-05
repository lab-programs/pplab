#ifndef PPLAB_TIMER_H
#define PPLAB_TIMER_H

#include "time.h"
#include "stdlib.h"
#include "stdlib.h"

typedef struct timer {
    clock_t start_time;
    clock_t end_time;
} timer;

static inline
timer* new_timer()
{
    return (timer*) malloc( sizeof(timer) );
}

static inline 
void start_timer( timer *t )
{
    t->start_time = clock();
}

static inline 
void end_timer( timer *t )
{
    t->end_time = clock();
}

static inline
double duration( timer *t )
{
    return (double)(t->end_time - t->start_time) / (double)CLOCKS_PER_SEC;
}

#endif