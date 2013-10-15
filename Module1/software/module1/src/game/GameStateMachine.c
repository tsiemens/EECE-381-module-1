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
	//TODO: un-ghetto this
	this->state = PLAYING;
	this->keyboard = keyboard;
	ImgSprite* img = ImgSprite_init(ImgSprite_alloc());
	SpriteParser_parse("play", img);
	BaseSprite_setPosition((BaseSprite*)img, 150, 200);
	this->menuSprites = SpriteArrayList_init(SpriteArrayList_alloc(), 5);
	this->instructionSprites = SpriteArrayList_init(SpriteArrayList_alloc(), 8);
	this->gameSprites = SpriteArrayList_init(SpriteArrayList_alloc(), 1);
	SpriteArrayList_insert(this->gameSprites, (BaseSprite*)img, 0);
	printf("returning\n");

//	menuInit(this);
//	instructionsInit(this);

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
	instructionsAlpha->setString(instructionsAlpha, "You are on a journey to Planet Math. Your goal is to");

	AlphaSprite* instructionsAlpha2 = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)instructionsAlpha2, INSTRUCTIONITEM_TEXT_XPOS, INSTRUCTIONITEM_TITLE_YPOS+CHAR_TO_PIXEL_HEIGHT*2);
	instructionsAlpha2->setString(instructionsAlpha2, "eat an exact amount of space burgers. Too few and");

	AlphaSprite* instructionsAlpha3 = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)instructionsAlpha3, INSTRUCTIONITEM_TEXT_XPOS, INSTRUCTIONITEM_TITLE_YPOS+CHAR_TO_PIXEL_HEIGHT*3);
	instructionsAlpha3->setString(instructionsAlpha3, "you won't make it. Too many, you will fall asleep.");

	AlphaSprite* instructionsAlpha4 = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)instructionsAlpha4, INSTRUCTIONITEM_TEXT_XPOS, INSTRUCTIONITEM_TITLE_YPOS+CHAR_TO_PIXEL_HEIGHT*4);
	instructionsAlpha4->setString(instructionsAlpha4, "Grab the burger with desired number & use different");

	AlphaSprite* instructionsAlpha5 = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)instructionsAlpha5, INSTRUCTIONITEM_TEXT_XPOS, INSTRUCTIONITEM_TITLE_YPOS+CHAR_TO_PIXEL_HEIGHT*5);
	instructionsAlpha5->setString(instructionsAlpha5, "lasers to use different operators. Good luck!");

	AlphaSprite* instructionsBackToMenu= AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)instructionsBackToMenu, INSTRUCTIONITEM_ESC_XPOS, MENUITEM_CONTINUE_YPOS);
	instructionsAlpha5->setString(instructionsBackToMenu, "Press ESC to return to menu");


	SpriteArrayList_insert(this->menuSprites, (BaseSprite*)menu, 0);
	SpriteArrayList_insert(this->menuSprites, (BaseSprite*)instructionsTitleAlpha, 1);
	SpriteArrayList_insert(this->menuSprites, (BaseSprite*)instructionsAlpha, 2);
	SpriteArrayList_insert(this->menuSprites, (BaseSprite*)instructionsAlpha2, 3);
	SpriteArrayList_insert(this->menuSprites, (BaseSprite*)instructionsAlpha3, 4);
	SpriteArrayList_insert(this->menuSprites, (BaseSprite*)instructionsAlpha4, 5);
	SpriteArrayList_insert(this->menuSprites, (BaseSprite*)instructionsAlpha5, 6);
	SpriteArrayList_insert(this->menuSprites, (BaseSprite*)instructionsBackToMenu, 7);
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

GameStateMachine_InstructionsProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent)
{

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
