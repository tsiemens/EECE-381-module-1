#ifndef SPRITEFACTORY_H_
#define SPRITEFACTORY_H_

#include "../sprite/ImgSprite.h"
#include "../sprite/RectSprite.h"
#include "../sprite/AlphaSprite.h"
#include "../sprite/SpriteParser.h"

ImgSprite* SpriteFactory_GeneratePlayerSprite();
ImgSprite* SpriteFactory_GenerateEnemySprite();
ImgSprite* SpriteFactory_GenerateLaserSprite();

#define PLAYER_SPRITE_ID 1
#define PLAYER_LASER_SPRITE_ID 2
#define ENEMY_SPRITE_ID 3

#endif /* SPRITEFACTORY_H_ */
