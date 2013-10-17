/*
 * EnemyHandler.c
 *
 *  Created on: 2013-10-17
 *  Author: Trevor Siemens
 */

#include "EnemyHandler.h"
#include <stdlib.h>

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
	SpriteParser_parse("enemy", enemy); // TODO change image to something else
	enemy->baseSprite.animTimer = Timer_init(Timer_alloc(), ENEMY_DEATH_TIME);
}

ImgSprite* EnemyHandler_getNewRandomEnemy()
{
	ImgSprite* enemy = NULL;
	if (s_numOpenCols < MAX_ENEMIES)
	{
		int col = getRandomOpenCol();
		s_occupiedCols[col] = 1;
		s_numOpenCols--;
		enemy = SpriteFactory_generateEnemySprite(rand() % 10, col);
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
