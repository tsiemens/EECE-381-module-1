/*
 * GameStateMachine.h
 *
 *  Created on: 2013-10-08
 *      Author: Jill
 */

#ifndef GAMESTATEMACHINE_H_
#define GAMESTATEMACHINE_H_

#include "../io/PS2Keyboard.h"
#include "../sprite/BaseSprite.h"
#include "../sprite/SpriteArrayList.h"

typedef enum {START, MAIN_MENU, PLAYING, PAUSED, GAME_OVER} GameStateE;

typedef struct GameStateMachine
{
	// The game state
	GameStateE state;

	PS2Keyboard* keyboard;
	SpriteArrayList* gameSprites;
	SpriteArrayList* menuSprites;

} GameStateMachine;

GameStateMachine* GameStateMachine_alloc();
GameStateMachine* GameStateMachine_init(GameStateMachine* this, PS2Keyboard* keyboard);
void menuInit(GameStateMachine* this);
void GameStateMachine_PerformLogic(GameStateMachine* this);

#endif /* GAMESTATEMACHINE_H_ */
