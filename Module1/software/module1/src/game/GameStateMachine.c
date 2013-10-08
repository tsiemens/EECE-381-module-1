/*
 * GameStateMachine.c
 *
 *  Created on: 2013-10-08
 *      Author: Jill
 */

#include "GameStateMachine.h"
#include "../io/PS2Keyboard.h"
#include <stdlib.h>
#include <stdio.h>

void GameStateMachine_ProcessKey(GameStateMachine* this, char key, int isUpEvent);
void GameStateMachine_PerformLogic(GameStateMachine* this);
void GameStateMachine_StartProcessKey(GameStateMachine* this, char key, int isUpEvent);
void GameStateMachine_PlayingProcessKey(GameStateMachine* this, char key, int isUpEvent);
void GameStateMachine_PausedProcessKey(GameStateMachine* this, char key, int isUpEvent);
void GameStateMachine_MainMenuProcessKey(GameStateMachine* this, char key, int isUpEvent);
void GameStateMachine_GameOverProcessKey(GameStateMachine* this, char key, int isUpEvent);
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
	this->state = START;
	this->keyboard = keyboard;
	return this;
}

void performFrameLogic(GameStateMachine* this){

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
		// draw game sprites
	}
	else if (this->state == PAUSED)
	{
		// draw game sprites, then draw menu sprite
	}
	else // MENU
	{
		// draw main menu sprites
	}
}

void GameStateMachine_ProcessKey(GameStateMachine* this, char key, int isUpEvent)
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

void GameStateMachine_StartProcessKey(GameStateMachine* this, char key, int isUpEvent)
{

}

void GameStateMachine_PlayingProcessKey(GameStateMachine* this, char key, int isUpEvent)
{

}

void GameStateMachine_PausedProcessKey(GameStateMachine* this, char key, int isUpEvent)
{

}

void GameStateMachine_MainMenuProcessKey(GameStateMachine* this, char key, int isUpEvent)
{

}

void GameStateMachine_GameOverProcessKey(GameStateMachine* this, char key, int isUpEvent)
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
