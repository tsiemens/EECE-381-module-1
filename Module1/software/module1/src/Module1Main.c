/*
 * Module1Main.c
 *
 *  Created on: 2013-09-27
 */

#include <stdio.h>
#include "system.h"
#include "io.h"
#include "util/Timer.h"
#include "io/PS2Keyboard.h"
#include "video/VideoHandler.h"
#include "sprite/RectSprite.h"
#include "sprite/ImgSprite.h"

// Approx time per loop for 60 Hz
#define MAIN_LOOP_MIN_TIME_MS 16

int main()
{
	// INITIALISATION
	int hasQuit = 0;

	// Timestamp timer setup
	Timer* loopTimer = Timer_init(Timer_alloc(), MAIN_LOOP_MIN_TIME_MS);

	// Init LEDs
	unsigned char ledVals = 0;
	IOWR_8DIRECT(LEDS_BASE, 0, ledVals);

	// TODO other initialisation

	// Testing Video Functionality
	VideoHandlerInit();
	RectSprite* rect = RectSprite_init(RectSprite_alloc());
	rect->baseSprite.height = 10;
	rect->baseSprite.width = 40;
	rect->colour = 0xFF6F;
	ImgSprite* img = ImgSprite_init(ImgSprite_alloc());
	img->baseSprite.height = 20;
	img->baseSprite.width = 40;
	img->baseSprite.xPos = 10;
	img->baseSprite.yPos = 40;

	BaseSprite* arr[] = {&(rect->baseSprite), &(img->baseSprite)};
	drawSprites(arr, 2);

	// MAIN PROGRAM LOOP
	while(hasQuit == 0) {
		Timer_start(loopTimer);

		// Debug lights (they increment each frame)
		ledVals++;
		IOWR_8DIRECT(LEDS_BASE, 0, ledVals);

		// TODO insert game logic here

		// Testing Video Functionality
//		VideoBufferSwapDemo(ledVals);

		// Sleep if finished logic within frame interval
		while (Timer_isDone(loopTimer) == 0) {}

		// TODO Swap buffers

	}

	return 0;
}
