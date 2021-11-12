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