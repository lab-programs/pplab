#ifndef PPLAB_MATRIX_H
#define PPLAB_MATRIX_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

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