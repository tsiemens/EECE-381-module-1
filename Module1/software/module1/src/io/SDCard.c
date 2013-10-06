/*
 * SDCard.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "SDCard.h"

/*
 * Initialises the SD Card reader
 *
 * @return SDCard struct
 */
SDCard * sdcard_init() {
	SDCard* this = (SDCard *) malloc(sizeof(SDCard));
	alt_up_sd_card_dev * device = alt_up_sd_card_open_dev("/dev/sd_card");
	if (device != NULL) {
		if (alt_up_sd_card_is_Present()) {
			printf("SD Card: connected.\n");
			if (alt_up_sd_card_is_FAT16()) {
				printf("SD Card: FAT16 file system detected.\n");
				this->sdcard_dev = device;
			} else {
				printf("Error: Unknown file system.\n");
			}
		}
	} else {
		printf("Error: cannot initiate sd card.\n");
	}
	return this;
}

/*
 * free the memory allocated by SDCard struct
 *
 * @param this: the SDCard struct
 */
void sdcard_free(SDCard* this) {
	free(this->sdcard_dev);
	free(this);
	return;
}

/*
 * Reads the file in the SDCard, parse the data into 2 bytes
 *
 * @param filename: the name of the file to read on the SD Card
 *
 * @return UnsignedCharPtr: contains the data number of data
 * 							read from the file
 */
UnsignedCharPtr* sdcard_read_file(char* filename) {
	short int file_id;
	int count = 0;
	short int* data_temp;

	UnsignedCharPtr* output;
	output = UnsignedCharPtr_alloc(ALLOC_SIZE);

	file_id = alt_up_sd_card_fopen(filename, 0);
	short temp = alt_up_sd_card_read(file_id);

	while (temp != -1) {
		output->data[count] = temp;
		count++;
		if ((count % ALLOC_SIZE) == 0) {
			data_temp = realloc(output->data, (count + ALLOC_SIZE)
					* sizeof(unsigned char));
			if (data_temp == NULL) {
				printf("Error: SD Card Memory Realloc\n");
				return output;
			}
			output->data = data_temp;
		}
		temp = alt_up_sd_card_read(file_id);
	}
	alt_up_sd_card_fclose(file_id);
	output->size = count;
	return output;
}

/*
 * Reads the file from SD Card, parse the data into 4 bytes
 *
 * @param filename: the name of the file to read from
 *
 * @return ShortIntPtr: the data in 4 bytes and the number of data
 * 						read from file.
 */
ShortIntPtr* sdcard_read_audio(char* filename) {
	short int file_id;
	int count = 0;
	short temp;
	short int* data_temp;

	ShortIntPtr* output = ShortIntPtr_alloc(AUDIO_ALLOC_SIZE);
	output->size = 0;

	file_id = alt_up_sd_card_fopen(filename, 0);

	char byte_before = alt_up_sd_card_read(file_id);
	char byte_after = alt_up_sd_card_read(file_id);

	temp = ((unsigned char) byte_before << 8) | (unsigned char) byte_after;

	while (temp != -1) {
		output->data[count] = temp;
		count++;
		if ((count % AUDIO_ALLOC_SIZE) == 0) {
			data_temp = realloc(output->data, (count + AUDIO_ALLOC_SIZE)
					* sizeof(short int));
			if (data_temp == NULL) {
				printf("Error: SD Card Memory Re-alloc\n");
				return output;
			}
			output->data = data_temp;
		}
		byte_before = alt_up_sd_card_read(file_id);
		byte_after = alt_up_sd_card_read(file_id);
		temp = ((unsigned char) byte_before << 8) | (unsigned char) byte_after;
	}
	alt_up_sd_card_fclose(file_id);
	output->size = count;
	return output;
}
