/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				SMS-Derfflinger
**
** DATE:        2024-08-15
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
    Color *newColor = (Color *)malloc(sizeof(Color));
    if (newColor == NULL){
		perror("Error Allocating");
        return NULL;
	}

    if (image->image[row][col].B & 1) {
        newColor->B = newColor->G = newColor->R = 255;
    } else {
        newColor->B = newColor->G = newColor->R = 0;
    }

    return newColor;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	//YOUR CODE HERE
    Image *newImage = createNewImage(image->rows, image->cols);
    if (newImage == NULL) {
        return NULL;
    }

    for (int i = 0; i < image->rows; i++) {
        for (int j = 0; j < image->cols; j++) {
            Color *temp = evaluateOnePixel(image, i, j);
            newImage->image[i][j] = *temp;
            free(temp);
        }
    }

    return newImage;
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
	//YOUR CODE HERE
    Image *image = readData(argv[1]);
    if (image == NULL) {
        return -1;
    }

    Image *newImage = steganography(image);
    writeData(newImage);
    freeImage(image);
    freeImage(newImage);

    return 0;
}
