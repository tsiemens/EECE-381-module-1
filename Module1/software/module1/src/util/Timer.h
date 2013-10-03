/*
 * Timer.h
 *
 *  Created on: 2013-10-02
 *  Author: Trevor Siemens
 */

#ifndef TIMER_H_
#define TIMER_H_

typedef struct Timer
{
	// When the timer was started
	long startTime;

	// How long the timer should run
	double waitTimeMs;
} Timer;

Timer* Timer_new(double waitTimeMs);

void Timer_start(Timer* this);

int Timer_isDone(Timer* this);

#endif /* TIMER_H_ */
