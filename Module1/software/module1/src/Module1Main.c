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
#include "sprite/RectSprite.h"
#include "sprite/AlphaSprite.h"

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

	// TO BE MOVED TO GAMESTATEMACHINE. TESTING MENU INITIALIZATION
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define MENUFRAME_WIDTH SCREEN_WIDTH/3
#define MENUFRAME_HEIGHT SCREEN_HEIGHT/4
#define MENUFRAME_BOT_PADDING 20
#define MENUFRAME_YPOS (SCREEN_HEIGHT - MENUFRAME_HEIGHT - MENUFRAME_BOT_PADDING)
#define MENUFRAME_XPOS (SCREEN_WIDTH/2 - MENUFRAME_WIDTH/2)
#define MENUFRAME_COLOR 0x4A49

	RectSprite* menuOuterFrame = RectSprite_init(RectSprite_alloc());
	menuOuterFrame->baseSprite.xPos = MENUFRAME_XPOS;
	menuOuterFrame->baseSprite.yPos = MENUFRAME_YPOS;
	menuOuterFrame->baseSprite.width = MENUFRAME_WIDTH;
	menuOuterFrame->baseSprite.height = MENUFRAME_HEIGHT;
	menuOuterFrame->colour = MENUFRAME_COLOR;

#define CHARBUFFER_WIDTH 80
#define CHARBUFFER_HEIGHT 60
#define CHAR_TO_PIXEL ((float)SCREEN_HEIGHT/CHARBUFFER_HEIGHT)
#define PIXEL_TO_CHAR ((float)CHARBUFFER_HEIGHT/SCREEN_HEIGHT)

#define CHAR_PADDING_Y 6
#define CHAR_PADDING_X 4
#define CHAR_NEWLINE 3
#define MENUITEM_START_XPOS (CHARBUFFER_WIDTH/2 - CHAR_PADDING_X)
#define MENUITEM_START_YPOS (PIXEL_TO_CHAR*MENUFRAME_YPOS + CHAR_PADDING_Y)

	AlphaSprite* menuStartAlpha = AlphaSprite_init(AlphaSprite_alloc());
	menuStartAlpha->baseSprite.xPos = MENUITEM_START_XPOS;
	menuStartAlpha->baseSprite.yPos = MENUITEM_START_YPOS;
	menuStartAlpha->setString(menuStartAlpha, "New Game");

#define MENUITEM_CONTINUE_YPOS (MENUITEM_START_YPOS + CHAR_NEWLINE)

	AlphaSprite* menuContinueAlpha = AlphaSprite_init(AlphaSprite_alloc());
	menuContinueAlpha->baseSprite.xPos = MENUITEM_START_XPOS;
	menuContinueAlpha->baseSprite.yPos = MENUITEM_CONTINUE_YPOS;
	menuContinueAlpha->setString(menuContinueAlpha, "Continue");

#define MENU_SELECTOR_PADDING 3
#define MENU_SELECTOR_XPOS MENUITEM_START_XPOS*((float)SCREEN_WIDTH/CHARBUFFER_WIDTH) - MENU_SELECTOR_PADDING
#define MENU_SELECTOR_NEWGAME_YPOS (MENUITEM_START_YPOS*((float)SCREEN_HEIGHT/CHARBUFFER_HEIGHT) - MENU_SELECTOR_PADDING)
#define	MENU_SELECTOR_CONTINUE_YPOS (MENU_SELECTOR_NEWGAME_YPOS + CHAR_TO_PIXEL*CHAR_NEWLINE)
#define MENU_SELECTOR_WIDTH 38
#define MENU_SELECTOR_HEIGHT 8
#define MENU_SELECTOR_COLOR 0xFFFF

	RectSprite* menuSelectorFrame = RectSprite_init(RectSprite_alloc());
	menuSelectorFrame->baseSprite.xPos = MENU_SELECTOR_XPOS;
	menuSelectorFrame->baseSprite.yPos = MENU_SELECTOR_CONTINUE_YPOS;
	menuSelectorFrame->baseSprite.width = MENU_SELECTOR_WIDTH;
	menuSelectorFrame->baseSprite.height = MENU_SELECTOR_HEIGHT;
	menuSelectorFrame->colour = MENU_SELECTOR_COLOR;

	// Video and Character handler init
	VideoHandlerInit();

	// MAIN PROGRAM LOOP
	while (hasQuit == 0) {
		Timer_start(loopTimer);

		// Debug lights (they increment each frame)
		ledVals++;
		IOWR_8DIRECT(LEDS_BASE, 0, ledVals);

		// TODO insert game logic here
		if(ledVals%2 == 0)
			menuSelectorFrame->baseSprite.yPos = MENU_SELECTOR_NEWGAME_YPOS;
		else
			menuSelectorFrame->baseSprite.yPos = MENU_SELECTOR_CONTINUE_YPOS;

		// Sleep if finished logic within frame interval
		while (Timer_isDone(loopTimer) == 0) {
		}

		// TO BE REMOVED. TESTING MENU DRAWING
		menuOuterFrame->baseSprite.draw(menuOuterFrame);
		menuSelectorFrame->baseSprite.draw(menuSelectorFrame);
		menuStartAlpha->baseSprite.draw(menuStartAlpha);
		menuContinueAlpha->baseSprite.draw(menuContinueAlpha);

		// Swap buffers and clear background buffer
		display();
	}

	return 0;
}
