/*
 * BaseSprite.c
 *
 *  Created on: 2013-10-05
 *  Author: Trevor Siemens
 */

#include "BaseSprite.h"
#include <stdlib.h>

BaseSprite* BaseSprite_new()
{
	BaseSprite *this = (BaseSprite *)malloc(sizeof(BaseSprite));
	return this;
}

BaseSprite* BaseSprite_constructor(BaseSprite* this)
{
	this->classType = 0;
	this->height = 0;
	this->width = 0;
	this->xPos = 0.0;
	this->yPos = 0.0;

	// draw is pure virtual in the base
	this->draw = 0;

	return this;
}
