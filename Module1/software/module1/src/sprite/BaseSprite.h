/*
 * BaseSprite.h
 *
 *  Created on: 2013-10-05
 *  Author: Trevor Siemens
 *
 *  A base class for all sprites.
 *  This should be treated as an pure virtual class
 */

#ifndef BASESPRITE_H_
#define BASESPRITE_H_

#define RECTSPRITE_CLASS_TYPE 1
#define IMGSPRITE_CLASS_TYPE 2

typedef struct BaseSprite
{
	// The type of the sprite. This is set by the subclass.
	int classType;

	// The position of the sprite
	float xPos;
	float yPos;
	// The size of the sprite
	int width;
	int height;

	/**
	 * A pure virtual function
	 * This can be called from a 'subclass' like the the following example with RectSprite:
	 *
	 * RectSprite* rect = RectSprite_init(RectSprite_alloc());
	 * rect->baseSprite.draw((BaseSprite*)rect);
	 */
	void (*draw)(struct BaseSprite* /*, VideoBuffer* */);
} BaseSprite;

BaseSprite* BaseSprite_alloc();

BaseSprite* BaseSprite_init(BaseSprite* this);

void BaseSprite_setSize(BaseSprite* this, int width, int height);

void BaseSprite_setPosition(BaseSprite* this, float xPos, float yPos);

#endif /* BASESPRITE_H_ */
