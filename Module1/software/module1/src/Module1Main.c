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
	VideoDemo();

	// MAIN PROGRAM LOOP
	while(hasQuit == 0) {
		Timer_start(loopTimer);

		// Debug lights (they increment each frame)
		ledVals++;
		IOWR_8DIRECT(LEDS_BASE, 0, ledVals);

		// TODO insert game logic here

		// Testing Video Functionality
		VideoBufferSwapDemo(ledVals);

		// Sleep if finished logic within frame interval
		while (Timer_isDone(loopTimer) == 0) {}

		// TODO Swap buffers

	}

	return 0;
}
