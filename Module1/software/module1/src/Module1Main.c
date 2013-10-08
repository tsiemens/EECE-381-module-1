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
#include "sprite/SpriteParser.h"

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
//	DEN'S DEMO CODE
	VideoHandlerInit();
	RectSprite* rect = RectSprite_init(RectSprite_alloc());
	BaseSprite_setSize((BaseSprite*)rect, 50, 50);
	rect->colour = 0x9012;
	ImgSprite* img = ImgSprite_init(ImgSprite_alloc());
	SpriteParser_parse("test", img);
	BaseSprite_setPosition((BaseSprite*)img, 100, 0);
	BaseSprite* arr[] = { (BaseSprite*)rect, (BaseSprite*)img };
	drawSprites(arr, 2);
//	END DEN'S DEMO CODE

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
