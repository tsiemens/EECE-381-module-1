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
#include "audio/AudioHandler.h"
#include "video/VideoHandler.h"
#include "util/ArrayPtr.h"
#include "sprite/ImgSprite.h"
#include "sprite/SpriteParser.h"
#include "game/GameStateMachine.h"

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

	// SD Card init
	sdcard_init();

	// Video and Character handler init
	VideoHandlerInit();
	// Audio handler init
	AudioHandlerInit();

	// ImgSprite stuff

	GameStateMachine* stateMachine = GameStateMachine_init(GameStateMachine_alloc(), PS2Keyboard_alloc_init());

	char debugFreqStr[10];

	// MAIN PROGRAM LOOP
	while (hasQuit == 0) {
		Timer_start(loopTimer);

		AudioHandler_play();

		// Debug lights (they increment each frame)
		ledVals++;
		IOWR_8DIRECT(LEDS_BASE, 0, ledVals);

		AudioHandler_play();

		// TODO insert game logic here
		GameStateMachine_performFrameLogic(stateMachine);

		AudioHandler_play();

		// Sleep if finished logic within frame interval
		while (Timer_isDone(loopTimer) == 0) {
			AudioHandler_play();
		}

		// Swap buffers and clear background buffer
		display();

		AudioHandler_play();

		if (IORD_8DIRECT(SWITCHES_BASE, 0)&1 == 1) {
			sprintf(debugFreqStr, "FPS:%2.1f", 1000/Timer_timeElapsed(loopTimer));
			printString(debugFreqStr, 70, 0);
		}
		else {
			printString("         ", 70, 0);
		}

		AudioHandler_play();

	}

	return 0;
}
