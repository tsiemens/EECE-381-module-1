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
#define CHAR_TO_PIXEL_HEIGHT ((float)SCREEN_HEIGHT/CHARBUFFER_HEIGHT)
#define PIXEL_TO_CHAR_HEIGHT ((float)CHARBUFFER_HEIGHT/SCREEN_HEIGHT)
#define CHAR_TO_PIXEL_WIDTH ((float)SCREEN_WIDTH/CHARBUFFER_WIDTH)
#define PIXEL_TO_CHAR_WIDTH ((float)CHARBUFFER_WIDTH/SCREEN_WIDTH)

#define CHAR_PADDING_Y 6
#define CHAR_PADDING_X 4
#define CHAR_NEWLINE 3
#define MENUITEM_START_XPOS (CHARBUFFER_WIDTH/2 - CHAR_PADDING_X)
#define MENUITEM_START_YPOS (PIXEL_TO_CHAR_HEIGHT*MENUFRAME_YPOS + CHAR_PADDING_Y)
#define MENUITEM_CONTINUE_YPOS (MENUITEM_START_YPOS + CHAR_NEWLINE)

#define MENU_SELECTOR_PADDING 3
#define MENU_SELECTOR_XPOS MENUITEM_START_XPOS*(CHAR_TO_PIXEL_WIDTH) - MENU_SELECTOR_PADDING
#define MENU_SELECTOR_NEWGAME_YPOS (MENUITEM_START_YPOS*(CHAR_TO_PIXEL_HEIGHT) - MENU_SELECTOR_PADDING)
#define	MENU_SELECTOR_CONTINUE_YPOS (MENU_SELECTOR_NEWGAME_YPOS + CHAR_TO_PIXEL_HEIGHT*CHAR_NEWLINE)
#define MENU_SELECTOR_WIDTH 38
#define MENU_SELECTOR_HEIGHT 8
#define MENU_SELECTOR_COLOR 0xFFFF

#define INSTRUCTIONITEM_TITLE_XPOS (MENUITEM_START_XPOS-1)
#define INSTRUCTIONITEM_TITLE_YPOS (MENUITEM_CONTINUE_YPOS-CHAR_TO_PIXEL_HEIGHT*7)

#define INSTRUCTIONITEM_TEXT_XPOS (INSTRUCTIONITEM_TITLE_XPOS-20)
#define INSTRUCTIONITEM_ESC_XPOS (INSTRUCTIONITEM_TITLE_XPOS-10)
typedef int bool;
#define true 1
#define false 0
