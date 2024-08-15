/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				SMS-Derfflinger
**
**
** DATE:        2024-08-15
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
	//YOUR CODE HERE
    FILE *file = fopen("example.txt", "r");
    if (file == NULL) {
        perror("Error opening file.");
        return NULL;
    }

    char format[3];
    if (fscanf(file, "%2s", format) != 1 || strcmp(format, "P3") != 0) {
        perror("Error reading format.");
        fclose(file);
        return NULL;
    }

    int cols, rows, max_val;
    if (fscanf(file, "%d %d", &cols, &rows) != 2) {
        perror("Error reading width and height.");
        fclose(file);
        return NULL;
    }

    if (fscanf(file, "%d", &max_val) != 1) {
        perror("Error reading max_val.");
        fclose(file);
        return NULL;
    }

    Image *newImage = (Image *)malloc(sizeof(Image));
    newImage->cols = cols;
    newImage->rows = rows;
    newImage->image = (Color **)calloc(rows, sizeof(Color *));
    if (newImage->image == NULL){
		perror("Error allocating.");
        fclose(file);
		return NULL;
	}

    for (int i = 0; i < rows; ++i){
		newImage->image[i] = (Color*)calloc(newImage->cols, sizeof(Color));
		if (newImage->image[i] == NULL){
			perror("Error allocating.");
            fclose(file);
            return NULL;
		}
	}

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf(file, "%hhu %hhu %hhu", &(newImage->image[i][j].R), &(newImage->image[i][j].G), &(newImage->image[i][j].B));
        }
    }

    return newImage;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
    printf("P3\n%d %d\n255\n", image->cols, image->rows);

    for (int i = 0; i < image->rows; i++) {
        int j;
        for (j = 0; j < image->cols - 1; j++) {
            printf("%3hhu %3hhu %3hhu   ", image->image[i][j].R, image->image[i][j].G, image->image[i][j].B);
        }
        printf("%3hhu %3hhu %3hhu\n", image->image[i][j].R, image->image[i][j].G, image->image[i][j].B);
    }
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
}