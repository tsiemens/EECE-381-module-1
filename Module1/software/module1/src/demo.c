/*
 * demo.c
 *
 */

#include "demo.h"

void demo_sd_image() {
	UnsignedCharPtr* image_data = sdcard_read_file_w_size("play");
	printf("SD Card: Read play %d bytes\n", image_data->size);
}

void demo_sd_audio() {

	// test sd card
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
	int buffer_size = 128;
	unsigned int audio_output[buffer_size];
	unsigned int audio_output_2[buffer_size];

	int i;
	for (i = 0; i < buffer_size; i++) {
		if (i % 2 == 0) {
			audio_output_2[i] = 0x7FFF;
		} else {
			audio_output_2[i] = 0x8000;
		}
	}

	while (end < test2->size) {
		begin = end;
		if (test2->size < buffer_size) {
			end = test2->size;
		} else if (((test2->size) - end) < buffer_size) {
			end = test2->size;
		} else {
			end += buffer_size;
		}
		ShortToIntCPY(&(test2->data[begin]), audio_output, end - begin);
		audio_play(audioDev, audio_output_2, end - begin);
		int k = 0;
		for (k = 0; k < 12800; k++) {
		}
	}

	ShortIntPtr_free(test2);
	test2 = NULL;
}

void demo_sd_audio_filename(Audio* audioDev, char* filename) {

	// test sd card
	ShortIntPtr* test2 = sdcard_read_audio(filename);
	printf("SD Card: Read %s %d audio data\n", filename, test2->size);

	// test audio
	int begin = 0;
	int end = 0;
	int buffer_size = 128;
	unsigned int audio_output[buffer_size];

	while (end < test2->size) {
		begin = end;
		if (test2->size < buffer_size) {
			end = test2->size;
		} else if (((test2->size) - end) < buffer_size) {
			end = test2->size;
		} else {
			end += buffer_size;
		}
		ShortToIntCPY(&(test2->data[begin]), audio_output, end - begin);
		audio_play(audioDev, audio_output, end - begin);
		while(audio_check_buffer(audioDev) < 128) {
			int t = 0;
			for (t = 0; t < 200; t++) {}
			for (t = 0; t < 200; t++) {}
		}
	}

	ShortIntPtr_free(test2);
	test2 = NULL;
}

