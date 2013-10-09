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
#include "io/SDCard.h"
#include "io/Audio.h"
#include "video/VideoHandler.h"
#include "util/ArrayPtr.h"
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

	// Video and Character handler init
	VideoHandlerInit();
	ImgSprite* img = ImgSprite_init(ImgSprite_alloc());
	SpriteParser_parse("pink", img);
	BaseSprite_setPosition((BaseSprite*)img, 50, 50);

	BaseSprite* sprites[1] = { (BaseSprite*)img };

	//

	// VIDEO DEMO. TO BE REMOVED.
//	printString("EECE 381. Shooting Game Project", 2,2);
	char debugFreqStr[10];

	// MAIN PROGRAM LOOP
	while (hasQuit == 0) {
		Timer_start(loopTimer);

		// Debug lights (they increment each frame)
		ledVals++;
		IOWR_8DIRECT(LEDS_BASE, 0, ledVals);

		// TODO insert game logic here

		// VIDEO DEMO. TO BE REMOVED.
		// Creates animation by having varying values every loop
//		drawLine(319, 120, (ledVals*2)%319, (ledVals*2)%239, 0xA22F+ledVals*50);
//		drawPixel(0xFFFF, 160, (ledVals*2)%240);

		// Sleep if finished logic within frame interval
		while (Timer_isDone(loopTimer) == 0) {
		}

		// Swap buffers and clear background buffer
		drawSprites(sprites, 1);
		display();

		sprintf(debugFreqStr, "FPS:%2.1f", 1000/Timer_timeElapsed(loopTimer));
		printString(debugFreqStr, 70, 0);
	}

	return 0;
}
