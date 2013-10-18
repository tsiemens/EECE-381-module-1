#ifndef SPRITEFACTORY_H_
#define SPRITEFACTORY_H_

#include "../sprite/ImgSprite.h"
#include "../sprite/RectSprite.h"
#include "../sprite/AlphaSprite.h"
#include "../sprite/SpriteParser.h"
#include "../sprite/BaseSprite.h"
#include "../sprite/SpriteArrayList.h"

ImgSprite* SpriteFactory_generatePlayerSprite();
ImgSprite* SpriteFactory_generateEnemySprite();
RectSprite* SpriteFactory_generateLaserSprite(BaseSprite* player, int colour);
SpriteArrayList* SpriteFactory_generateInstructions();
SpriteArrayList* SpriteFactory_generateScoreBar();
SpriteArrayList* SpriteFactory_generateMenu(int isMainMenu);
SpriteArrayList* SpriteFactory_generateLevelMenu();
char intToChar(int val);

#define PLAYER_SPRITE_ID 1
#define PLAYER_LASER_SPRITE_ID 2
#define ENEMY_SPRITE_ID_BASE 10

/****************ENEMY GENERATION****************/
#define ENEMY_COL_WIDTH 32
#define ENEMY_COLS 10
#define ENEMY_SPEED 0.05

/****************CHAR AND PIXEL CONVERSIONS****************/
#define CHAR_TO_PIXEL_HEIGHT ((float)SCREEN_HEIGHT/CHARBUFFER_HEIGHT)
#define PIXEL_TO_CHAR_HEIGHT ((float)CHARBUFFER_HEIGHT/SCREEN_HEIGHT)
#define CHAR_TO_PIXEL_WIDTH ((float)SCREEN_WIDTH/CHARBUFFER_WIDTH)
#define PIXEL_TO_CHAR_WIDTH ((float)CHARBUFFER_WIDTH/SCREEN_WIDTH)

/****************MAIN MENU DEFINITIONS****************/
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define MENUFRAME_WIDTH SCREEN_WIDTH/3
#define MENUFRAME_HEIGHT SCREEN_HEIGHT/4
#define MENUFRAME_BOT_PADDING 20
#define MENUFRAME_YPOS (SCREEN_HEIGHT - MENUFRAME_HEIGHT - MENUFRAME_BOT_PADDING)
#define MENUFRAME_XPOS (SCREEN_WIDTH/2 - MENUFRAME_WIDTH/2)
#define MENUFRAME_COLOR 0x4A4

#define CHARBUFFER_WIDTH 80
#define CHARBUFFER_HEIGHT 60

#define CHAR_PADDING_Y 6
#define CHAR_PADDING_X 4
#define CHAR_NEWLINE 3
#define MENUITEM_START_XPOS (CHARBUFFER_WIDTH/2 - CHAR_PADDING_X - 3)
#define MENUITEM_START_YPOS (PIXEL_TO_CHAR_HEIGHT*MENUFRAME_YPOS + CHAR_PADDING_Y)
#define MENUITEM_CONTINUE_YPOS (MENUITEM_START_YPOS + CHAR_NEWLINE)

#define MENU_SELECTOR_PADDING 3
#define MENU_SELECTOR_XPOS MENUITEM_START_XPOS*(CHAR_TO_PIXEL_WIDTH) - MENU_SELECTOR_PADDING
#define MENU_SELECTOR_NEWGAME_YPOS (MENUITEM_START_YPOS*(CHAR_TO_PIXEL_HEIGHT) - MENU_SELECTOR_PADDING)
#define	MENU_SELECTOR_CONTINUE_YPOS (MENU_SELECTOR_NEWGAME_YPOS + CHAR_TO_PIXEL_HEIGHT*CHAR_NEWLINE)
#define MENU_SELECTOR_WIDTH 55
#define MENU_SELECTOR_HEIGHT 8
#define MENU_SELECTOR_COLOR 0xFFFF

typedef enum {
	SEL_NEWGAME,
	SEL_INSTRUCTIONS,
	SEL_RESUME,
	SEL_QUIT
} MenuSelection;

/****************INSTRUCTIONS DEFINITIONS****************/

#define INSTRUCTIONITEM_TITLE_XPOS (MENUITEM_START_XPOS+2)
#define INSTRUCTIONITEM_TITLE_YPOS (MENUITEM_CONTINUE_YPOS-CHAR_TO_PIXEL_HEIGHT*7)
#define INSTRUCTIONITEM_TEXT_XPOS (INSTRUCTIONITEM_TITLE_XPOS-20)
#define INSTRUCTIONITEM_ESC_XPOS (INSTRUCTIONITEM_TITLE_XPOS-10)

#define INSTRUCTIONITEM_P1_L1 "You are on a journey to Planet Math. Your goal is to"
#define INSTRUCTIONITEM_P1_L2 "eat an exact amount of space burgers. Too few and"
#define INSTRUCTIONITEM_P1_L3 "you won't make it. Too many, you will fall asleep."
#define INSTRUCTIONITEM_P1_L4 "Grab the burger with desired number & use different"
#define INSTRUCTIONITEM_P1_L5 "lasers to use different operators. Good luck!"

typedef enum {
	INS_FIRSTPAGE,
	INS_SECONDPAGE
} InstuctionPage;

/****************SCOREBAR DEFINITIONS****************/
#define SCOREBAR_TOP_PADDING CHAR_TO_PIXEL_HEIGHT + 2
#define SCOREBAR_SPACING 2
#define SCOREBAR_WORD_WIDTH 8
#define SCOREBAR_VAL_WIDTH 3
#define SCOREBAR_COLOR 0x1234

#define SCOREBAR_WORD_YPOS 2
#define SCOREBAR_CURRENT_WORD_XPOS SCOREBAR_SPACING
#define SCOREBAR_CURRENT_VAL_XPOS (SCOREBAR_CURRENT_WORD_XPOS + SCOREBAR_SPACING + SCOREBAR_WORD_WIDTH)
#define SCOREBAR_TARGET_WORD_XPOS (SCOREBAR_CURRENT_VAL_XPOS + SCOREBAR_SPACING + SCOREBAR_VAL_WIDTH)
#define SCOREBAR_TARGET_VAL_XPOS (SCOREBAR_TARGET_WORD_XPOS + SCOREBAR_SPACING + SCOREBAR_WORD_WIDTH)
#define SCOREBAR_LEVEL_WORD_XPOS (SCOREBAR_TARGET_VAL_XPOS + SCOREBAR_SPACING + SCOREBAR_VAL_WIDTH)
#define SCOREBAR_LEVEL_VAL_XPOS (SCOREBAR_LEVEL_WORD_XPOS + SCOREBAR_SPACING + SCOREBAR_WORD_WIDTH)

#define SCOREBAR_CURRENT_STRING "Current:"
#define SCOREBAR_CURRENT_VAL "0"
#define SCOREBAR_TARGET_STRING "Target:"
#define SCOREBAR_TARGET_VAL "0"
#define SCOREBAR_LEVEL_STRING "Level:"
#define SCOREBAR_LEVEL_VAL "0"

typedef enum {
	SCOREBAR_
};
/****************LEVEL MENU DEFINITIONS****************/
#define LEVELMENU_TITLE_XPOS 			20
#define LEVELMENU_TITLE_YPOS			CHAR_TO_PIXEL_HEIGHT
#define LEVELMENU_A_LEVEL_XPOS			INSTRUCTIONITEM_TEXT_XPOS

#define LEVELMENU_ADDEASY_YPOS			(CHAR_TO_PIXEL_HEIGHT*3)
#define LEVELMENU_ADDMEDIUM_YPOS		(CHAR_TO_PIXEL_HEIGHT*4)
#define LEVELMENU_ADDHARD_YPOS			(CHAR_TO_PIXEL_HEIGHT*5)

#define LEVELMENU_MULEASY_YPOS			(CHAR_TO_PIXEL_HEIGHT*8)
#define LEVELMENU_MULMEDIUM_YPOS		(CHAR_TO_PIXEL_HEIGHT*9)
#define LEVELMENU_MULHARD_YPOS			(CHAR_TO_PIXEL_HEIGHT*10)

#define LEVELMENU_ADDLEVEL_TITLE_YPOS	CHAR_TO_PIXEL_HEIGHT*2*CHAR_NEWLINE
#define LEVELMENU_MULLEVEL_TITLE_YPOS	LEVELMENU_ADDHARD_YPOS*CHAR_TO_PIXEL_HEIGHT + CHAR_NEWLINE + 15

#define LEVELMENU_SELECTOR_XPOS 		INSTRUCTIONITEM_TEXT_XPOS*CHAR_TO_PIXEL_WIDTH - MENU_SELECTOR_PADDING
#define LEVELMENU_SELECTOR_EASY_YPOS 	LEVELMENU_ADDEASY_YPOS*CHAR_TO_PIXEL_HEIGHT - MENU_SELECTOR_PADDING
#define	LEVELMENU_SELECTOR_MEDIUM_YPOS 	LEVELMENU_ADDMEDIUM_YPOS*CHAR_TO_PIXEL_HEIGHT - MENU_SELECTOR_PADDING
#define LEVELMENU_SELECTOR_HARD_YPOS 	LEVELMENU_ADDHARD_YPOS*CHAR_TO_PIXEL_HEIGHT - MENU_SELECTOR_PADDING

#define LEVELMENU_SELECTOR_MUL_EASY_YPOS 	LEVELMENU_MULEASY_YPOS*CHAR_TO_PIXEL_HEIGHT - MENU_SELECTOR_PADDING
#define	LEVELMENU_SELECTOR_MUL_MEDIUM_YPOS 	LEVELMENU_MULMEDIUM_YPOS*CHAR_TO_PIXEL_HEIGHT - MENU_SELECTOR_PADDING
#define LEVELMENU_SELECTOR_MUL_HARD_YPOS 	LEVELMENU_MULHARD_YPOS*CHAR_TO_PIXEL_HEIGHT - MENU_SELECTOR_PADDING
#endif /* SPRITEFACTORY_H_ */
