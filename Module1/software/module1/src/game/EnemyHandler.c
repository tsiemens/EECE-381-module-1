/*
 * EnemyHandler.c
 *
 *  Created on: 2013-10-17
 *  Author: Trevor Siemens
 */

#include "EnemyHandler.h"
#include <stdlib.h>

static char s_occupiedCols[ENEMY_COLS];
static int s_numOpenCols;

int getRandomOpenCol();

void EnemyHandler_init()
{
	int i;
	for(i = 0; i < ENEMY_COLS; i++){
		s_occupiedCols[i] = 0;
	}
	s_numOpenCols = ENEMY_COLS;
}

void EnemyHandler_notifyEnemyDestroyed(BaseSprite* enemy)
{
	if(enemy != NULL){
		int col = (int)enemy->xPos/ENEMY_COL_WIDTH;
		s_occupiedCols[col] = 0;
		s_numOpenCols++;
	}
}

void EnemyHandler_enemyShot(ImgSprite* enemy)
{
	free(enemy->colours);
	SpriteParser_parse("ymene", enemy);
	enemy->baseSprite.yVel = 0;
	((BaseSprite*)enemy)->animTimer = Timer_init(Timer_alloc(), ENEMY_DEATH_TIME);
	Timer_start(((BaseSprite*)enemy)->animTimer);
}

ImgSprite* EnemyHandler_getNewRandomEnemy()
{
	ImgSprite* enemy = NULL;
	if ((ENEMY_COLS - s_numOpenCols) < MAX_ENEMIES)
	{
		int col = getRandomOpenCol();
		s_occupiedCols[col] = 1;
		s_numOpenCols--;
		enemy = SpriteFactory_generateEnemySprite(rand() % 10 + 1, col);
	}
	return enemy;
}

int getRandomOpenCol()
{
	int randVal = rand() % s_numOpenCols;
	int emptyColsFound = 0;
	int i;
	for(i = 0; i < ENEMY_COLS; i++){
		if (s_occupiedCols[i] == 0){
			if(emptyColsFound == randVal){
				return i;
			}
			emptyColsFound++;
		}
	}
	return 0;
}
