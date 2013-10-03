/*
 * PS2Keyboard.c
 *
 *  Created on: 2013-09-27
 *  Author: Trevor Siemens
 *
 *  An alternative driver set, to avoid
 *  some malfunction in the Altera PS/2 drivers,
 *  which is preventing us from using a perfectly good keyboard.
 */

#include <stdlib.h>
#include <stdio.h>
#include "io.h"
#include "PS2Keyboard.h"

static alt_u8 keyMap[0x80] = {
		0,/* 00 */   0,/* 01 */   0,/* 02 */   0,/* 03 */   0,/* 04 */   0,/* 05 */   0,/* 06 */   0,/* 07 */
		0,/* 08 */   0,/* 09 */   0,/* 0a */   0,/* 0b */   0,/* 0c */   '\t',/* 0d */ '`',/* 0e */   0,/* 0f */
		0,/* 10 */   KEY_ALT,/* 11 */  KEY_SHIFT,/* 12 */   0,/* 13 */   KEY_CTRL,/* 14 */ 'q',/* 15 */ '1',/* 16 */   0,/* 17 */
		0,/* 18 */   0,/* 19 */ 'z',/* 1a */ 's',/* 1b */ 'a',/* 1c */ 'w',/* 1d */ '2',/* 1e */   KEY_WINDOWS,/* 1f */
		0,/* 20 */ 'c',/* 21 */ 'x',/* 22 */ 'd',/* 23 */ 'e',/* 24 */ '4',/* 25 */ '3',/* 26 */   KEY_WINDOWS,/* 27 */
		0,/* 28 */ ' ',/* 29 */ 'v',/* 2a */ 'f',/* 2b */ 't',/* 2c */ 'r',/* 2d */ '5',/* 2e */   0,/* 2f */
		0,/* 30 */ 'n',/* 31 */ 'b',/* 32 */ 'h',/* 33 */ 'g',/* 34 */ 'y',/* 35 */ '6',/* 36 */   0,/* 37 */
		0,/* 38 */   0,/* 39 */ 'm',/* 3a */ 'j',/* 3b */ 'u',/* 3c */ '7',/* 3d */ '8',/* 3e */   0,/* 3f */
		0,/* 40 */ ',',/* 41 */ 'k',/* 42 */ 'i',/* 43 */ 'o',/* 44 */ '0',/* 45 */ '9',/* 46 */   0,/* 47 */
		0,/* 48 */ '.',/* 49 */ '/',/* 4a */ 'l',/* 4b */ ';',/* 4c */ 'p',/* 4d */ '-',/* 4e */   0,/* 4f */
		0,/* 50 */   0,/* 51 */'\'',/* 52 */   0,/* 53 */ '[',/* 54 */ '=',/* 55 */   0,/* 56 */   0,/* 57 */
		KEY_CAPS,/* 58 */   KEY_SHIFT,/* 59 */'\n',/* 5a */ ']',/* 5b */   0,/* 5c */'\\',/* 5d */   0,/* 5e */   0,/* 5f */
		0,/* 60 */   0,/* 61 */   0,/* 62 */   0,/* 63 */   0,/* 64 */   0,/* 65 */KEY_BKSP,/* 66 */   0,/* 67 */
		0,/* 68 */   KEY_END,/* 69 */   0,/* 6a */   KEY_LEFT,/* 6b */   KEY_HOME,/* 6c */   0,/* 6d */   0,/* 6e */   0,/* 6f */
		KEY_INS,/* 70 */   KEY_DEL,/* 71 */   KEY_DOWN,/* 72 */   0,/* 73 */   KEY_RIGHT,/* 74 */   KEY_UP,/* 75 */KEY_ESC,/* 76 */   0,/* 77 */
		0,/* 78 */   0,/* 79 */   KEY_PGDN,/* 7a */   0,/* 7b */   0,/* 7c */   KEY_PGUP,/* 7d */   0,/* 7e */ 0/* 7f */
};

/*
 * Reads a byte from the keyboard buffer,
 * and decodes it into alt_u8 (unsigned char)
 *
 * @param keyboard: the device which should be a ps2 keyboard
 * @param byte: pointer to byte in which the key will be stored
 *
 * @return 0 = down key press, 1 = key release, -1 = error/empty buffer
 */
int PS2Keyboard_readKey(PS2Keyboard* this, alt_u8* byte)
{
	alt_up_ps2_dev* keyboard = this->keyboard;
	alt_u8 tempByte;
	int status = alt_up_ps2_read_data_byte(keyboard, &tempByte);

	if (status == 0)
	{
		if (tempByte == 0xAA)
		{
			// AA means passed, should be ignored
			return PS2Keyboard_readKey(this, byte);
		} else if (tempByte == 0xF0)
		{
			// F0 signifies an up key press, with next byte being that key
			if (PS2Keyboard_readKey(this, byte) == 0)
			{
				return 1;
			}
		} else
		{
			if (tempByte < 0x80)
			{
				*byte = keyMap[tempByte];
				return 0;
			}
		}
	}

	return -1;
}

/**
 * Returns a new ps2 keyboard, which has been initialised
 */
PS2Keyboard* PS2Keyboard_new()
{
	PS2Keyboard* this = NULL;
	alt_up_ps2_dev* device = alt_up_ps2_open_dev("/dev/ps2_0");
	alt_up_ps2_dev* ps2 = device;

	if (device != NULL) {
		printf ("Opened PS/2 device\n");

		// initialize the device
		alt_u8 byte;
		//send the reset request, wait for ACK
		alt_up_ps2_write_data_byte_with_ack(ps2, 0xff);
		// now try to get the BAT result, AA means passed
		int status = alt_up_ps2_read_data_byte(ps2, &byte);
		if (status == 0 && byte == 0xAA)
		{
			//get the 2nd byte
			status = alt_up_ps2_read_data_byte(ps2, &byte);
			if (status == -ETIMEDOUT)
			{
				//for keyboard, only 2 bytes are sent(ACK, PASS/FAIL), so timeout
				ps2->device_type = PS2_KEYBOARD;
			}
		}

		this = (PS2Keyboard *)malloc(sizeof(PS2Keyboard));
		this->keyboard = device;
	} else {
		printf ("Error: could not open PS/2 device\n");
	}

	return this;
}

void PS2Keyboard_free(PS2Keyboard* this)
{
	free(this->keyboard);
	free(this);
}
