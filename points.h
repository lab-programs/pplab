#ifndef PPLAB_POINTS_H
#define PPLAB_POINTS_H

#include "stdio.h"
#include "stdlib.h"
#include "omp.h"
#include "time.h"
#include "limits.h"

typedef struct {
    int x, y;
} point;
typedef point* points;

typedef struct {
    int n;
    points centers;
    int *counts;
} cluster;

static inline
cluster get_cluster() {
    cluster c;
    c.n = 4;
    c.centers = (points) malloc( 4 * sizeof(point) );
    c.centers[0].x = 25;
    c.centers[0].y = 25;
    c.centers[1].x = 25;
    c.centers[1].y = 75;
    c.centers[2].x = 75;
    c.centers[2].y = 25;
    c.centers[3].x = 75;
    c.centers[3].y = 75;
    c.counts = (int*) malloc( 4 * sizeof(int) );
    c.counts[0] = 0;
    c.counts[1] = 0;
    c.counts[2] = 0;
    c.counts[3] = 0;
    return c;
}

static inline
points random_points( int size )
{
    points p = (points) malloc( size * sizeof(point) );
    srand(time(0));
    for ( int i=0; i<size; i++ ) {
        p[i].x = rand() % 100;
        p[i].y = rand() % 100;
    }
    return p;
}

static inline 
double distance( point p1, point p2 )
{
    int xdiff = abs(p1.x - p2.x);
    int ydiff = abs(p1.y - p2.y);
    return xdiff * xdiff + ydiff * ydiff;
}

static inline
void cluster_points_serial( points p, int size, cluster c )
{
    long i;
    for( i=0; i<size; i++ ) {
        int min_dist = INT_MAX;
        int curr_dist, cluster_index = -1;

        for( int j=0; j<c.n; j++ )
        {
            curr_dist = distance(p[i], c.centers[j]);
            if( curr_dist < min_dist )
            {
                min_dist = curr_dist;
                cluster_index = j;
            }
        }

        c.counts[cluster_index]++;
    }
}

static inline
void cluster_points_parallel( points p, int size, cluster c )
{
    long i;
    #pragma omp parallel for
    for( i=0; i<size; i++ ) {
        int min_dist = INT_MAX;
        int curr_dist, cluster_index = -1;

        for( int j=0; j<c.n; j++ )
        {
            curr_dist = distance(p[i], c.centers[j]);
            if( curr_dist < min_dist )
            {
                min_dist = curr_dist;
                cluster_index = j;
            }
        }

        c.counts[cluster_index]++;
    }
}

#endif