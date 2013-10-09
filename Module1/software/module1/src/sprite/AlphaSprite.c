/*
 * AlphaSprite.h
 *
 *  Created on: 2013-10-08
 *  Author: Wesley Tsai
 *
 *  Reminder that the character buffer is an 80x60 Array
 */

#include "AlphaSprite.h"
#include <stdlib.h>

AlphaSprite* AlphaSprite_alloc()
{
	AlphaSprite *this = (AlphaSprite *)malloc(sizeof(AlphaSprite));
	return this;
}

AlphaSprite* AlphaSprite_init(AlphaSprite* this)
{
	BaseSprite_init(&this->baseSprite);

	char* defaultString = "Please Set A String";

	this->baseSprite.classType = ALPHASPRITE_CLASS_TYPE;
	this->baseSprite.draw = &AlphaSprite_draw;
	this->setString = &AlphaSprite_setString;
	this->string = defaultString;

	return this;
}

void AlphaSprite_setString(AlphaSprite* this, char* string)
{
	this->string = string;
}

void AlphaSprite_draw(BaseSprite* super)
{
	AlphaSprite* this = (AlphaSprite*) super;
	printString(this->string, (unsigned int)this->baseSprite.xPos, (unsigned int)this->baseSprite.yPos);
}
