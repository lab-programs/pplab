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