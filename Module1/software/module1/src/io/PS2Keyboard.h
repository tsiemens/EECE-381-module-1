/*
 * Keyboard.h
 *
 *  Created on: 2013-09-27
 *  Author: Trevor Siemens
 */

#ifndef PS2KEYBOARD_H_
#define PS2KEYBOARD_H_

#include "altera_up_avalon_ps2.h"
#include "../util/Timer.h"

#define KEY_ESC 0x1b
#define KEY_BKSP 0x8
#define KEY_SHIFT 0xF
#define KEY_CAPS 0x90
#define KEY_CTRL 0x91
#define KEY_WINDOWS 0x92
#define KEY_ALT 0x93
#define KEY_INS 0x94
#define KEY_DEL 0x95
#define KEY_HOME 0x96
#define KEY_END 0x97
#define KEY_PGUP 0x98
#define KEY_PGDN 0x99
#define KEY_UP 0x9a
#define KEY_DOWN 0x9b
#define KEY_LEFT 0x9c
#define KEY_RIGHT 0x9d

// Wrapper for the altera keyboard
typedef struct PS2Keyboard
{
	// the keyboard
	alt_up_ps2_dev* keyboard;
} PS2Keyboard;

PS2Keyboard* PS2Keyboard_alloc_init();

void PS2Keyboard_free(PS2Keyboard* this);

int PS2Keyboard_readKey(PS2Keyboard* this, alt_u8* byte);

#endif /* PS2KEYBOARD_H_ */
