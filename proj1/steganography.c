/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	//YOUR CODE HERE
	Color *newColor = malloc(sizeof(Color));
  if (newColor == NULL) return NULL;

  uint8_t b_channal = image->image[row][col].B;

  int lsb = b_channal & 1;

  if (lsb == 1) {
    newColor->R = newColor->G = newColor->B = 255;
  } else {
    newColor->R = newColor->G = newColor->B = 0;
  }

  return newColor;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	//YOUR CODE HERE
	Image *newImg = malloc(sizeof(Image));
	if (newImg == NULL) return NULL;

  newImg->rows = image->rows;
  newImg->cols = image->cols;

  newImg->image = malloc(sizeof(Color *) * newImg->rows);
  if (newImg->image == NULL) {
    free(newImg);
    return NULL;
  }

  for (int i = 0; i < newImg->rows; i++) {
    newImg->image[i] = malloc(sizeof(Color) * newImg->cols);
    if (newImg->image[i] == NULL) {
      free(newImg);
      free(newImg->image);
      return NULL;
    }
    for (int j = 0; j < newImg->cols; j++) {
      Color *tempColor = evaluateOnePixel(image, i, j);
      if (tempColor == NULL) {
        free(newImg->image[i]);
        free(newImg->image);
        free(newImg);
        return NULL;
      }
      newImg->image[i][j] = *tempColor;
      free(tempColor);
    }
  }

  return newImg;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    return -1;
  }

  Image *originalImage = readData(argv[1]);
  if (originalImage == NULL) {
    return -1;
  }

  Image *secretImage = steganography(originalImage);
  if (secretImage == NULL) {
    freeImage(originalImage);
    return -1;
  }

  writeData(secretImage);

  freeImage(originalImage);
  freeImage(secretImage);

  return 0;
}
