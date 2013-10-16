/*
 * GameStateMachine.c
 *
 *  Created on: 2013-10-08
 *      Author: Jill
 */

#include "GameStateMachine.h"
#include "MenuDefinitions.h"
#include "../io/PS2Keyboard.h"
#include "../video/VideoHandler.h"
#include "../sprite/ImgSprite.h"
#include "../sprite/RectSprite.h"
#include "../sprite/AlphaSprite.h"
#include "../sprite/SpriteParser.h"
#include "../sprite/SpriteFactory.h"
#include "../audio/AudioHandler.h"

#include <stdlib.h>
#include <stdio.h>

#define LASER_DURATION 200

void GameStateMachine_ProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent);
void GameStateMachine_StartProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent);
void GameStateMachine_PlayingProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent);
void GameStateMachine_PausedProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent);
void GameStateMachine_MainMenuProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent);
void GameStateMachine_InstructionsProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent);
void GameStateMachine_GameOverProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent);
void GameStateMachine_StartPerformLogic(GameStateMachine* this);
void GameStateMachine_PlayingPerformLogic(GameStateMachine* this);
void GameStateMachine_PausedPerformLogic(GameStateMachine* this);
void GameStateMachine_MainMenuPerformLogic(GameStateMachine* this);
void GameStateMachine_InstructionsPerformLogic(GameStateMachine* this);
void GameStateMachine_GameOverPerformLogic(GameStateMachine* this);

GameStateMachine* GameStateMachine_alloc()
{
	GameStateMachine *this = (GameStateMachine *)malloc(sizeof(GameStateMachine));
	return this;
}

// Constructor for GameStateMachine
GameStateMachine* GameStateMachine_init(GameStateMachine* this, PS2Keyboard* keyboard)
{
	this->state = MAIN_MENU;
	this->keyboard = keyboard;

	this->menuSprites = SpriteArrayList_init(SpriteArrayList_alloc(), 5);
	this->gameSprites = SpriteArrayList_init(SpriteArrayList_alloc(), 2);
	this->instructionSprites = SpriteArrayList_init(SpriteArrayList_alloc(), 9);

	this->frameTimer = Timer_init(Timer_alloc(), 0);
	this->lastFrameDuration = 0;

	ImgSprite* playerSprite = SpriteFactory_generatePlayerSprite();
	SpriteArrayList_insert(this->gameSprites, (BaseSprite*)playerSprite, 0);

	instructionsInit(this);
	menuInit(this);

	return this;
}

void instructionsInit(GameStateMachine* this)
{
	ImgSprite* menu = ImgSprite_init(ImgSprite_alloc());
	SpriteParser_parse("title", menu);
	BaseSprite_setPosition((BaseSprite*)menu, 90, -80);

	AlphaSprite* instructionsTitleAlpha = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)instructionsTitleAlpha, INSTRUCTIONITEM_TITLE_XPOS, INSTRUCTIONITEM_TITLE_YPOS);
	instructionsTitleAlpha->setString(instructionsTitleAlpha, "INSTRUCTIONS");

	AlphaSprite* instructionsAlpha = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)instructionsAlpha, INSTRUCTIONITEM_TEXT_XPOS, INSTRUCTIONITEM_TITLE_YPOS+CHAR_TO_PIXEL_HEIGHT);
	instructionsAlpha->setString(instructionsAlpha, INSTRUCTIONITEM_P1_L1);

	AlphaSprite* instructionsAlpha2 = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)instructionsAlpha2, INSTRUCTIONITEM_TEXT_XPOS, INSTRUCTIONITEM_TITLE_YPOS+CHAR_TO_PIXEL_HEIGHT*2);
	instructionsAlpha2->setString(instructionsAlpha2, INSTRUCTIONITEM_P1_L2);

	AlphaSprite* instructionsAlpha3 = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)instructionsAlpha3, INSTRUCTIONITEM_TEXT_XPOS, INSTRUCTIONITEM_TITLE_YPOS+CHAR_TO_PIXEL_HEIGHT*3);
	instructionsAlpha3->setString(instructionsAlpha3, INSTRUCTIONITEM_P1_L3);

	AlphaSprite* instructionsAlpha4 = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)instructionsAlpha4, INSTRUCTIONITEM_TEXT_XPOS, INSTRUCTIONITEM_TITLE_YPOS+CHAR_TO_PIXEL_HEIGHT*4);
	instructionsAlpha4->setString(instructionsAlpha4, INSTRUCTIONITEM_P1_L4);

	AlphaSprite* instructionsAlpha5 = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)instructionsAlpha5, INSTRUCTIONITEM_TEXT_XPOS, INSTRUCTIONITEM_TITLE_YPOS+CHAR_TO_PIXEL_HEIGHT*5);
	instructionsAlpha5->setString(instructionsAlpha5, INSTRUCTIONITEM_P1_L5);

	AlphaSprite* instructionsAlphaRight = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)instructionsAlphaRight, INSTRUCTIONITEM_TITLE_XPOS+5, INSTRUCTIONITEM_TITLE_YPOS+CHAR_TO_PIXEL_HEIGHT*6);
	instructionsAlphaRight->setString(instructionsAlphaRight, ">");

	AlphaSprite* instructionsBackToMenu= AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)instructionsBackToMenu, INSTRUCTIONITEM_ESC_XPOS, MENUITEM_CONTINUE_YPOS+CHAR_TO_PIXEL_HEIGHT*2);
	instructionsAlpha5->setString(instructionsBackToMenu, "Press ESC to return to menu");


	SpriteArrayList_insert(this->instructionSprites, (BaseSprite*)menu, 0);
	SpriteArrayList_insert(this->instructionSprites, (BaseSprite*)instructionsTitleAlpha, 1);
	SpriteArrayList_insert(this->instructionSprites, (BaseSprite*)instructionsAlpha, 2);
	SpriteArrayList_insert(this->instructionSprites, (BaseSprite*)instructionsAlpha2, 3);
	SpriteArrayList_insert(this->instructionSprites, (BaseSprite*)instructionsAlpha3, 4);
	SpriteArrayList_insert(this->instructionSprites, (BaseSprite*)instructionsAlpha4, 5);
	SpriteArrayList_insert(this->instructionSprites, (BaseSprite*)instructionsAlpha5, 6);
	SpriteArrayList_insert(this->instructionSprites, (BaseSprite*)instructionsAlphaRight, 7);
	SpriteArrayList_insert(this->instructionSprites, (BaseSprite*)instructionsBackToMenu, 8);
}

void menuInit(GameStateMachine* this)
{
	RectSprite* menuOuterFrame = RectSprite_init(RectSprite_alloc());
	BaseSprite_setSize((BaseSprite*)menuOuterFrame, MENUFRAME_WIDTH, MENUFRAME_HEIGHT);
	BaseSprite_setPosition((BaseSprite*)menuOuterFrame, MENUFRAME_XPOS, MENUFRAME_YPOS);
	menuOuterFrame->colour = MENUFRAME_COLOR;

	AlphaSprite* menuStartAlpha = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)menuStartAlpha, MENUITEM_START_XPOS, MENUITEM_START_YPOS);
	menuStartAlpha->setString(menuStartAlpha, "Start");

	AlphaSprite* menuContinueAlpha = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)menuContinueAlpha, MENUITEM_START_XPOS, MENUITEM_CONTINUE_YPOS);
	menuContinueAlpha->setString(menuContinueAlpha, "Instructions");

	RectSprite* menuSelectorFrame = RectSprite_init(RectSprite_alloc());
	BaseSprite_setSize((BaseSprite*)menuSelectorFrame, MENU_SELECTOR_WIDTH, MENU_SELECTOR_HEIGHT);
	BaseSprite_setPosition((BaseSprite*)menuSelectorFrame, MENU_SELECTOR_XPOS, MENU_SELECTOR_CONTINUE_YPOS);
	menuSelectorFrame->colour = MENU_SELECTOR_COLOR;

	ImgSprite* menu = ImgSprite_init(ImgSprite_alloc());
	SpriteParser_parse("title", menu);
	BaseSprite_setPosition((BaseSprite*)menu, 90, 0);

	//menuSprites array to be made dynamic (if possible)
	SpriteArrayList_insert(this->menuSprites, (BaseSprite*)menuOuterFrame, 0);
	SpriteArrayList_insert(this->menuSprites, (BaseSprite*)menuStartAlpha, 1);
	SpriteArrayList_insert(this->menuSprites, (BaseSprite*)menuContinueAlpha, 2);
	SpriteArrayList_insert(this->menuSprites, (BaseSprite*)menuSelectorFrame, 3);
	SpriteArrayList_insert(this->menuSprites, (BaseSprite*)menu, 4);

}

void GameStateMachine_performFrameLogic(GameStateMachine* this)
{
	this->lastFrameDuration = Timer_timeElapsed(this->frameTimer);
	Timer_start(this->frameTimer);

	alt_u8 readKey;
	int keyStatus = PS2Keyboard_readKey(this->keyboard, &readKey);

	while(keyStatus >= 0)
	{
		GameStateMachine_ProcessKey(this, (char)readKey, keyStatus);
		keyStatus = PS2Keyboard_readKey(this->keyboard, &readKey);
	}
	GameStateMachine_PerformLogic(this);

	if(this->state == PLAYING)
	{
		VideoHandler_drawSprites(this->gameSprites);
	}
	else if (this->state == PAUSED)
	{
		// draw game sprites, then draw menu sprite
	}
	else if (this->state == INSTRUCTIONS)
	{
		VideoHandler_drawSprites(this->instructionSprites);
	}
	else // MENU
	{
		VideoHandler_drawSprites(this->menuSprites);
	}
}

void GameStateMachine_ProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent)
{
	switch( this->state )
	{
	    case START:
	        GameStateMachine_StartProcessKey(this, key, isUpEvent);
	    case MAIN_MENU:
	    	GameStateMachine_MainMenuProcessKey(this, key, isUpEvent);
	    case INSTRUCTIONS:
	    	GameStateMachine_InstructionsProcessKey(this, key, isUpEvent);
	    case PLAYING:
	    	GameStateMachine_PlayingProcessKey(this, key, isUpEvent);
	    case PAUSED:
	    	GameStateMachine_PausedProcessKey(this, key, isUpEvent);
	    case GAME_OVER:
	    	GameStateMachine_GameOverProcessKey(this, key, isUpEvent);

	}
}

void GameStateMachine_PerformLogic(GameStateMachine* this)
{
	switch( this->state )
	{
	    case START:
	        GameStateMachine_StartPerformLogic(this);
	    case MAIN_MENU:
	    	GameStateMachine_MainMenuPerformLogic(this);
	    case INSTRUCTIONS:
	    	GameStateMachine_InstructionsPerformLogic(this);
	    case PLAYING:
	    	GameStateMachine_PlayingPerformLogic(this);
	    case PAUSED:
	    	GameStateMachine_PausedPerformLogic(this);
	    case GAME_OVER:
	    	GameStateMachine_GameOverPerformLogic(this);

	}
}

void GameStateMachine_StartProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent)
{

}

void GameStateMachine_PlayingProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent)
{
	if(key == KEY_LEFT || key == KEY_RIGHT)
	{
		BaseSprite* playerSprite = SpriteArrayList_getWithId(this->gameSprites, PLAYER_SPRITE_ID);
		if (playerSprite != NULL)
		{
			if(isUpEvent == 0)
			{
				if(key == KEY_LEFT) {
					playerSprite->xVel += -PLAYER_SPEED;
				}
				else {
					playerSprite->xVel += PLAYER_SPEED;
				}
			}
			else if (isUpEvent == 1)
			{
				if(key == KEY_LEFT) {
					playerSprite->xVel += PLAYER_SPEED;
				}
				else {
					playerSprite->xVel += -PLAYER_SPEED;
				}
			}
		}
	}
	else if(key == '1' || key == '2' || key == '3' || key == '4')
	{
		RectSprite* laserSprite = (RectSprite*)SpriteArrayList_getWithId(this->gameSprites, PLAYER_LASER_SPRITE_ID);
		if (laserSprite == NULL && isUpEvent == 0 /* && check if key enabled*/)
		{
			BaseSprite* player = SpriteArrayList_getWithId(this->gameSprites, PLAYER_SPRITE_ID);
			int laserColour = 0;
			if (key == '1')
				laserColour = 0x07FF; // Add
			else if (key == '2')
				laserColour = 0xF800; // Subtract
			else if (key == '3')
				laserColour = 0x07E0; // Mult
			else if (key == '4')
				laserColour = 0xF81F; // Div

			// set audio to play shooting effects
			AudioHandler_playShoot();

			laserSprite = SpriteFactory_generateLaserSprite(player, laserColour);
			laserSprite->baseSprite.animTimer = Timer_init(Timer_alloc(), LASER_DURATION);
			Timer_start(laserSprite->baseSprite.animTimer);
			SpriteArrayList_insert(this->gameSprites, (BaseSprite*)laserSprite, this->gameSprites->last + 1);
		}
	}
	else if(key == KEY_ESC && isUpEvent == 0) {
			this->state = MAIN_MENU;
	}
}

void GameStateMachine_PausedProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent)
{

}

void GameStateMachine_MainMenuProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent)
{
	static bool startSelected;

	BaseSprite* selSprite = SpriteArrayList_getAt(this->menuSprites, 3);
	if(isUpEvent == 0)
	{
		if(key == KEY_DOWN) {
			selSprite->yPos = MENU_SELECTOR_CONTINUE_YPOS;
			startSelected = false;
		}
		else if(key == KEY_UP) {
			selSprite->yPos = MENU_SELECTOR_NEWGAME_YPOS;
			startSelected = true;
		}
		else if(key == '\n' && startSelected == true) {
			//clearChar() to be replaced by clearing individual strings
			clearChar();
			this->state = PLAYING;
		}
		else if(key == '\n' && startSelected == false) {
			clearChar();
			this->state = INSTRUCTIONS;
		}
	}
}

GameStateMachine_InstructionsProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent)
{
	static bool onSecondPage;
	if(isUpEvent == 0)
	{
		if (key == KEY_ESC)
		{
			clearChar();
			this->state = MAIN_MENU;
		}
		else if(key == KEY_RIGHT && onSecondPage == false)
		{
			onSecondPage = true;
			BaseSprite* line1 = SpriteArrayList_getAt(this->instructionSprites, 2);
			((AlphaSprite*)line1)->setString(((AlphaSprite*)line1), "Aim your spaceship under the falling burgers and shoot");
			BaseSprite* line2 = SpriteArrayList_getAt(this->instructionSprites, 3);
			((AlphaSprite*)line2)->setString(((AlphaSprite*)line2), "them down with an OPERATION LASER. You can see what");
			BaseSprite* line3 = SpriteArrayList_getAt(this->instructionSprites, 4);
			((AlphaSprite*)line3)->setString(((AlphaSprite*)line3), "key uses which operation at the top of the screen when");
			BaseSprite* line4 = SpriteArrayList_getAt(this->instructionSprites, 5);
			((AlphaSprite*)line4)->setString(((AlphaSprite*)line4), "playing. The current and maximum amounts of burgers is");
			BaseSprite* line5 = SpriteArrayList_getAt(this->instructionSprites, 6);
			((AlphaSprite*)line5)->setString(((AlphaSprite*)line5), "also shown at the top. Good luck!");
			BaseSprite* left = SpriteArrayList_getAt(this->instructionSprites, 7);
			((AlphaSprite*)left)->setString(((AlphaSprite*)left), "<");
			clearChar();
		}
		else if(key == KEY_LEFT && onSecondPage == true)
		{
			onSecondPage = false;
			BaseSprite* line1 = SpriteArrayList_getAt(this->instructionSprites, 2);
			((AlphaSprite*)line1)->setString(((AlphaSprite*)line1), INSTRUCTIONITEM_P1_L1);
			BaseSprite* line2 = SpriteArrayList_getAt(this->instructionSprites, 3);
			((AlphaSprite*)line2)->setString(((AlphaSprite*)line2), INSTRUCTIONITEM_P1_L2);
			BaseSprite* line3 = SpriteArrayList_getAt(this->instructionSprites, 4);
			((AlphaSprite*)line3)->setString(((AlphaSprite*)line3), INSTRUCTIONITEM_P1_L3);
			BaseSprite* line4 = SpriteArrayList_getAt(this->instructionSprites, 5);
			((AlphaSprite*)line4)->setString(((AlphaSprite*)line4), INSTRUCTIONITEM_P1_L4);
			BaseSprite* line5 = SpriteArrayList_getAt(this->instructionSprites, 6);
			((AlphaSprite*)line5)->setString(((AlphaSprite*)line5), INSTRUCTIONITEM_P1_L5);
			BaseSprite* left = SpriteArrayList_getAt(this->instructionSprites, 7);
			((AlphaSprite*)left)->setString(((AlphaSprite*)left), ">");
			clearChar();
		}
	}
}

void GameStateMachine_GameOverProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent)
{

}

/******** PERFORMING LOGIC **********/
void GameStateMachine_StartPerformLogic(GameStateMachine* this)
{

}

GameStateMachine_InstructionsPerformLogic(GameStateMachine* this)
{

}

void GameStateMachine_PlayingPerformLogic(GameStateMachine* this)
{
	// Updating player position (finalization)
	BaseSprite* playerSprite = SpriteArrayList_getWithId(this->gameSprites, PLAYER_SPRITE_ID);
	if (playerSprite != NULL){
		if (playerSprite->xVel > PLAYER_SPEED)
			playerSprite->xVel = PLAYER_SPEED;
		else if (playerSprite->xVel < -PLAYER_SPEED)
			playerSprite->xVel = -PLAYER_SPEED;
		BaseSprite_updatePos(playerSprite, this->lastFrameDuration);
		if ( (playerSprite->xPos + playerSprite->width) >= 320)
			playerSprite->xPos = 319 - playerSprite->width;
		else if ( playerSprite->xPos < 0 )
			playerSprite->xPos = 0;
	}
	BaseSprite* laserSprite = SpriteArrayList_getWithId(this->gameSprites, PLAYER_LASER_SPRITE_ID);
	if (laserSprite != NULL)
	{
		if (Timer_isDone(laserSprite->animTimer) == 1){
			SpriteArrayList_removeObject(this->gameSprites, laserSprite);
			RectSprite_free((RectSprite*)laserSprite);
		} else {
			laserSprite->xPos = playerSprite->xPos + (playerSprite->width/2) - 1;
		}
	}
}

void GameStateMachine_PausedPerformLogic(GameStateMachine* this)
{

}

void GameStateMachine_MainMenuPerformLogic(GameStateMachine* this)
{

}

void GameStateMachine_GameOverPerformLogic(GameStateMachine* this)
{

}
