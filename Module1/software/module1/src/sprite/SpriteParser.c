#include "SpriteParser.h"
#include "../io/SDCard.h"
#include "../util/ArrayPtr.h"
#include <stdio.h>

// Declarations of Helper Functions:
int convertToInt(unsigned char buffer[], int start);
int convertToPixel(unsigned char char_h, unsigned char char_l);
void getColourArray(unsigned char buffer[], int size, int pixelArray[]);
int* getColourMatrix(const char* filePath);
void convertToMatrix(int pixelArray[], int width, int height, int** matrix);

/*
 * 	Parses image at file path into the ImgSprite
 * 	@param filePath = the file path of the raw16 file
 * 	@param sprite = the ImgSprite to parse the data into
 */
void SpriteParser_parse(const char* filePath, ImgSprite* sprite) {

	//	UNTESTED CODE TO BE TESTED WHEN MERGED AND CAN WORK WITH SD CARD STUFF
	//	GET FILE DATA:
	SDCard* sd_card = sdcard_init();
	UnsignedCharPtr* img_data = sdcard_read_file(filePath);
	int bytesread = img_data->size;
	printf("SD Card: Read %s %d bytes\n", filePath, img_data->size);
	int width = convertToInt(img_data->data, 0);
	int height = convertToInt(img_data->data, 4);
	printf("size: %d x %d\n\n", width, height);

	//	GET/SET SPRITE DIMENSIONS:
	BaseSprite_setSize((BaseSprite*)sprite, convertToInt(img_data->data, 0), convertToInt(img_data->data, 4));

	//	GET PIXELS:
	int pixelArray[(bytesread - 8)];
	getColourArray(img_data->data, bytesread, pixelArray);
	convertToMatrix(pixelArray, width, height, sprite->matrix);

}
/* ------------------------------ PRIVATE HELPER METHODS ------------------------------ */

/*
 * 	Takes in an array and populates it with the buffer's pixel colour data
 * 	@param buffer[] = takes in the size buffer of bytes to read from
 * 	@param size = size of buffer
 * 	@param pixelArray[] = is populated with pixel information
 */
void getColourArray(unsigned char buffer[], int size, int pixelArray[]) {
	int i;
	for (i = 8; i < (size - 1); i = i + 2) {
		pixelArray[i / 2 - 4] = convertToPixel(buffer[1 + i], buffer[i]);
	}
}

/*
 * 	Converts a 1d array into a 2d array/matrix
 * 	@param pixelArray[] = the 1d array
 * 	@param int** matrix = the 2d array/matrix
 */
void convertToMatrix(int pixelArray[], int width, int height, int** matrix) {
	printf("\n\nMATRIX: %d by %d\n", width, height);
	int i, j;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			matrix[i][j] = pixelArray[(j * height) + i];
			printf("%04x ", matrix[i][j] );
		}
		printf("\n");
	}
}

/*
 * 	Converts 4 bytes into a 32bit integer -- assumes little endian
 * 	@param buffer[] = takes in the size buffer of bytes to read from
 * 	@param start = indicates which index to start converting (if width, 0; if height, 4)
 */
int convertToInt(unsigned char buffer[], int start) {
	// assuming little endian
	unsigned char val[] = { buffer[3 + start], buffer[2 + start], buffer[1 + start], buffer[start] };
	int intVal = *((int*)val);
	return intVal;
}

/*
 * 	Takes 2 bytes and returns a 16bit colour value (first 2 bytes of the 32bit int are 0's)
 * 	@param char_h = the high byte
 * 	@param char_l = the low byte
 */
int convertToPixel(unsigned char char_h, unsigned char char_l) {
	unsigned char val[] = { char_h, char_l, 0, 0 };
	int intVal = *((int*)val);
	return intVal;
}
