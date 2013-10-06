/*
 * VideoHandler.c
 *
 * 	Created on: 2013-10-4
 * 	Author: Wesley Tsai
 *
 * 	This file abstracts dealing with writing to the forground/background video buffers.
 * 	It allows drawing and printing to the screen without any knowledge of buffers and addresses.
 *
 * 	How to: First call VideoHandlerInit(), then proceed to use the provided functions to draw to the screen.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "VideoHandler.h"

static VideoBuffer* Video;
static CharBuffer* Character;

/*
 * VideoDemo() contains what using the VideoHandler would look like in the main loop.
 * It should be removed in the final product.
 */
void VideoDemo()
{
	//Initialize the VideoHandler
	VideoHandlerInit();

	// Draw a white line and purple line to the foreground buffer
	drawLine(0, 0, 320, 240, 0xFFFF);
	drawLine(0, 240, 320, 0, 0xF154);

	// Write some text
	printString("Nios II Hardware is so gud", 2,2);

	drawPixel(31, 1, 1);
	drawPixel(32, 2, 2);

	display();
}

/*
 * Demo on how to draw to backbuffer and then switch buffers to create animation
 *
 */
void VideoBufferSwapDemo(unsigned char randomVal)
{
	drawLine(0, 120, 50, 100+randomVal%100 , 0xFFFF-randomVal);
	drawPixel(0xFFFF, randomVal, randomVal);
	display();
}

/*
 * Intializes the static Video and Character buffers contained in this file
 */
void VideoHandlerInit()
{
	Video = Video_New();
	Character = Char_New();
}

/*
 * Draws a pixel with a specific color to the background
 *
 * @param unsigned int color: The color in 16-bit RGB 565
 * @param unsigned int x: The x coordinate to draw at
 * @param unsigned int y: the y coordinate to draw at
 */
void drawPixel(unsigned int color, unsigned int x, unsigned int y)
{
	drawLine(x, y, x, y, color);
	//Draw pixel hal function is not working
	//Video_drawPixel(Video, color, x, y);
}

/*
 * Draws a line with a specific color to the background
 *
 * @param int x0: starting x coordinate (0..320)
 * @param int y0: starting y coordinate (0..240)
 * @param int x1: ending x coordinate (0..320)
 * @param int y1: ending y coordinate (0..240)
 * @param int color: The color in 16-bit RGB
 */
void drawLine(int x0, int y0, int x1, int y1, int color)
{
	Video_drawLine(Video, x0, y0, x1, y1, color, BACKGROUND);
}

/*
 * Prints a string to the foreground
 * WARNING: The character buffer is separate from the video buffer.
 * 			Meaning, clearing and switching the video buffer will not affect the character buffer.
 *
 * @param const char *ptr: The string to print
 * @param unsigned int x: The x coordinate to print at
 * @param unsigned int y: the y coordinate to print at
 *
 */
void printString(const char *ptr, unsigned int x,unsigned int y)
{
	Char_printString(Character, ptr, x, y);
}

/*
 * Called at the end of the loop, after all the drawing to the background has finished.
 * Swaps buffer from background to foreground, and clears background in preparation for further drawing.
 */

void display()
{
	Video_swapBuffers(Video);
	while(Video_bufferIsSwapping(Video));
	Video_clearScreen(Video, BACKGROUND);
}

void drawSprite(RectSprite* rectSprite, int x0, int y0)
{
	int i, j;
	fillMatrix(rectSprite->matrix);
	printf("size: %i, %i", rectSprite->baseSprite.height, rectSprite->baseSprite.width);
	for(j = 0; j < rectSprite->baseSprite.height; j++)
	{
		for (i = 0; i < rectSprite->baseSprite.width; i++)
		{
			printf("%04x\n", rectSprite->matrix[i][j]);
			drawPixel(rectSprite->matrix[i][j], x0+i, y0+j);
		}
	}

	display();
}

// a temp function to fill in a matrix for testing purposes
int** fillMatrix(int** matrix)
{
	int i, j;

	for(i=0; i<MAX_HEIGHT; i++)
	{
		for(j=0; j < MAX_WIDTH; j++)
		{
			if(j > 30 && j < 60)
				matrix[i][j] = 0xFFFF;
			else
				matrix[i][j] = 0xF800;
		}
	}

	return matrix;
}

//TO DO: BACKGROUND BUFFER AND BUFFER SWITCHING
