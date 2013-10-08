/*
 * GameStateMachine.h
 *
 *  Created on: 2013-10-08
 *      Author: Jill
 */

#ifndef GAMESTATEMACHINE_H_
#define GAMESTATEMACHINE_H_

#include "../io/PS2Keyboard.h"

typedef enum {START, MAIN_MENU, PLAYING, PAUSED, GAME_OVER} GameStateE;

typedef struct GameStateMachine
{
	// The game state
	GameStateE state;

	PS2Keyboard* keyboard;

	// TODO Sprite arrays

} GameStateMachine;

GameStateMachine* GameStateMachine_alloc();
GameStateMachine* GameStateMachine_init(GameStateMachine* this);

#endif /* GAMESTATEMACHINE_H_ */
