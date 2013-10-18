/*
 * GameStateMachine.c
 *
 *  Created on: 2013-10-08
 *      Author: Jill
 */

#include "GameStateMachine.h"

#include <stdlib.h>
#include <stdio.h>

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
	this->menuSprites = SpriteFactory_generateMenu(1);
	this->pausedSprites = SpriteFactory_generateMenu(0);
	this->levelSprites = SpriteFactory_generateLevelMenu();
	this->level = 0;
	this->current = 0;
	this->target = 0;
	this->scorebarSprites = SpriteFactory_generateScoreBar();
	this->instructionSprites = SpriteFactory_generateInstructions();
	this->winSprites = SpriteFactory_generateWinScreen();

	this->gameSprites = SpriteArrayList_init(SpriteArrayList_alloc(), 2);

	this->frameTimer = Timer_init(Timer_alloc(), 0);
	this->lastFrameDuration = 0;

	ImgSprite* playerSprite = SpriteFactory_generatePlayerSprite();
	SpriteArrayList_insert(this->gameSprites, (BaseSprite*)playerSprite, 0);

	EnemyHandler_init();

	return this;
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
		VideoHandler_drawSprites(this->scorebarSprites);
	}
	else if(this->state == LEVEL_MENU)
	{
		VideoHandler_drawSprites(this->levelSprites);
	}
	else if (this->state == PAUSED)
	{
		VideoHandler_drawSprites(this->pausedSprites);
	}
	else if (this->state == INSTRUCTIONS)
	{
		VideoHandler_drawSprites(this->instructionSprites);
	}
	else if (this->state == MAIN_MENU)
	{
		VideoHandler_drawSprites(this->menuSprites);
	}
	else if (this->state == WON)
	{
		VideoHandler_drawSprites(this->winSprites);
	}
}

void GameStateMachine_ProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent)
{
	srand(alt_timestamp());
	switch( this->state )
	{
	    case START:
	        GameStateMachine_StartProcessKey(this, key, isUpEvent);
	        break;
	    case MAIN_MENU:
	    	GameStateMachine_MainMenuProcessKey(this, key, isUpEvent);
	    	break;
	    case LEVEL_MENU:
	    	GameStateMachine_LevelMenuProcessKey(this, key, isUpEvent);
	    	break;
	    case INSTRUCTIONS:
	    	GameStateMachine_InstructionsProcessKey(this, key, isUpEvent);
	    	break;
	    case PLAYING:
	    	GameStateMachine_PlayingProcessKey(this, key, isUpEvent);
	    	break;
	    case NEXT_LEVEL:
	    	GameStateMachine_NextLevelProcessKey(this, key, isUpEvent);
	    	break;
	    case WON:
	    	GameStateMachine_WonProcessKey(this, key, isUpEvent);
	    	break;
	    case PAUSED:
	    	GameStateMachine_PausedProcessKey(this, key, isUpEvent);
	    	break;
	    case GAME_OVER:
	    	GameStateMachine_GameOverProcessKey(this, key, isUpEvent);
	    	break;
	}
}

void GameStateMachine_PerformLogic(GameStateMachine* this)
{
	switch( this->state )
	{
	    case START:
	        GameStateMachine_StartPerformLogic(this);
	        break;
	    case MAIN_MENU:
	    	GameStateMachine_MainMenuPerformLogic(this);
	    	break;
	    case LEVEL_MENU:
	    	GameStateMachine_LevelMenuPerformLogic(this);
	    	break;
	    case INSTRUCTIONS:
	    	GameStateMachine_InstructionsPerformLogic(this);
	    	break;
	    case PLAYING:
	    	GameStateMachine_PlayingPerformLogic(this);
	    	break;
	    case NEXT_LEVEL:
			GameStateMachine_NextLevelPerformLogic(this);
			break;
	    case WON:
	    	GameStateMachine_WonPerformLogic(this);
	    	break;
	    case PAUSED:
	    	GameStateMachine_PausedPerformLogic(this);
	    	break;
	    case GAME_OVER:
	    	GameStateMachine_GameOverPerformLogic(this);
	    	break;
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
				laserColour = LASER_ADD_COLOUR;
			else if (key == '2')
				laserColour = LASER_SUB_COLOUR;
			else if (key == '3')
				laserColour = LASER_MUL_COLOUR;
			else if (key == '4')
				laserColour = LASER_DIV_COLOUR;

			// set audio to play shooting effects
			AudioHandler_playShoot();

			laserSprite = SpriteFactory_generateLaserSprite(player, laserColour);
			laserSprite->baseSprite.animTimer = Timer_init(Timer_alloc(), LASER_DURATION);
			Timer_start(laserSprite->baseSprite.animTimer);
			SpriteArrayList_insert(this->gameSprites, (BaseSprite*)laserSprite, this->gameSprites->last + 1);
		}
	}
	else if(key == KEY_ESC && isUpEvent == 0) {
		clearChar();
		this->state = PAUSED;
	}
	else if(key == 'e' && isUpEvent == 0) {
		ImgSprite* newEnemy = EnemyHandler_getNewRandomEnemy((this->difficulty)%3);
		if (newEnemy != NULL){
			SpriteArrayList_insert(this->gameSprites, (BaseSprite*)newEnemy, 0);
		}
	}
}

void GameStateMachine_NextLevelProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent)
{

}

void GameStateMachine_WonProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent)
{
	if(isUpEvent == 0)
	{
		clearChar();
		this->state = MAIN_MENU;
	}
}

void GameStateMachine_PausedProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent)
{
	static MenuSelection pauseMenuSelection = SEL_RESUME;
	BaseSprite* selSprite = SpriteArrayList_getAt(this->pausedSprites, 3);

	if(isUpEvent == 0)
	{
		if(key == KEY_DOWN)
		{
			pauseMenuSelection = SEL_QUIT;
			selSprite->yPos = MENU_SELECTOR_CONTINUE_YPOS;

		}
		else if(key == KEY_UP)
		{
			pauseMenuSelection = SEL_RESUME;
			selSprite->yPos = MENU_SELECTOR_NEWGAME_YPOS;
		}
		else if(key == '\n' && pauseMenuSelection == SEL_RESUME)
		{
			clearChar();
			this->state = PLAYING;
		}
		else if(key == '\n' && pauseMenuSelection == SEL_QUIT)
		{
			clearChar();
			this->state = MAIN_MENU;
			this->level = 0;
		}
	}

}

void GameStateMachine_LevelMenuProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent)
{
	static GameLevel selectedLevel = ADD_EASY;
	BaseSprite* selSprite = SpriteArrayList_getAt(this->levelSprites, 5);
	if(isUpEvent == 0)
	{
		if(key == KEY_DOWN && selectedLevel != MUL_HARD) {
			if(selectedLevel == ADD_EASY) // addeasy to addmedium
			{
				selectedLevel = ADD_MEDIUM;
				selSprite->yPos = LEVELMENU_SELECTOR_MEDIUM_YPOS;
			}
			else if(selectedLevel == ADD_MEDIUM) // addMedium to addHard
			{
				selectedLevel = ADD_HARD;
				selSprite->yPos = LEVELMENU_SELECTOR_HARD_YPOS;
			}
			else if(selectedLevel == ADD_HARD) //addHard to mulEasy
			{
				selectedLevel = MUL_EASY;
				selSprite->yPos = LEVELMENU_SELECTOR_MUL_EASY_YPOS;
			}
			else if(selectedLevel == MUL_EASY) //mulEasy to mulMedium
			{
				selectedLevel  = MUL_MEDIUM;
				selSprite->yPos = LEVELMENU_SELECTOR_MUL_MEDIUM_YPOS;
			}
			else if(selectedLevel == MUL_MEDIUM) //mulMedium to mulHard
			{
				selectedLevel = MUL_HARD;
				selSprite->yPos = LEVELMENU_SELECTOR_MUL_HARD_YPOS;
			}
		}
		else if(key == KEY_UP && selectedLevel != ADD_EASY)
		{
			if(selectedLevel == ADD_MEDIUM) // addMedium to addEasy
			{
				selectedLevel = ADD_EASY;
				selSprite->yPos = LEVELMENU_SELECTOR_EASY_YPOS;
			}
			else if(selectedLevel == ADD_HARD) // addHard to addMedium
			{
				selectedLevel = ADD_MEDIUM;
				selSprite->yPos = LEVELMENU_SELECTOR_MEDIUM_YPOS;
			}
			else if(selectedLevel == MUL_EASY) // mulEasy to addHard
			{
				selectedLevel = ADD_HARD;
				selSprite->yPos = LEVELMENU_SELECTOR_HARD_YPOS;
			}
			else if(selectedLevel == MUL_MEDIUM) // mulMedium to mulEasy
			{
				selectedLevel = MUL_EASY;
				selSprite->yPos = LEVELMENU_SELECTOR_MUL_EASY_YPOS;
			}
			else if(selectedLevel == MUL_HARD) // mulHard to mulMedium
			{
				selectedLevel = MUL_MEDIUM;
				selSprite->yPos = LEVELMENU_SELECTOR_MUL_MEDIUM_YPOS;
			}
		}
		else if(key == '\n')
		{
			clearChar();

			this->state = NEXT_LEVEL;
			this->difficulty = selectedLevel;
		}
		else if(key == KEY_ESC)
		{
			clearChar();
			this->state = MAIN_MENU;
		}
	}

}

void GameStateMachine_MainMenuProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent)
{
	static MenuSelection mainMenuSelection = SEL_INSTRUCTIONS;

	BaseSprite* selSprite = SpriteArrayList_getAt(this->menuSprites, 3);
	if(isUpEvent == 0)
	{
		if(key == KEY_DOWN) {
			selSprite->yPos = MENU_SELECTOR_CONTINUE_YPOS;
			mainMenuSelection = SEL_INSTRUCTIONS;
		}
		else if(key == KEY_UP) {
			selSprite->yPos = MENU_SELECTOR_NEWGAME_YPOS;
			mainMenuSelection = SEL_NEWGAME;
		}
		else if(key == '\n' && mainMenuSelection == SEL_NEWGAME) {
			//clearChar() to be replaced by clearing individual strings
			clearChar();
			this->state = LEVEL_MENU;
		}
		else if(key == '\n' && mainMenuSelection == SEL_INSTRUCTIONS) {
			clearChar();
			this->state = INSTRUCTIONS;
		}
	}
}

void GameStateMachine_InstructionsProcessKey(GameStateMachine* this, alt_u8 key, int isUpEvent)
{
	static InstuctionPage insPage;
	if(isUpEvent == 0)
	{
		if (key == KEY_ESC)
		{
			clearChar();
			this->state = MAIN_MENU;
		}
		else if(key == KEY_RIGHT && insPage == INS_FIRSTPAGE)
		{
			insPage = INS_SECONDPAGE;
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
		else if(key == KEY_LEFT && insPage == INS_SECONDPAGE)
		{
			insPage = INS_FIRSTPAGE;
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

void GameStateMachine_InstructionsPerformLogic(GameStateMachine* this)
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
	BaseSprite* enemySprite;
	int i;
	for(i=0; i <= this->gameSprites->last; i++){
		enemySprite = SpriteArrayList_getAt(this->gameSprites, i);
		if (enemySprite != NULL && enemySprite->spriteId > ENEMY_SPRITE_ID_BASE){
			BaseSprite_updatePos(enemySprite, this->lastFrameDuration);

			// Check if shield hits, if so, remove and perform operation on value
			if (enemySprite->animTimer == NULL && laserSprite->xPos > enemySprite->xPos && laserSprite->xPos < (enemySprite->xPos + enemySprite->width) ) {
				EnemyHandler_enemyShot((ImgSprite*)enemySprite);
				// set audio to play shooting effects
				AudioHandler_playHit();
				if (enemySprite->spriteId == 10)
					this->current += 10;
				else
					this->current += enemySprite->spriteId - 10;
			}
			// Removes enemy sprite if moved to the very bottom
			if ( (enemySprite->yPos + enemySprite->height) >= 240) {
				enemySprite->animTimer = Timer_init(Timer_alloc(), 0);
			}

			// Check for timer duration, destroy enemy if times up
			if (Timer_isDone(enemySprite->animTimer) == 1) {
				EnemyHandler_notifyEnemyDestroyed(enemySprite);
				SpriteArrayList_removeObject(this->gameSprites, enemySprite);
				ImgSprite_free((ImgSprite*)enemySprite);
				if (this->current == this->target)
					this->state = NEXT_LEVEL;
			}
		}
	}

	char* currentString = ((AlphaSprite*)SpriteArrayList_getWithId(this->scorebarSprites, SCOREBAR_CURRENT_ID))->string;
	sprintf(currentString, "%i", this->current);


}

// Called on New Game or when win condition met
// Increments level and sets a new target value
void GameStateMachine_NextLevelPerformLogic(GameStateMachine* this)
{
	// Set values for a new game
	this->level++;
	this->target = rand()%((this->level*10)+10);
	this->current = 0;

	// Throw values up on the scorebar
	char* targetString = ((AlphaSprite*)SpriteArrayList_getWithId(this->scorebarSprites, SCOREBAR_TARGET_ID))->string;
	sprintf(targetString, "%i", this->target);
	char* levelString = ((AlphaSprite*)SpriteArrayList_getWithId(this->scorebarSprites, SCOREBAR_LEVEL_ID))->string;
	sprintf(levelString, "%i", this->level);

	// Delete all on-screen enemies
	BaseSprite* enemySprite;
	int i;
	for(i=0; i <= this->gameSprites->last; i++){
		enemySprite = SpriteArrayList_getAt(this->gameSprites, i);
		if (enemySprite != NULL && enemySprite->spriteId > ENEMY_SPRITE_ID_BASE){
			enemySprite->animTimer = Timer_init(Timer_alloc(), 0);
		}
	}

	clearChar();
	if(this->level == 2) //TODO: change it to 11
		this->state = WON;
	else
		this->state = PLAYING;
}

void GameStateMachine_PausedPerformLogic(GameStateMachine* this)
{

}

void GameStateMachine_LevelMenuPerformLogic(GameStateMachine* this)
{

}

void GameStateMachine_MainMenuPerformLogic(GameStateMachine* this)
{

}

void GameStateMachine_GameOverPerformLogic(GameStateMachine* this)
{

}

void GameStateMachine_WonPerformLogic(GameStateMachine* this)
{

}
