/*
 * EnemyHandler.h
 *
 *  Created on: 2013-10-17
 *      Author: trevsiemens
 */

#ifndef ENEMYHANDLER_H_
#define ENEMYHANDLER_H_

#include "../sprite/SpriteFactory.h"
#include "../sprite/SpriteArrayList.h"
#include "../sprite/BaseSprite.h"
#include "../sprite/ImgSprite.h"

#define MAX_ENEMIES 3
#define ENEMY_DEATH_TIME 400.0

void EnemyHandler_init();

void EnemyHandler_notifyEnemyDestroyed(BaseSprite* enemy);

void EnemyHandler_enemyShot(ImgSprite* enemy);

ImgSprite* EnemyHandler_getNewRandomEnemy(EnemyLevel enemyLevel);

#endif /* ENEMYHANDLER_H_ */
