#include "SpriteFactory.h"
#include <stdlib.h>

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
ImgSprite* SpriteFactory_generateEnemySprite(int value, int col, EnemyLevel enemyLevel)
{
	char* image = "enemy ";

	if(value == 10)
		image[5] = intToChar(0);
	else
		image[5] = intToChar(value);

	//Load the enemy image from the SD card
	ImgSprite* sprite = ImgSprite_init(ImgSprite_alloc());
	SpriteParser_parse(image, sprite);

	//Assign sprite properties;
	((BaseSprite*)sprite)->spriteId = ENEMY_SPRITE_ID_BASE + value;
	BaseSprite_setPosition((BaseSprite*)sprite, col*ENEMY_COL_WIDTH, 0);

	switch(enemyLevel)
		{
			case EASY:
				sprite->baseSprite.yVel = ENEMY_SPEED_EASY;
				break;
			case MEDIUM:
				sprite->baseSprite.yVel = ENEMY_SPEED_NORMAL;
				break;
			case HARD:
				sprite->baseSprite.yVel = ENEMY_SPEED_HARD;
				break;
		}
	sprite->baseSprite.xVel = 0;
	return sprite;
}

char intToChar(int val)
{
	switch(val) {
		case 0: return '0';
		case 1: return '1';
		case 2: return '2';
		case 3: return '3';
		case 4: return '4';
		case 5: return '5';
		case 6: return '6';
		case 7: return '7';
		case 8: return '8';
		case 9: return '9';
		default: return '\0';
	}
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
 * Returns a menu (either pause menu or a main menu) as a SpriteArrayList.
 * MAIN MENU: Simply replace the "menuSprites" list in GameStateMachine to use
 */

SpriteArrayList* SpriteFactory_generateMenu(int isMainMenu)
{
	SpriteArrayList* menuSprites = SpriteArrayList_init(SpriteArrayList_alloc(), isMainMenu+4);

	RectSprite* menuOuterFrame = RectSprite_init(RectSprite_alloc());
	BaseSprite_setSize((BaseSprite*)menuOuterFrame, MENUFRAME_WIDTH, MENUFRAME_HEIGHT);
	BaseSprite_setPosition((BaseSprite*)menuOuterFrame, MENUFRAME_XPOS, MENUFRAME_YPOS);
	menuOuterFrame->colour = MENUFRAME_COLOR;

	AlphaSprite* menuOption1 = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)menuOption1, MENUITEM_START_XPOS, MENUITEM_START_YPOS);

	AlphaSprite* menuOption2 = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)menuOption2, MENUITEM_START_XPOS, MENUITEM_CONTINUE_YPOS);

	RectSprite* menuSelectorFrame = RectSprite_init(RectSprite_alloc());
	BaseSprite_setSize((BaseSprite*)menuSelectorFrame, MENU_SELECTOR_WIDTH, MENU_SELECTOR_HEIGHT);
	BaseSprite_setPosition((BaseSprite*)menuSelectorFrame, MENU_SELECTOR_XPOS, MENU_SELECTOR_CONTINUE_YPOS);
	menuSelectorFrame->colour = MENU_SELECTOR_COLOR;

	if(isMainMenu == 1)
	{
		menuOption1->setString(menuOption1, "Start");
		menuOption2->setString(menuOption2, "Instructions");
	}
	else
	{
		menuOption1->setString(menuOption1, "Resume");
		menuOption2->setString(menuOption2, "Quit Game");
		BaseSprite_setPosition((BaseSprite*)menuSelectorFrame, MENU_SELECTOR_XPOS, MENU_SELECTOR_NEWGAME_YPOS);
	}

	SpriteArrayList_insert(menuSprites, (BaseSprite*)menuOuterFrame, 0);
	SpriteArrayList_insert(menuSprites, (BaseSprite*)menuOption1, 1);
	SpriteArrayList_insert(menuSprites, (BaseSprite*)menuOption2, 2);
	SpriteArrayList_insert(menuSprites, (BaseSprite*)menuSelectorFrame, 3);

	if(isMainMenu == 1)
	{
		ImgSprite* menu = ImgSprite_init(ImgSprite_alloc());
		SpriteParser_parse("title", menu);
		BaseSprite_setPosition((BaseSprite*)menu, 90, 60);
		SpriteArrayList_insert(menuSprites, (BaseSprite*)menu, 4);
	}
	return menuSprites;
}

SpriteArrayList* SpriteFactory_generateLevelMenu()
{
	SpriteArrayList* levelSprites = SpriteArrayList_init(SpriteArrayList_alloc(), 10);

	ImgSprite* levelMenuTitle = ImgSprite_init(ImgSprite_alloc());
	SpriteParser_parse("level", levelMenuTitle);
	BaseSprite_setPosition((BaseSprite*)levelMenuTitle, LEVELMENU_TITLE_XPOS, LEVELMENU_TITLE_YPOS);

	ImgSprite* addSubtractTitle = ImgSprite_init(ImgSprite_alloc());
	SpriteParser_parse("addsub", addSubtractTitle);
	BaseSprite_setPosition((BaseSprite*)addSubtractTitle, 30, LEVELMENU_ADDLEVEL_TITLE_YPOS);

	AlphaSprite* addEasy = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)addEasy, LEVELMENU_A_LEVEL_XPOS, LEVELMENU_ADDEASY_YPOS);
	addEasy->setString(addEasy, "Easy");

	AlphaSprite* addMedium = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)addMedium, LEVELMENU_A_LEVEL_XPOS, LEVELMENU_ADDMEDIUM_YPOS);
	addMedium->setString(addMedium, "Medium");

	AlphaSprite* addHard = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)addHard, LEVELMENU_A_LEVEL_XPOS, LEVELMENU_ADDHARD_YPOS);
	addHard->setString(addHard, "Hard");

	// MULTIPLY
	ImgSprite* mulDivTitle = ImgSprite_init(ImgSprite_alloc());
	SpriteParser_parse("muldiv", mulDivTitle);
	BaseSprite_setPosition((BaseSprite*)mulDivTitle, 30, LEVELMENU_MULLEVEL_TITLE_YPOS);

	AlphaSprite* multiplyEasy = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)multiplyEasy, LEVELMENU_A_LEVEL_XPOS, LEVELMENU_MULEASY_YPOS);
	multiplyEasy->setString(multiplyEasy, "Easy");

	AlphaSprite* multiplyMedium = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)multiplyMedium, LEVELMENU_A_LEVEL_XPOS, LEVELMENU_MULMEDIUM_YPOS);
	multiplyMedium->setString(multiplyMedium, "Medium");

	AlphaSprite* multiplyHard = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)multiplyHard, LEVELMENU_A_LEVEL_XPOS, LEVELMENU_MULHARD_YPOS);
	addHard->setString(multiplyHard, "Hard");

	// SELECTOR
	RectSprite* levelSelectFrame = RectSprite_init(RectSprite_alloc());
	BaseSprite_setSize((BaseSprite*)levelSelectFrame, MENU_SELECTOR_WIDTH, MENU_SELECTOR_HEIGHT);
	BaseSprite_setPosition((BaseSprite*)levelSelectFrame, LEVELMENU_SELECTOR_XPOS, LEVELMENU_SELECTOR_EASY_YPOS);
	levelSelectFrame->colour = MENU_SELECTOR_COLOR;

	SpriteArrayList_insert(levelSprites, (BaseSprite*)levelMenuTitle, 0);
	SpriteArrayList_insert(levelSprites, (BaseSprite*)addSubtractTitle, 1);
	SpriteArrayList_insert(levelSprites, (BaseSprite*)addEasy, 2);
	SpriteArrayList_insert(levelSprites, (BaseSprite*)addMedium, 3);
	SpriteArrayList_insert(levelSprites, (BaseSprite*)addHard, 4);
	SpriteArrayList_insert(levelSprites, (BaseSprite*)levelSelectFrame, 5);
	SpriteArrayList_insert(levelSprites, (BaseSprite*)mulDivTitle, 6);
	SpriteArrayList_insert(levelSprites, (BaseSprite*)multiplyEasy, 7);
	SpriteArrayList_insert(levelSprites, (BaseSprite*)multiplyMedium, 8);
	SpriteArrayList_insert(levelSprites, (BaseSprite*)multiplyHard, 9);
	return levelSprites;
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

	char* currentString = (char*)malloc(sizeof(char)*3);
	currentString = "  ";
	AlphaSprite* scorebarCurrentVal = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)scorebarCurrentVal, SCOREBAR_CURRENT_VAL_XPOS, SCOREBAR_WORD_YPOS);
	scorebarCurrentVal->setString(scorebarCurrentVal, currentString);
	scorebarCurrentVal->baseSprite.spriteId = SCOREBAR_CURRENT_ID;

	AlphaSprite* scorebarTargetWord = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)scorebarTargetWord, SCOREBAR_TARGET_WORD_XPOS, SCOREBAR_WORD_YPOS);
	scorebarTargetWord->setString(scorebarTargetWord, SCOREBAR_TARGET_STRING);

	char* targetString = (char*)malloc(sizeof(char)*3);
	targetString = " ";
	AlphaSprite* scorebarTargetVal = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)scorebarTargetVal, SCOREBAR_TARGET_VAL_XPOS, SCOREBAR_WORD_YPOS);
	scorebarTargetVal->setString(scorebarTargetVal, targetString);
	scorebarTargetVal->baseSprite.spriteId = SCOREBAR_TARGET_ID;

	AlphaSprite* scorebarLevelWord = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)scorebarLevelWord, SCOREBAR_LEVEL_WORD_XPOS, SCOREBAR_WORD_YPOS);
	scorebarLevelWord->setString(scorebarLevelWord, SCOREBAR_LEVEL_STRING);

	char* levelString = (char*)malloc(sizeof(char)*3);
	levelString = "";
	AlphaSprite* scorebarLevelVal = AlphaSprite_init(AlphaSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)scorebarLevelVal, SCOREBAR_LEVEL_VAL_XPOS, SCOREBAR_WORD_YPOS);
	scorebarLevelVal->setString(scorebarLevelVal, levelString);
	scorebarLevelVal->baseSprite.spriteId = SCOREBAR_LEVEL_ID;

	RectSprite* scorebarUnderline = RectSprite_init(RectSprite_alloc());
	BaseSprite_setPosition((BaseSprite*)scorebarUnderline, 3, SCOREBAR_TOP_PADDING);
	BaseSprite_setSize((BaseSprite*)scorebarUnderline, CHAR_TO_PIXEL_WIDTH*(SCOREBAR_LEVEL_VAL_XPOS+SCOREBAR_VAL_WIDTH), CHAR_TO_PIXEL_HEIGHT*2);
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

