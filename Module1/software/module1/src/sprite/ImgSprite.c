/*
 * ImgSprite.h
 *
 *  Created on: 2013-10-05
 *  Author: Pui Yan (Denise) Kwok
 */

#include "ImgSprite.h"
#include <stdlib.h>


void ImgSprite_draw(BaseSprite* super/*, VideoBuffer* */);

ImgSprite* ImgSprite_alloc()
{
	ImgSprite *this = (ImgSprite *)malloc(sizeof(ImgSprite));
	return this;
}

ImgSprite* ImgSprite_init(ImgSprite* this)
{
	BaseSprite_init(&this->baseSprite);
	// Set this instance of ImgSprite
	this->baseSprite.classType = IMGSPRITE_CLASS_TYPE;
	this->baseSprite.draw = &ImgSprite_draw;
	this->matrix = malloc(sizeof(int)*MAX_HEIGHT*MAX_WIDTH);
	return this;
}

// TODO: Remove this function, as it is just a temporary function to fill in a matrix for testing purposes
int** fillMatrix(int** matrix, int rows, int cols)
{
	int i, j;

	for(i=0; i < rows; i++)
	{
		for(j=0; j < cols; j++)
		{
			if(j >= (cols/4) && j <= (3*cols/4))
				matrix[i][j] = 0xFFFF;
			else
				matrix[i][j] = 0xF800;
		}
	}

	return matrix;
}

/**
 * Draws the rectangle sprite on the video buffer
 */
void ImgSprite_draw(BaseSprite* super/*, VideoBuffer* */)
{
	ImgSprite* this = (ImgSprite* )super;

	int i, j;
	fillMatrix(this->matrix, this->baseSprite.height, this->baseSprite.width);

	for(i = 0; i < this->baseSprite.height; i++)
	{
		for (j = 0; j < this->baseSprite.width; j++)
		{
			drawPixel(this->matrix[i][j], (int)this->baseSprite.xPos + j, (int)this->baseSprite.yPos + i);
		}
	}
}

