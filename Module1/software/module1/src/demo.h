/*
 * demo.h
 */

#ifndef DEMO_H_
#define DEMO_H_

#include <stdio.h>
#include "io/SDCard.h"
#include "io/Audio.h"
#include "util/ArrayPtr.h"

void demo_sd_audio();
void demo_sd_audio_filename(Audio* audioDev, char* filename);
void demo_sd_image();

#endif
