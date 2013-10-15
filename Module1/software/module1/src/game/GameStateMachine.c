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
	//TODO: un-ghetto this
	this->state = MAIN_MENU;
	this->keyboard = keyboard;
	ImgSprite* img = ImgSprite_init(ImgSprite_alloc());
	SpriteParser_parse("play", img);
	BaseSprite_setPosition((BaseSprite*)img, 150, 200);
	this->menuSprites = SpriteArrayList_init(SpriteArrayList_alloc(), 4);
	this->gameSprites = SpriteArrayList_init(SpriteArrayList_alloc(), 1);
	SpriteArrayList_insert(this->gameSprites, (BaseSprite*)img, 0);
	printf("returning\n");

	menuInit(this);

	return this;
}

void menuInit(GameStateMachine* this)
{
	RectSprite* menuOuterFrame = RectSprite_init(RectSprite_alloc());
	BaseSprite_setSize((BaseSprite*)menuOuterFrame, MENUFRAME_WIDTH, MENUFRAME_HEIGHT);
	BaseSprite_setPosition((BaseSprite*)menuOuterFrame, MENUFRAME_XPOS, MENUFRAME_YPOS);
	menuOuterFrame->colour = MENUFRAME_COLOR;

	AlphaSprite* menuStartAlpha = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)menuStartAlpha, MENUITEM_START_XPOS, MENUITEM_START_YPOS);
	menuStartAlpha->setString(menuStartAlpha, "New Game");

	AlphaSprite* menuContinueAlpha = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)menuContinueAlpha, MENUITEM_START_XPOS, MENUITEM_CONTINUE_YPOS);
	menuContinueAlpha->setString(menuContinueAlpha, "Continue");

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

void GameStateMachine_performFrameLogic(GameStateMachine* this){

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
	BaseSprite* sprite = SpriteArrayList_getAt(this->gameSprites, 0);
	if(isUpEvent == 0)
	{
		if(key == KEY_LEFT) {
			sprite->xPos -= 10;
			if ( sprite->xPos < 0)
				sprite->xPos = 0;
		}
		else if(key == KEY_RIGHT) {
			sprite->xPos += 10;
			if ( (sprite->xPos + sprite->width) >= 320)
				sprite->xPos = 319 - sprite->width;
		}
		else if(key == KEY_ESC) {
			this->state = MAIN_MENU;
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
