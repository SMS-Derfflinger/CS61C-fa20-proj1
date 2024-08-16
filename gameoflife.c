/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				SMS-Derfflinger
**
**
** DATE:        2024-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

// The surrounding health state corresponds to the survival state
const int alives[9] = {0, 0, 1, 1, 0, 0, 0, 0, 0};
const int deads[9] = {0, 0, 0, 1, 0, 0, 0, 0, 0};

typedef enum RGBchoice {
    RED,
    GREEN,
    BLUE
}RGBchoice;

unsigned int getBit(uint8_t bytes, int bits) {
    return (bytes >> bits) & 1;
}

void setBit(uint8_t *bytes, int bits, int value) {
    if (value) {
        *bytes = *bytes | (1 << bits);  // set nth bit to 1
    } else {
        *bytes = *bytes & (~(1 << bits)); // set nth bit to 0
    }
}

uint8_t getRGB(Image *image, int row, int col, RGBchoice choice) {
    uint8_t temp;
    switch (choice)
    {
    case RED:
        temp = image->image[row][col].R;
        break;
    case GREEN:
        temp = image->image[row][col].G;
        break;
    case BLUE:
        temp = image->image[row][col].G;
        break;
    default:
        break;
    }

    return temp;
}

int getRealCoordinate(int origin, int offset, int total) {
    int temp = origin + offset;
    while (temp < 0){
        temp += total;
    }

    return temp % total;
}

void setRGB(uint8_t *RGB, Image *image, int row, int col, RGBchoice choice) {
    uint8_t tempRGB;

    // uint8_t's 8 bits
    for (int i = 0; i < 8; i++) {
        int neighbors = 0;
        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                int realRow = getRealCoordinate(row, y, image->rows);
                int realCol = getRealCoordinate(col, x, image->cols);
                tempRGB = getRGB(image, realRow, realCol, choice);
                if (getBit(tempRGB, i) == 1) {
                    neighbors++;
                }
            }
        }

        if (getBit(*RGB, i) == 1) {
            setBit(RGB, i, alives[neighbors]);
        } else {
            setBit(RGB, i, deads[neighbors]);
        }
    }
}

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	//YOUR CODE HERE
    Color *color = (Color*)malloc(sizeof(Color));
    if (color == NULL) {
        perror("Error Allocating");
        return NULL;
    }

    color->R = image->image[row][col].R;
    color->G = image->image[row][col].G;
    color->B = image->image[row][col].B;

    setRGB(&color->R, image, row, col, RED);
    setRGB(&color->G, image, row, col, GREEN);
    setRGB(&color->B, image, row, col, BLUE);

    return color;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	//YOUR CODE HERE
    Image *newImage = createNewImage(image->rows, image->cols);
    if (newImage == NULL) {
        return NULL;
    }

    for (int i = 0; i < image->rows; i++) {
        for (int j = 0; j < image->cols; j++) {
            Color *temp = evaluateOneCell(image, i, j, rule);
            newImage->image[i][j] = *temp;
            free(temp);
        }
    }

    return newImage;
}

void processCLI(int argc, char **argv, char **filename, uint32_t *rule) 
{
	if (argc != 3) {
		printf("usage: %s filename\n", argv[0]);
		printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
		printf("rule is a hex number beginning with 0x; Life is 0x1808.\n");
		exit(-1);
	}
	*filename = argv[1];
	char *endPtr;
	unsigned long hexValue = strtoul(argv[2], &endPtr, 16);
	*rule = (uint32_t)hexValue;
	if (!(*rule <= 0x3FFFF && *rule >= 0x00000)){
		printf("rule is out of range\n");
		exit(-1);
	}
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
    Image *image;
	char *filename;
	uint32_t rule;
	processCLI(argc, argv, &filename, &rule);

	image = readData(filename);
    Image *newImage = life(image, rule);
    if (image == NULL || newImage == NULL) {
        return -1;
    }

    writeData(newImage);
	freeImage(image);
	freeImage(newImage);

    return 0;
}
