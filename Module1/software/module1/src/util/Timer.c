/*
 * Timer.c
 *
 *  Created on: 2013-10-02
 *  Author: Trevor Siemens
 */

#include "Timer.h"

#include <stdlib.h>
#include <stdio.h>
#include "sys/alt_timestamp.h"

static int s_timestampTimerStatus = -1;
static double s_timestampTimerFreq = 0;

/**
 * Initialises the system timestamp timer, and
 * sets the static Timer vars.
 */
int Timer_initTimestampTimer()
{
	/*
	 * NOTE: If this receives an error where is is not recognized,
	 * the issue is that you must manually go into the bsp editor
	 * and set sys_clk_timer to use none, and timestamp_timer to timer_0.
	 * The bsp must then be re-generated.
	 */

	int tsStatus = alt_timestamp_start();
	if (tsStatus != 0) {
		printf("Timestamp timer failed with status %d.\n", tsStatus);
	} else {
		printf("Timestamp timer initialised.\n");
		s_timestampTimerFreq = (double)alt_timestamp_freq();
		printf("Timer freq: %0.1f Hz\n", s_timestampTimerFreq);
	}

	s_timestampTimerStatus = tsStatus;
	return tsStatus;
}

/**
 * Initialises a new timer
 *
 * @param waitTimeMs : The time the timer should run for, in miliseconds
 */
Timer* Timer_new(double waitTimeMs)
{
	Timer *this = (Timer *)malloc(sizeof(Timer));
	this->startTime = 0;
	this->waitTimeMs = waitTimeMs;

	// If the global timer has not been initialised, do so.
	if(s_timestampTimerStatus != 0) {
			if (Timer_initTimestampTimer() != 0) {
				return NULL;
			}
	}

	return this;
}

/**
 * Starts the timer
 *
 * @param this
 */
void Timer_start(Timer* this)
{
	this->startTime = alt_timestamp();
}

/**
 * Returns if the timer is done
 *
 * @param this
 *
 * @return 1 if the timer has finished (or was never started), 0 otherwise.
 */
int Timer_isDone(Timer* this)
{
	if (this->startTime == 0) {
		return 1;
	}

	alt_timestamp_type endTime = alt_timestamp();
	double timeTaken = ((endTime - this->startTime)/s_timestampTimerFreq)*1000.0;

	if (timeTaken < this->waitTimeMs) {
		return 0;
	} else {
		return 1;
	}
}
