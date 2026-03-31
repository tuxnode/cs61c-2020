/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
  FILE *inptr = fopen(filename, "r");    
  if (inptr == NULL) {
    perror("readData: Can't open file");
    return NULL;
  }

  Image *image = malloc(sizeof(Image));
  if (image == NULL) {
    fclose(inptr);
    return NULL;
  }

  char format[3];
  uint32_t max_val;

  if (fscanf(inptr, "%s %u %u %u", format, &image->cols, &image->rows, &max_val) != 4) {
    fprintf(stderr, "Invalid PPM header\n");
    free(image);
    fclose(inptr);
    return NULL;
  }

  image->image = (Color **)malloc(image->rows * sizeof(Color *));
  if (image->image == NULL) {
    free(image);
    fclose(inptr);
    return NULL;
  }

  for (uint32_t row = 0; row < image->rows; row++) {
    image->image[row] = (Color *)malloc(image->cols * sizeof(Color));
    for (uint32_t col = 0; col < image->cols; col++) {
      int r, g, b;
        if (fscanf(inptr, "%d %d %d", &r, &g, &b) == 3) {
          image->image[row][col].R = (uint8_t)r;
          image->image[row][col].G = (uint8_t)g;
          image->image[row][col].B = (uint8_t)b;
        }
    }
  }

  fclose(inptr);
  return image;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
	if (!image) return;

  printf("P3\n");
  printf("%u %u\n", image->cols, image->rows);
  printf("255\n");

  for (uint32_t i = 0; i < image->rows; i++) {
    for (uint32_t j = 0; j < image->cols; j++) {
      Color p = image->image[i][j];
      printf("%3hhu %3hhu %3hhu", p.R, p.G, p.B);
      if (j < image->cols - 1) printf("   ");
    }
    printf("\n");
  }
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
	if (!image) return;

  for (uint32_t i = 0; i < image->rows; i++) {
    free(image->image[i]);
  }

  free(image->image);

  free(image);
}
