#ifndef SPRITEFACTORY_H_
#define SPRITEFACTORY_H_

#include "../sprite/BaseSprite.h"
#include "../sprite/ImgSprite.h"
#include "../sprite/RectSprite.h"
#include "../sprite/AlphaSprite.h"
#include "../sprite/SpriteParser.h"

ImgSprite* SpriteFactory_generatePlayerSprite();
ImgSprite* SpriteFactory_generateEnemySprite();
RectSprite* SpriteFactory_generateLaserSprite(BaseSprite* player, int colour);

#define PLAYER_SPRITE_ID 1
#define PLAYER_LASER_SPRITE_ID 2
#define ENEMY_SPRITE_ID_BASE 10

#endif /* SPRITEFACTORY_H_ */
