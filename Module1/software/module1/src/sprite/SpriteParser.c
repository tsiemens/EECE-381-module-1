#include "SpriteParser.h"

// Helper Function Declarations:
int* convertToInt(unsigned char buffer[], int start);
int* convertToPixel(unsigned char char_h, unsigned char char_l);
void getColourArray(unsigned char buffer[], int size, int pixelArray[]);
int* getColourMatrix(const char* filePath);
void convertToMatrix(int pixelArray[], int width, int height, int** matrix);


/*
 * Takes in the file path to the .raw16 file and ImgSprite and parses .raw16 details into the ImgSprite
 */
void SpriteParser_parse(const char* filePath, ImgSprite* sprite)
{
//	FILE* fp = NULL;
//	if( (fp = (fopen(filePath, "rb"))) == NULL)
//	{
//		printf("Couldn't open file\n");
//		return NULL;
//	}
//	// GET SIZE:
//		unsigned char buffer[8];
//		int bytesread = fread(buffer, sizeof(char), sizeof(buffer), fp);
//		sprite->baseSprite.width = (*convertToInt(buffer, 0));
//		sprite->baseSprite.height = (*convertToInt(buffer, 4));
//
//	// GET PIXELS:
//		unsigned char pixelBuffer[2*width*height]; // buffer with number of pixel bytes to read
//		int pixelBytesRead = fread(pixelBuffer, sizeof(char), sizeof(pixelBuffer), fp);
//		int pixelArray[pixelBytesRead];
//		getColourArray(pixelBuffer, pixelBytesRead, pixelArray);
//		fclose(fp);



		// demo data:
		unsigned char byteArray[40];
		int i;
		for(i=0; i < 40; i++)
		{
			byteArray[i] = 0xff;
			printf("b: %x ", byteArray[i]);
		}

		sprite->baseSprite.height = 4;
		sprite->baseSprite.width = 5;
		int pixelArray[40];
		getColourArray(byteArray, 40, pixelArray);
		// end demo data
		convertToMatrix(pixelArray, sprite->baseSprite.width, sprite->baseSprite.height, sprite->matrix);
}

/************************************************************************************************************************/

/*
 * Takes in an array and populates it with the buffer's pixel colour data
 * @param buffer[] = takes in the size buffer of bytes to read from
 * @param pixelArray[] = is populated with pixel information
 */
void getColourArray(unsigned char buffer[], int size, int pixelArray[])
{
	int i;
	for(i=0; i < size-1; i=i+2)
	{
		pixelArray[i/2] = (*convertToPixel(buffer[1+i], buffer[i]));
	}
}

/*
 * Converts a 1d array into a 2d array/matrix
 * @param pixelArray[] = the 1d array
 * @param int** matrix = the 2d array/matrix
 */
void convertToMatrix (int pixelArray[], int width, int height, int** matrix)
{
	int i, j;
	for(i=0; i < height; i++)
	{
		for(j=0; j < width; j++)
		{
			matrix[i][j] = pixelArray[(j*height) + i];
		}
	}
}

/*
 * Converts 4 bytes into a 32bit integer -- assumes little endian
 * @param buffer[] = takes in the size buffer of bytes to read from
 * @param start = indicates which index to start converting (if width, 0; if height, 4)
 */
int* convertToInt(unsigned char buffer[], int start)
{
	// assuming little endian
	unsigned char val[] = {buffer[3+start], buffer[2+start], buffer[1+start], buffer[start]};
	return (int*)val;
}

/*
 * Takes 2 bytes and returns a 16bit colour value (first 2 bytes of the 32bit int are 0's)
 * @param char_h = the high byte
 * @param char_l = the low byte
 */
int* convertToPixel(unsigned char char_h, unsigned char char_l)
{
	unsigned char val[] = {char_h, char_l, 0, 0};
	return (int*)val;
}

