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

#include <stdlib.h>
#include <stdio.h>

void GameStateMachine_ProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent);
void GameStateMachine_StartProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent);
void GameStateMachine_PlayingProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent);
void GameStateMachine_PausedProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent);
void GameStateMachine_MainMenuProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent);
void GameStateMachine_GameOverProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent);
void GameStateMachine_StartPerformLogic(GameStateMachine* this);
void GameStateMachine_PlayingPerformLogic(GameStateMachine* this);
void GameStateMachine_PausedPerformLogic(GameStateMachine* this);
void GameStateMachine_MainMenuPerformLogic(GameStateMachine* this);
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
	this->frameTimer = Timer_init(Timer_alloc(), 0);
	this->lastFrameDuration = 0;

	//TODO: un-ghetto this
	ImgSprite* img = ImgSprite_init(ImgSprite_alloc());
	SpriteParser_parse("play", img);
	((BaseSprite*)img)->spriteId = PLAYER_SPRITE_ID;
	BaseSprite_setPosition((BaseSprite*)img, 150, 200);
	RectSprite* rect = RectSprite_init(RectSprite_alloc());
	rect->colour = 0xFFFF;
	BaseSprite_setSize((BaseSprite*)rect, 20, 10);
	BaseSprite_setPosition((BaseSprite*)rect, 150, 200);

	this->menuSprites = SpriteArrayList_init(SpriteArrayList_alloc(), 4);

	this->gameSprites = SpriteArrayList_init(SpriteArrayList_alloc(), 1);
	SpriteArrayList_insert(this->gameSprites, (BaseSprite*)img, 0);

	menuInit(this);

	return this;
}

void menuInit(GameStateMachine* this)
{
	RectSprite* menuOuterFrame = RectSprite_init(RectSprite_alloc());
	menuOuterFrame->baseSprite.xPos = MENUFRAME_XPOS;
	menuOuterFrame->baseSprite.yPos = MENUFRAME_YPOS;
	menuOuterFrame->baseSprite.width = MENUFRAME_WIDTH;
	menuOuterFrame->baseSprite.height = MENUFRAME_HEIGHT;
	menuOuterFrame->colour = MENUFRAME_COLOR;

	AlphaSprite* menuStartAlpha = AlphaSprite_init(AlphaSprite_alloc());
	menuStartAlpha->baseSprite.xPos = MENUITEM_START_XPOS;
	menuStartAlpha->baseSprite.yPos = MENUITEM_START_YPOS;
	menuStartAlpha->setString(menuStartAlpha, "New Game");

	AlphaSprite* menuContinueAlpha = AlphaSprite_init(AlphaSprite_alloc());
	menuContinueAlpha->baseSprite.xPos = MENUITEM_START_XPOS;
	menuContinueAlpha->baseSprite.yPos = MENUITEM_CONTINUE_YPOS;
	menuContinueAlpha->setString(menuContinueAlpha, "Continue");

	RectSprite* menuSelectorFrame = RectSprite_init(RectSprite_alloc());
	menuSelectorFrame->baseSprite.xPos = MENU_SELECTOR_XPOS;
	menuSelectorFrame->baseSprite.yPos = MENU_SELECTOR_CONTINUE_YPOS;
	menuSelectorFrame->baseSprite.width = MENU_SELECTOR_WIDTH;
	menuSelectorFrame->baseSprite.height = MENU_SELECTOR_HEIGHT;
	menuSelectorFrame->colour = MENU_SELECTOR_COLOR;


	//menuSprites array to be made dynamic (if possible)
	SpriteArrayList_insert(this->menuSprites, (BaseSprite*)menuOuterFrame, 0);
	SpriteArrayList_insert(this->menuSprites, (BaseSprite*)menuStartAlpha, 1);
	SpriteArrayList_insert(this->menuSprites, (BaseSprite*)menuContinueAlpha, 2);
	SpriteArrayList_insert(this->menuSprites, (BaseSprite*)menuSelectorFrame, 3);
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
	BaseSprite* playerSprite = SpriteArrayList_getWithId(this->gameSprites, PLAYER_SPRITE_ID);
	if(isUpEvent == 0)
	{
		if (playerSprite != NULL && (key == KEY_LEFT || key == KEY_RIGHT)){
			if(key == KEY_LEFT) {
				playerSprite->xVel += -PLAYER_SPEED;
			}
			else {
				playerSprite->xVel += PLAYER_SPEED;
			}
		}
		else if(key == KEY_ESC) {
			this->state = MAIN_MENU;
		}
	}
	else if (isUpEvent == 1)
	{
		if (playerSprite != NULL && (key == KEY_LEFT || key == KEY_RIGHT)){
			if(key == KEY_LEFT) {
				playerSprite->xVel += PLAYER_SPEED;
			}
			else {
				playerSprite->xVel += -PLAYER_SPEED;
			}
		}
	}
}

void GameStateMachine_PausedProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent)
{

}

void GameStateMachine_MainMenuProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent)
{
	static bool newGameSelected;

	BaseSprite* selSprite = SpriteArrayList_getAt(this->menuSprites, 3);
	if(isUpEvent == 0)
	{
		if(key == KEY_DOWN) {
			selSprite->yPos = MENU_SELECTOR_CONTINUE_YPOS;
			newGameSelected = false;
		}
		else if(key == KEY_UP) {
			selSprite->yPos = MENU_SELECTOR_NEWGAME_YPOS;
			newGameSelected = true;
		}
		else if(key == '\n' && newGameSelected == true) {
			//clearChar() to be replaced by clearing individual strings
			clearChar();
			this->state = PLAYING;
		}
	}
}

void GameStateMachine_GameOverProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent)
{

}

void GameStateMachine_StartPerformLogic(GameStateMachine* this)
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
