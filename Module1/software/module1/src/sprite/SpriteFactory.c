#include "SpriteFactory.h"
#include "../sprite/ImgSprite.h"
#include "../sprite/RectSprite.h"
#include "../sprite/AlphaSprite.h"
#include "../sprite/SpriteParser.h"
#include "../sprite/BaseSprite.h"

/*
 * REQUIRES: SD card inserted containing "play.bmp"
 * EFFECTS: Returns a pointer to an initialized player sprite
 */
ImgSprite* SpriteFactory_generatePlayerSprite()
{
	//Load the player image from the SD card
	ImgSprite* playerSprite = ImgSprite_init(ImgSprite_alloc());
	SpriteParser_parse("play", playerSprite);

	//Assign sprite properties;
	((BaseSprite*)playerSprite)->spriteId = PLAYER_SPRITE_ID;
	BaseSprite_setPosition((BaseSprite*)playerSprite, 150, 200);
	return playerSprite;
}

/*
 * REQUIRES: SD card inserted containing enemy images
 * EFFECTS: Returns a pointer to an initialized enemy sprite
 */
ImgSprite* SpriteFactory_generateEnemySprite()
{
}

/*
 *
 *
 */
RectSprite* SpriteFactory_generateLaserSprite(BaseSprite* player, int colour)
{
	RectSprite* laserSprite = RectSprite_init(RectSprite_alloc());
	((BaseSprite*)laserSprite)->spriteId = PLAYER_LASER_SPRITE_ID;
	laserSprite->baseSprite.width = 1;
	laserSprite->baseSprite.height = player->yPos - 1;
	laserSprite->baseSprite.xPos = player->xPos + (player->width/2) - 1;
	laserSprite->baseSprite.yPos = 0;
	laserSprite->colour = colour;
	return laserSprite;
}
