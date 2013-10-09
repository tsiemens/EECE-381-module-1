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
	RectSprite* rect = RectSprite_init(RectSprite_alloc());
	rect->colour = 0xFFFF;
	BaseSprite_setSize((BaseSprite*)rect, 20, 10);
	BaseSprite_setPosition((BaseSprite*)rect, 150, 200);
	this->gameSprites = malloc(sizeof(BaseSprite*));
	(*this->gameSprites)[0] = (BaseSprite*)img;
	printf("returning\n");

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
	(*this->menuSprites)[0] = (BaseSprite*)menuOuterFrame;
	(*this->menuSprites)[1] = (BaseSprite*)menuStartAlpha;
	(*this->menuSprites)[2] = (BaseSprite*)menuContinueAlpha;
	(*this->menuSprites)[3] = (BaseSprite*)menuSelectorFrame;
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
		BaseSprite* arr = *(this->gameSprites);
		drawSprites(arr, 1);
	}
	else if (this->state == PAUSED)
	{
		// draw game sprites, then draw menu sprite
	}
	else // MENU
	{
		BaseSprite* arr = *(this->menuSprites);
		drawSprites(arr, 4);
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
	BaseSprite* sprite = (*this->gameSprites)[0];
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
	}
}

void GameStateMachine_PausedProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent)
{

}

void GameStateMachine_MainMenuProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent)
{
	static bool newGameSelected;

	BaseSprite* selSprite = (*this->menuSprites)[3];
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
		else if(key == KEY_CTRL && newGameSelected == true) {
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
