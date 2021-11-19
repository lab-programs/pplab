# PPLAB programs 

## Concepts ( According to docs )

1. Parallel execution which are independent of each other: Use parallel sections
```c
#pragma omp parallel sections 
{
    #pragma omp section
    { ... }
    #pragma omp section
    { ... }
}
```

2. Parallelize one for loop
```c
int i;
#pragma omp parallel for
{
    for( i=0; i<n; i++)
    { ... }
}
```

3. Parallelize multiple for loops
```c
int i, j, k;
#pragma omp parallel for
{
    for( i=0; i<n1; i++ )
    {
        for( j=0; j<n2; j++ )
        {
            for( k=0; k<n3; k++ )
            { ... }
        }
    }
}
```

3. Shared: shared data between all threads
```c
int i;
#pragma omp parallel shared(i)
{ ... }
```
4. Private: created a copy for every thread - the value declared before the section is not readable and the value cant be accessed after the section
```c
int i=10;
#pragma omp parallel private(i)
{
    // here i value will not be 10
    ...
}
// here i value will be 10, any changes made in above section won't be reflected here
```
5. FirstPrivate: the value declared before the section is readable but the value cant be accessed after the section
```c
int i=10;
#pragma omp parallel firstprivate(i)
{
    // here i value is 10
    ...
}
// here i value will be 10, any changes made in above section won't be reflected here
```
6. LastPrivate: the value declared before the section is not readable but the value 
```c
int i=10;
#pragma omp parallel lastprivate(i)
{
    // here i value will not be 10
    i = 20;
}
// here i value will be 20
```
## Questions

### Part-A

1. Write parallel program using OpenMP to sort n element using merge sort. - [Merge Sort Header](merge_sort.h), [Main](merge_sort.c)

> MERGE SORT HEADER
```c
#ifndef MERGE_SORT_H
#define MERGE_SORT_H

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "array.h"

static inline 
void merge( array* arr, int left, int mid, int right ) {
    /// for the first half of the range of the array
    int start1 = left;
    int end1 = mid;

    /// for the last half of the range of the array
    int start2 = mid+1;
    int end2 = right;

    /// start the merging process until start1 <= end1 && start2 <= end2
    while( start1 <= end1 && start2 <= end2 ) {

        if( arr->v[start1] <= arr->v[start2] ) {
            start1++;
            continue;
        }

        /// shift all elements right by one and insert in the right place
        int val = arr->v[start2];
        for( int i=start2; i>start1; i-- )
            arr->v[i] = arr->v[i-1];
        arr->v[start1] = val;

        /// increment the pointers
        start1++;
        start2++;
        end1++;
    }
}

/// merge sort using serial programming
static inline 
void merge_sort_serial( array* arr, int left, int right ) {
    /// base case: if left is greater than or equal to right, then : it means there are no more elements to sort
    if( left >= right ) return;
    
    /// mid is the middle of the range of the array that is being considered
    int mid = left + (right - left) / 2;
    
    /// sort the left half of the range of the array
    merge_sort_serial( arr, left, mid );
    
    /// sort the right half of the range of the array
    merge_sort_serial( arr, mid+1, right );
    
    /// merge the two halves of the array
    merge( arr, left, mid, right);
}

/// merge sort using parallel programming
static inline 
void merge_sort_parallel( array* arr, int left, int right ) {
    /// base case: if left is greater than or equal to right, then : it means there are no more elements to sort
    if( left >= right ) return;

    /// mid is the middle of the range of the array that is being considered
    int mid = left + (right - left) / 2;

    #pragma omp parallel section
    {
        #pragma omp section
            merge_sort_parallel( arr, left, mid );
        #pragma omp section
            merge_sort_parallel( arr, mid+1, right );
    }
    
    /// merge the two halves of the array
    merge( arr, left, mid, right);
}
#endif
```

> MERGE SORT MAIN
```c
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
```

2. Write a program to Multiply a matrix by a vector and get the result of the operation. - [Matrix Header](matrix.h), [Main](matrix.c)

> MATRIX HEADER
```c
#ifndef PPLAB_MATRIX_H
#define PPLAB_MATRIX_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

typedef struct {
    int** v;
    int m;
    int n;
} matrix;

static inline
matrix* new_matrix( int m, int n )
{
    matrix *mat = (matrix*) malloc( sizeof(matrix) );
    mat->m = m;
    mat->n = n;
    mat->v = (int**) malloc( m * sizeof(int*) );
    for ( int i=0; i<m; i++ )
    {
        mat->v[i] = (int*) malloc( n * sizeof(int) );
    }
    return mat;
}

static inline
void free_matrix( matrix* mat )
{
    for( int i=0; i<mat->m; i++ )
        free( mat->v[i] );
    free( mat->v );
    free( mat );
}

static inline
void randomize_matrix( matrix* mat )
{
    srand(time(0));
    for ( int i=0; i<mat->m; i++ )
    {
        for ( int j=0; j<mat->n; j++ )
        {
            mat->v[i][j] = 10 + rand() % 90;
        }
    }
}

static inline
void print_matrix( matrix* mat )
{
    printf("Order: %dx%d\n", mat->m, mat->n);
    printf("Matrix:\n");
    if( mat == NULL )
        return;
    for( int i=0; i<mat->m; i++ )
    {
        for( int j=0; j<mat->n; j++ )
        {
            printf("%d ", mat->v[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

static inline
matrix* mat_mul_serial( matrix *m1, matrix *m2 )
{
    if( m1->n != m2->m )
    {
        printf("Matrix cant be multiplied");
        return NULL;
    }
    matrix* ans = new_matrix( m1->m, m2->n );
    for( int i=0; i<m1->m; i++ )
    {
        for( int j=0; j<m2->n; j++ )
        {
            ans->v[i][j] = 0;
            for( int k=0; k<m1->n; k++ )
            {
                ans->v[i][j] += (m1->v[i][k] * m2->v[k][j]);
            }
        }
    }
    return ans;
}

static inline
matrix* mat_mul_parallel( matrix *m1, matrix *m2 )
{
    if( m1->n != m2->m )
    {
        printf("Matrix cant be multiplied");
        return NULL;
    }
    int i,j,k;
    matrix* ans = new_matrix( m1->m, m2->n );

    #pragma omp parallel for private(i,j,k) shared(m1, m2, ans)
    for( i=0; i<m1->m; i++ )
    {
        for( j=0; j<m2->n; j++ )
        {
            ans->v[i][j] = 0;
            for( k=0; k<m1->n; k++ )
            {
                ans->v[i][j] += (m1->v[i][k] * m2->v[k][j]);
            }
        }
    }
    return ans;
}

#endif
```

> MATRIX MAIN
```c
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
```

3. Write an OpenMP program which demonstrates how to "multitask",
implement two separate task, one to generate prime table and other to
generate sine table for a given input using OpenMP for parallel execution.
Justify the inference. - [Sinprime Header](sinprime.h), [Main](sinprime.c)

> SINPRIME HEADER
```c
#ifndef PPLAB_SINPRIME_H
#define PPLAB_SINPRIME_H

#include <math.h>
#include "array.h"

static inline
void generate_prime_table( int n )
{
    array* sieve = new_arr(n+1);
    for( int i=1; i<sieve->n; i++ )
    {
        sieve->v[i] = 1;
    }
    sieve->v[0] = 0;
    sieve->v[1] = 0;
    int end = 1 + sieve->n/2;
    for( int i=2; i<end; i++ ) 
    {
        for( int j=2*i; j<sieve->n; j += i )
        {
            sieve->v[j] = 0;
        }
    }
    for( int i=1; i<sieve->n; i++ )
    {
        if( sieve->v[i] == 1 )
        {
            printf("%d\n", i);
        }
    }
    printf("\n\n");
    free(sieve);
}

static inline
void generate_sin_table( int n ) 
{
    double sin_table[n+1];
    double angles[n+1];
    for( int i=0; i<=n; i++ )
    {
        angles[i] = ((double)i/(double)n) * M_PI;
        sin_table[i] = sin( ((double)i/(double)n) * M_PI );
    }
    for( int i=0; i<=n; i++ )
    {
        printf("sin(%lf) = %lf\n", angles[i], sin_table[i]);
    }
    printf("\n");
}

#endif
```

> SINPRIME MAIN
```c
#include "sinprime.h"
#include "stdlib.h"
#include "omp.h"
#include "timer.h"

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

    timer* p = new_timer();
    start_timer(p);
    #pragma omp parallel sections
    {
        #pragma omp section
            generate_prime_table(n);
        #pragma omp section
            generate_sin_table(n);
    }
    end_timer(p);

    printf("time taken : %lf sec\n", duration(p));
    
}
```

4. Write a program to show how first private clause works. (Factorial program) - [Factorial Header](factorial.h), [Main](factorial.c)

> FACTORIAL HEADER

```c
#ifndef PPLAB_FACTORIAL_H
#define PPLAB_FACTORIAL_H

#include "stdio.h"
#include "stdlib.h"
#include "omp.h"

typedef long long ll;

ll factorial ( int n )
{
    if( n < 0 ) return 0;
    if( n == 0 || n == 1 ) return 1;
    return n * factorial(n-1);
}

void factorial_private( int n, int num_threads )
{
    ll *x;
    int i, j;

    if( omp_get_dynamic() )
    {
        omp_set_dynamic(0);
    }
    omp_set_num_threads(num_threads);

    printf("using %d num threads\n\n", omp_get_max_threads());

    x = (ll*) malloc( n * sizeof(ll) );
    for( i=0; i<n; i++ )
        x[i] = factorial(i);

    j = 0;
    #pragma omp parallel for firstprivate(x,j) 
    for( i=0; i<n; i++ )
    {
        j += i;
        x[i] = j * x[i-1]; 
    }

    for( i=0; i<n; i++ )
        printf("factorial = %lld\t\t\t\t, x[%d] = %lld\n", factorial(i), i, x[i]);

    free(x);
}

#endif
```

> FACTORIAL MAIN

```c
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
```

5. Write an OpenMP parallel program for Points Classification. Prove the correctness of sequential program with that of parallel. [Points Header](points.h), [Main](points.c)

> POINTS HEADER
```c
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
```

> POINTS MAIN
```c
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
```

6. Write an OpenMP program to convert a color image to black and white image. Demonstrate the performance of different scheduling techniques for varying chunk values. [Image Header](image.h), [Main](image.c)

> IMAGE HEADER
```c
#ifndef PPLAB_IMAGE_H
#define PPLAB_IMAGE_H

#include "stdio.h"
#include "string.h"
#include "gd.h"
#include "omp.h"
#include "time.h"

static inline
FILE* open_file( const char *file_name, const char *mode )
{
    FILE *fp = fopen(file_name, "r");
    if ( fp == NULL ) 
    {
        printf("Error opening file %s\n", file_name);
        exit(1);
    }
    return fp;
}

static inline
void color_to_bw( char *input_file_name, char *output_file_name, int num_threads )
{
    /* open input file in read mode */
    FILE *input_file = open_file(input_file_name, "r");

    /* create an gdImagePtr image from the file and get its dimensions */
    gdImagePtr image = gdImageCreateFromPng(input_file);
    int width = gdImageSX(image);
    int height = gdImageSY(image);

    /* variables which will contain the 8-bit intensity values */
    int red, blue, green, color;
    
    /* start timer */
    double start = omp_get_wtime();

    /* start task */
    #pragma omp parallel for 
        for( int i=0; i<width; i++ )
        {
            for( int j=0; j<height; j++ )
            {
                /* get the pixel */
                color = gdImageGetPixel(image, i, j);
                /* get the 8 bit intensity values for rgb */
                red = 255 - gdImageRed(image, color);
                green = 255 - gdImageGreen(image, color);
                blue = 255 - gdImageBlue(image, color);
                /* get the grayscale image intensity value */
                color  = gdImageColorAllocate(image, red, green, blue);
                /* set the pixel with the grayscale intensity */
                gdImageSetPixel(image, i, j, color);
            }
        }
    /* end task */
    
    /* end timer */
    double end = omp_get_wtime();
    /* calculate duration */
    double duration = (end - start) / CLOCKS_PER_SEC;

    /* open the output file */
    FILE *output_file = fopen(output_file_name, "w");
    /* create the file */
    freopen(output_file_name, "w", output_file);
    /* save the image to the output_file */
    gdImagePng(image, output_file);

    printf("image size: %dx%d\n", width, height);
    printf("time taken = %lf\n", duration);

    /* free memory */
    gdImageDestroy(image);
    fclose(input_file);
    fclose(output_file);
}

#endif
```

> IMAGE MAIN
```c
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
```

### Part-B

7. Write a program for communication among two processes.
> Header
```c
#ifndef PPLAB_PROC_COM_H
#define PPLAB_PROC_COM_H

#include "stdio.h"
#include "stdlib.h"
#include "signal.h"
#include "sys/types.h"
#include "unistd.h"

static inline
void sighup( int );
static inline
void sigint( int );
static inline
void sigquit( int );

static inline
void proc_com()
{
    int pid = fork();
    if( pid < 0 )
    {
        perror("process not found");
        exit(1);
    }

    if( pid == 0 )
    {
        /* child process */
        signal( SIGHUP, sighup );
        signal( SIGINT, sigint );
        signal( SIGQUIT, sigquit );
        for(;;);
    }
    else
    {
        /* parent process */
        printf("parent process sent sighup signal\n");
        kill( pid, SIGHUP );
        sleep(3);

        printf("parent process sent sigint signal\n");
        kill( pid, SIGINT );
        sleep(3);
        
        printf("parent process sent sigquit signal\n");
        kill( pid, SIGQUIT );
        sleep(3);
    }
}

static inline
void sighup( int pid )
{
    signal( SIGHUP, sighup );
    printf("child process received sighup signal\n");
}

static inline
void sigint( int pid )
{
    signal( SIGINT, sigint );
    printf("child process received sigint signal\n");
}

static inline
void sigquit( int pid )
{
    signal( SIGQUIT, sigquit );
    printf("child process got destroyed\n");
}

#endif
```
> Main Program
```c
#include "proc_com.h"

int main( int argc, char **argv )
{
    proc_com();
}
```

8. Write MPI program to compute dot product of two vectors using blockstriped 
partitioning with uniform data distribution.

```c
#ifndef PPLAB_DOTPRODUCT_H
#define PPLAB_DOTPRODUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>

typedef struct {
    int *v;
    int n;
} vector;

static inline
vector* new_vector( int n )
{
    vector* vect = (vector*) malloc( sizeof(vector) );
    vect->v = (int*) malloc( n * sizeof(int) );
    vect->n = n;
    return vect;
}

static inline
void free_vector( vector *vect )
{
    if( vect == NULL ) return;
    if( vect->v == NULL )
    {
        free( vect );
        return;
    }
    free(vect->v);
    free(vect);
} 

#endif
```

```c
#include "dotproduct.h"

int main( int argc, char **argv )
{
    int i, id, sum, global_sum;
    int n_proc = 2;
    int vect_size = 100;
    if( argc == 2 )
    {
        vect_size = atoi(argv[1]);
    }
    /* initialize MPI */
    MPI_Init(&argc, &argv);
    /* get the number of processors for MPI_COMM_WORLD method */
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);
    /* get the rank of MPI_COMM_WORLD method */
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    
    /* start the jobs */
    if( id == 0 )
        printf("Starting %d jobs to find the dot product:\n", n_proc);

    /* create vectors and assign values */
    vector* a = new_vector(vect_size);
    vector* b = new_vector(vect_size);
    for( i=0; i<vect_size; i++ )
    {
        a->v[i] = i*2;
        b->v[i] = i;
    }
    sum = 0;
    for( i=0; i<vect_size; i++ )
        sum += a->v[i] * b->v[i];
    
    printf("Task %d partial sum = %d\n", id, sum);

    MPI_Reduce(&sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if( id == 0 )
        printf("Done MPI, global sum = %d\n", global_sum);

    free_vector(a);
    free_vector(b);
    MPI_Finalize();
}
```

9. Write MPI program that computes the value of PI using Monto-Carlo
Algorithm.

```c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi/mpi.h>

#define SEED 847965282

int main( int argc, char **argv )
{
    int n_iter = 100000;
    int i, id;
    double x, y, z;
    int count = 0;
    double pi;
    int n_proc;

    if( argc == 2 )
    {
        n_iter = atoi(argv[1]);
    }
    printf("number of iterations = %d\n", n_iter);
    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &n_proc );
    MPI_Comm_rank( MPI_COMM_WORLD, &id );

    srand(SEED + id); 
    int received[n_proc];
    int received_n_iter[n_proc];

    /* if not the root process */
    if( id != 0 )
    {
        for( i=0; i<n_iter; i++ )
        {
            x = (double)rand() / (double)RAND_MAX;
            y = (double)rand() / (double)RAND_MAX;
            /* calculate distance from the center */
            z = sqrt(x*x + y*y);
            /* if z <= 1, the point is inside the circle */
            if( z <= 1 )
                count++;
        }

        for( i=0; i<n_proc; i++ )
        {
            MPI_Send(&count, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
            MPI_Send(&n_iter, 1, MPI_LONG, 0, 2, MPI_COMM_WORLD);
        }
    }
    else {
        /* if it is the root process */
        for( int i=0; i<n_proc; i++ )
        {
            MPI_Recv(&received[i], 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&received_n_iter[i], 1, MPI_LONG, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    if( id == 0 )
    {
        int final_count = 0;
        long final_n_iter = 0;
        for( i=0; i<n_proc; i++ )
        {
            final_count += received[i];
            final_n_iter += received_n_iter[i];
        }
        pi = ((double)final_count / (double)final_n_iter) * 4.0;
        printf("PI = %lf\n", pi);
    }
    /* terminate */
    MPI_Finalize();
}
```

10. C program which creates new communicators involving a subset of initial set
of MPI processes in the default communicator MPI_COMM_WORLD

11. Write MPI program to compute Matrix-Matrix Multiplication using selfscheduling algorithm.

12. C program which searches integers between A and B for a value J such that
F(J) = C, using the MPI parallel programming environment
