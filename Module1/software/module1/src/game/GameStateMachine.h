/*
 * GameStateMachine.h
 *
 *  Created on: 2013-10-08
 *      Author: Jill
 */

#ifndef GAMESTATEMACHINE_H_
#define GAMESTATEMACHINE_H_

#include "../sprite/SpriteFactory.h"
#include "../io/PS2Keyboard.h"
#include "../video/VideoHandler.h"
#include "../sprite/BaseSprite.h"
#include "../sprite/SpriteArrayList.h"
#include "../sprite/ImgSprite.h"
#include "../sprite/RectSprite.h"
#include "../sprite/AlphaSprite.h"
#include "../sprite/SpriteParser.h"

#define PLAYER_SPEED 0.3
#define LASER_DURATION 200
#define ENEMY_SHOT_DURATION 2000

typedef enum {START, MAIN_MENU, INSTRUCTIONS, PLAYING, PAUSED, GAME_OVER} GameState;

typedef struct GameStateMachine
{
	// The game state
	GameState state;

	PS2Keyboard* keyboard;
	SpriteArrayList* gameSprites;

	SpriteArrayList* menuSprites;

	SpriteArrayList* instructionSprites;

	SpriteArrayList* scorebarSprites;

	SpriteArrayList* pausedSprites;

	Timer* frameTimer;
	double lastFrameDuration;

	int level;
	int target;
	int current;

} GameStateMachine;

GameStateMachine* GameStateMachine_alloc();
GameStateMachine* GameStateMachine_init(GameStateMachine* this, PS2Keyboard* keyboard);
void GameStateMachine_PerformLogic(GameStateMachine* this);
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




#endif /* GAMESTATEMACHINE_H_ */


