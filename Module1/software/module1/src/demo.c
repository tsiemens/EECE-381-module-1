/*
 * demo.c
 *
 */

#include "demo.h"
#include <stdio.h>
#include "io/SDCard.h"
#include "io/Audio.h"
#include "util/ArrayPtr.h"

int demo_sd_audio() {

	// test sd card
	SDCard* sd_card = sdcard_init();

	UnsignedCharPtr* audio_data = sdcard_read_file("b2.wav");
	printf("SD Card: Read b2.wav %d bytes\n", audio_data->size);

	UnsignedCharPtr_free(audio_data);
	audio_data = NULL;

	ShortIntPtr* test2 = sdcard_read_audio("blop.wav");
	printf("SD Card: Read blop.wav %d audio data\n", test2->size);

	// test audio
	Audio* audioDev = audio_init();
	int begin = 0;
	int end = 0;
	unsigned int audio_output[128];
	while (end < test2->size) {
		begin = end;
		if (test2->size < 128) {
			end = test2->size;
		} else if (((test2->size) - end) < 128) {
			end = test2->size;
		} else {
			end += 128;
		}
		ShortToIntCPY(&(test2->data[begin]), audio_output, end - begin);
		audio_play(audioDev, audio_output, end - begin);
		int k = 0;
		for (k = 0; k < 12800; k++) {
		}
	}

	ShortIntPtr_free(test2);
	test2 = NULL;

	sdcard_free(sd_card);
	sd_card = NULL;

	return 0;
}
