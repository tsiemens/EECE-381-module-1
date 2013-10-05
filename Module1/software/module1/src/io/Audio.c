/*
 * Audio.c
 *
 */

#include "Audio.h"

void av_config_setup() {
	alt_up_av_config_dev * av_config = alt_up_av_config_open_dev("/dev/audio_and_video");
	if (av_config != NULL) {
		printf("Initializing audio video device\n");
		while (!alt_up_av_config_read_ready(av_config)) {
		}
		printf("Opened audio video device\n");
	} else {
		printf("Error: could not open audio video device\n");
	}
}

Audio* audio_init() {
	av_config_setup();
	Audio* this = NULL;
	alt_up_audio_dev * device = alt_up_audio_open_dev("/dev/audio_0");
	if (device != NULL) {
		printf("Opened audio device\n");
		this = (Audio *) malloc(sizeof(Audio));
		this->audio_dev = device;
	} else {
		printf("Error: could not open audio\n");
	}
	return this;
}

void Audio_free(Audio* this) {
	free(this->audio_dev);
	free(this);
}

void audio_reset(Audio* this) {
	alt_up_audio_reset_audio_core(this->audio_dev);
}

int audio_play_channel(Audio* this, unsigned int *buf, int len, int channel) {
	return alt_up_audio_write_fifo(this->audio_dev, buf, len, channel);
}
