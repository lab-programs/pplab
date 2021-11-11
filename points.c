#include "points.h"

int main( int argc, char** argv ) {
    int n_points = 20;
    if( argc == 2 )
    {
        n_points = atoi(argv[1]);
    }
    points p = random_points(n_points);
    cluster c1 = get_cluster();
    cluster c2 = get_cluster();

    cluster_points_serial(p, n_points, c1);
    cluster_points_parallel(p, n_points, c2);

    printf("serial ans: ");
    for( int i=0; i<c1.n; i++ ) {
        printf("%d ", c1.counts[i]);
    }
    printf("\nparallel ans: ");
    for( int i=0; i<c2.n; i++ ) {
        printf("%d ", c1.counts[i]);
    }
    printf("\n");
    free(p);
    free(c1.centers);
    free(c1.counts);
    free(c2.centers);
    free(c2.counts);
}