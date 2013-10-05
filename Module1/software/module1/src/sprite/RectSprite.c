/*
 * RectSprite.c
 *
 *  Created on: 2013-10-05
 *  Author: Trevor Siemens
 */

#include "RectSprite.h"
#include <stdlib.h>

void RectSprite_draw(BaseSprite* super/*, VideoBuffer* */);

RectSprite* RectSprite_new()
{
	RectSprite *this = (RectSprite *)malloc(sizeof(RectSprite));
	return this;
}

RectSprite* RectSprite_constructor(RectSprite* this)
{
	BaseSprite_constructor(&this->baseSprite);
	// Set this instance of RectSprite
	this->baseSprite.classType = RECTSPRITE_CLASS_TYPE;
	this->baseSprite.draw = &RectSprite_draw;

	this->colour = 0;

	return this;
}

/**
 * Draws the rectangle sprite on the video buffer
 */
void RectSprite_draw(BaseSprite* super/*, VideoBuffer* */)
{
	RectSprite* this = (RectSprite* )super;
}
