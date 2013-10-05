/*
 * SDCard.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "SDCard.h"

SDCard * sdcard_init() {
	SDCard* this = NULL;
	alt_up_sd_card_dev * device = NULL;
	device = alt_up_sd_card_open_dev("/dev/sd_card");
	if (device != NULL) {
		if (alt_up_sd_card_is_Present()) {
			printf("SD Card: connected.\n");
			if (alt_up_sd_card_is_FAT16()) {
				printf("SD Card: FAT16 file system detected.\n");
				this = (SDCard *) malloc(sizeof(SDCard));
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

void sdcard_free(SDCard* this) {
	free(this->sdcard_dev);
	free(this);
}

ShortIntPtr* sdcard_read_file(char* filename) {
	short int file_id;
	int count = 0;
	short int* data_temp;

	ShortIntPtr* output;
	output = (ShortIntPtr *) malloc(sizeof(ShortIntPtr));
	if (output == NULL) {
		printf("Error: ShortIntPtr Malloc\n");
		return output;
	}
	output->data = malloc(ALLOC_SIZE * sizeof(short int));
	if (output->data == NULL) {
		printf("Error: ShortIntPtr data Malloc\n");
		return output;
	}

	file_id = alt_up_sd_card_fopen(filename, 0);
	short temp = alt_up_sd_card_read(file_id);

	while (temp != -1) {
		output->data[count] = temp;
		count++;
		if ((count % ALLOC_SIZE) == 0) {
			data_temp = realloc(output->data, sizeof(output->data) + ALLOC_SIZE
					* sizeof(short int));
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

ShortIntPtr* sdcard_read_audio(char* filename) {
	short int file_id;
	int count = 0;
	short temp;
	short int* data_temp;

	ShortIntPtr* output;
	output = (ShortIntPtr *) malloc(sizeof(ShortIntPtr));
	if (output == NULL) {
		printf("Error: ShortIntPtr Malloc\n");
		return output;
	}
	output->size = 0;
	output->data = malloc(AUDIO_ALLOC_SIZE * sizeof(short int));
	if (output->data == NULL) {
		printf("Error: ShortIntPtr data Malloc\n");
		return output;
	}

	file_id = alt_up_sd_card_fopen(filename, 0);

	char byte_before = alt_up_sd_card_read(file_id);
	char byte_after = alt_up_sd_card_read(file_id);

	temp = ((unsigned char) byte_before << 8) | (unsigned char) byte_after;

	while (temp != -1) {
		output->data[count] = temp;
		count++;
		if ((count % AUDIO_ALLOC_SIZE) == 0) {
			data_temp = realloc(output->data, sizeof(output->data)
					+ AUDIO_ALLOC_SIZE * sizeof(short int));
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
