#include "SpriteFactory.h"

/*
 * REQUIRES: SD card inserted containing "play"
 * EFFECTS: Returns a pointer to an initialized player sprite
 */
ImgSprite* SpriteFactory_generatePlayerSprite()
{
	//Load the player image from the SD card
	ImgSprite* playerSprite = ImgSprite_init(ImgSprite_alloc());
	SpriteParser_parse("player", playerSprite);

	//Assign sprite properties;
	((BaseSprite*)playerSprite)->spriteId = PLAYER_SPRITE_ID;
	BaseSprite_setPosition((BaseSprite*)playerSprite, 150, 200);
	return playerSprite;
}

/*
 * REQUIRES: SD card inserted containing enemy images
 * EFFECTS: Returns a pointer to an initialized enemy sprite based on the value given.
 * 			Returns null if value not 1 <= value <= 10
 */
ImgSprite* SpriteFactory_generateEnemySprite(int value)
{
	char* image;
	if (value < 1 || value > 10)
		return NULL;



	//Load the enemy image from the SD card
	ImgSprite* sprite = ImgSprite_init(ImgSprite_alloc());
	SpriteParser_parse("player", sprite);

	//Assign sprite properties;
	((BaseSprite*)sprite)->spriteId = PLAYER_SPRITE_ID;
	BaseSprite_setPosition((BaseSprite*)sprite, 150, 200);
	return sprite;
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

/*
 * Returns the main menu as a SpriteArrayList.
 * Simply replace the "menuSprites" list in GameStateMachine to use
 */

SpriteArrayList* SpriteFactory_generateMainMenu()
{
	SpriteArrayList* mainMenuSprites = SpriteArrayList_init(mainMenuSprites, 5);

	RectSprite* menuOuterFrame = RectSprite_init(RectSprite_alloc());
	BaseSprite_setSize((BaseSprite*)menuOuterFrame, MENUFRAME_WIDTH, MENUFRAME_HEIGHT);
	BaseSprite_setPosition((BaseSprite*)menuOuterFrame, MENUFRAME_XPOS, MENUFRAME_YPOS);
	menuOuterFrame->colour = MENUFRAME_COLOR;

	AlphaSprite* menuStartAlpha = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)menuStartAlpha, MENUITEM_START_XPOS, MENUITEM_START_YPOS);
	menuStartAlpha->setString(menuStartAlpha, "Start");

	AlphaSprite* menuContinueAlpha = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)menuContinueAlpha, MENUITEM_START_XPOS, MENUITEM_CONTINUE_YPOS);
	menuContinueAlpha->setString(menuContinueAlpha, "Instructions");

	RectSprite* menuSelectorFrame = RectSprite_init(RectSprite_alloc());
	BaseSprite_setSize((BaseSprite*)menuSelectorFrame, MENU_SELECTOR_WIDTH, MENU_SELECTOR_HEIGHT);
	BaseSprite_setPosition((BaseSprite*)menuSelectorFrame, MENU_SELECTOR_XPOS, MENU_SELECTOR_CONTINUE_YPOS);
	menuSelectorFrame->colour = MENU_SELECTOR_COLOR;

	ImgSprite* menu = ImgSprite_init(ImgSprite_alloc());
	SpriteParser_parse("title", menu);
	BaseSprite_setPosition((BaseSprite*)menu, 90, 60);

	SpriteArrayList_insert(mainMenuSprites, (BaseSprite*)menuOuterFrame, 0);
	SpriteArrayList_insert(mainMenuSprites, (BaseSprite*)menuStartAlpha, 1);
	SpriteArrayList_insert(mainMenuSprites, (BaseSprite*)menuContinueAlpha, 2);
	SpriteArrayList_insert(mainMenuSprites, (BaseSprite*)menuSelectorFrame, 3);
	SpriteArrayList_insert(mainMenuSprites, (BaseSprite*)menu, 4);

	return mainMenuSprites;
}

SpriteArrayList* SpriteFactory_generateInstructions()
{
	SpriteArrayList* instructionSprites = SpriteArrayList_init(SpriteArrayList_alloc(), 9);

	ImgSprite* menu = ImgSprite_init(ImgSprite_alloc());
	SpriteParser_parse("title", menu);
	BaseSprite_setPosition((BaseSprite*)menu, 90, 20);

	AlphaSprite* instructionsTitleAlpha = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)instructionsTitleAlpha, INSTRUCTIONITEM_TITLE_XPOS, INSTRUCTIONITEM_TITLE_YPOS);
	instructionsTitleAlpha->setString(instructionsTitleAlpha, "INSTRUCTIONS");

	AlphaSprite* instructionsAlpha = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)instructionsAlpha, INSTRUCTIONITEM_TEXT_XPOS, INSTRUCTIONITEM_TITLE_YPOS+CHAR_TO_PIXEL_HEIGHT);
	instructionsAlpha->setString(instructionsAlpha, INSTRUCTIONITEM_P1_L1);

	AlphaSprite* instructionsAlpha2 = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)instructionsAlpha2, INSTRUCTIONITEM_TEXT_XPOS, INSTRUCTIONITEM_TITLE_YPOS+CHAR_TO_PIXEL_HEIGHT*2);
	instructionsAlpha2->setString(instructionsAlpha2, INSTRUCTIONITEM_P1_L2);

	AlphaSprite* instructionsAlpha3 = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)instructionsAlpha3, INSTRUCTIONITEM_TEXT_XPOS, INSTRUCTIONITEM_TITLE_YPOS+CHAR_TO_PIXEL_HEIGHT*3);
	instructionsAlpha3->setString(instructionsAlpha3, INSTRUCTIONITEM_P1_L3);

	AlphaSprite* instructionsAlpha4 = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)instructionsAlpha4, INSTRUCTIONITEM_TEXT_XPOS, INSTRUCTIONITEM_TITLE_YPOS+CHAR_TO_PIXEL_HEIGHT*4);
	instructionsAlpha4->setString(instructionsAlpha4, INSTRUCTIONITEM_P1_L4);

	AlphaSprite* instructionsAlpha5 = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)instructionsAlpha5, INSTRUCTIONITEM_TEXT_XPOS, INSTRUCTIONITEM_TITLE_YPOS+CHAR_TO_PIXEL_HEIGHT*5);
	instructionsAlpha5->setString(instructionsAlpha5, INSTRUCTIONITEM_P1_L5);

	AlphaSprite* instructionsAlphaRight = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)instructionsAlphaRight, INSTRUCTIONITEM_TITLE_XPOS+5, INSTRUCTIONITEM_TITLE_YPOS+CHAR_TO_PIXEL_HEIGHT*6);
	instructionsAlphaRight->setString(instructionsAlphaRight, ">");

	AlphaSprite* instructionsBackToMenu= AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)instructionsBackToMenu, INSTRUCTIONITEM_ESC_XPOS, MENUITEM_CONTINUE_YPOS+CHAR_TO_PIXEL_HEIGHT*2);
	instructionsAlpha5->setString(instructionsBackToMenu, "Press ESC to return to menu");

	SpriteArrayList_insert(instructionSprites, (BaseSprite*)menu, 0);
	SpriteArrayList_insert(instructionSprites, (BaseSprite*)instructionsTitleAlpha, 1);
	SpriteArrayList_insert(instructionSprites, (BaseSprite*)instructionsAlpha, 2);
	SpriteArrayList_insert(instructionSprites, (BaseSprite*)instructionsAlpha2, 3);
	SpriteArrayList_insert(instructionSprites, (BaseSprite*)instructionsAlpha3, 4);
	SpriteArrayList_insert(instructionSprites, (BaseSprite*)instructionsAlpha4, 5);
	SpriteArrayList_insert(instructionSprites, (BaseSprite*)instructionsAlpha5, 6);
	SpriteArrayList_insert(instructionSprites, (BaseSprite*)instructionsAlphaRight, 7);
	SpriteArrayList_insert(instructionSprites, (BaseSprite*)instructionsBackToMenu, 8);

	return instructionSprites;
}

/*
 * Returns the SpriteArrayList for the top bar during play where the score, current val and target val are stored.
 *
 */
SpriteArrayList* SpriteFactory_generateScoreBar()
{
	SpriteArrayList* scorebarSprites = SpriteArrayList_init(SpriteArrayList_alloc(), 7);

	AlphaSprite* scorebarCurrentWord = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)scorebarCurrentWord, SCOREBAR_CURRENT_WORD_XPOS, SCOREBAR_WORD_YPOS);
	scorebarCurrentWord->setString(scorebarCurrentWord, SCOREBAR_CURRENT_STRING);

	AlphaSprite* scorebarCurrentVal = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)scorebarCurrentVal, SCOREBAR_CURRENT_VAL_XPOS, SCOREBAR_WORD_YPOS);
	scorebarCurrentVal->setString(scorebarCurrentVal, SCOREBAR_CURRENT_VAL);

	AlphaSprite* scorebarTargetWord = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)scorebarTargetWord, SCOREBAR_TARGET_WORD_XPOS, SCOREBAR_WORD_YPOS);
	scorebarTargetWord->setString(scorebarTargetWord, SCOREBAR_TARGET_STRING);

	AlphaSprite* scorebarTargetVal = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)scorebarTargetVal, SCOREBAR_TARGET_VAL_XPOS, SCOREBAR_WORD_YPOS);
	scorebarTargetVal->setString(scorebarTargetVal, SCOREBAR_TARGET_VAL);

	AlphaSprite* scorebarLevelWord = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)scorebarLevelWord, SCOREBAR_LEVEL_WORD_XPOS, SCOREBAR_WORD_YPOS);
	scorebarLevelWord->setString(scorebarLevelWord, SCOREBAR_LEVEL_STRING);

	AlphaSprite* scorebarLevelVal = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)scorebarLevelVal, SCOREBAR_LEVEL_VAL_XPOS, SCOREBAR_WORD_YPOS);
	scorebarLevelVal->setString(scorebarLevelVal, SCOREBAR_LEVEL_VAL);

	RectSprite* scorebarUnderline = RectSprite_init(RectSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)scorebarUnderline, 0, SCOREBAR_TOP_PADDING);
	BaseSprite_setSize((BaseSprite*)scorebarUnderline, CHAR_TO_PIXEL_WIDTH*(SCOREBAR_LEVEL_VAL_XPOS+SCOREBAR_VAL_WIDTH), 2);
	scorebarUnderline->colour = SCOREBAR_COLOR;

	SpriteArrayList_insert(scorebarSprites, (BaseSprite*)scorebarCurrentWord, 0);
	SpriteArrayList_insert(scorebarSprites, (BaseSprite*)scorebarCurrentVal, 1);
	SpriteArrayList_insert(scorebarSprites, (BaseSprite*)scorebarTargetWord, 2);
	SpriteArrayList_insert(scorebarSprites, (BaseSprite*)scorebarTargetVal, 3);
	SpriteArrayList_insert(scorebarSprites, (BaseSprite*)scorebarLevelWord, 4);
	SpriteArrayList_insert(scorebarSprites, (BaseSprite*)scorebarLevelVal, 5);
	SpriteArrayList_insert(scorebarSprites, (BaseSprite*)scorebarUnderline, 6);

	return scorebarSprites;
}

