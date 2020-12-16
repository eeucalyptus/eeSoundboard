/*
 * Stopwatch.c
 *
 *  Created on: 17 Sep 2019
 *      Author: schwarzwald
 */

#include "Stopwatch.h"

#include <libopencm3/cm3/systick.h>

#include <stdint.h>

static int32_t Stopwatch_StartTicks;

void Stopwatch_Start(void) {
	Stopwatch_StartTicks = (int32_t)systick_get_value();
}

uint32_t Stopwatch_Stop(void) {
	int32_t stopTicks = (int32_t)systick_get_value();

	// Check for wrap around in SysTick
	int32_t elapsedTicks;
	if(stopTicks < Stopwatch_StartTicks) {
		elapsedTicks = stopTicks - Stopwatch_StartTicks;
	}
	else {
		elapsedTicks = Stopwatch_StartTicks + 0x1000000 - stopTicks;
	}

	// Calculate us from ticks
	uint32_t elapsedUs = (uint32_t)((float)elapsedTicks / 10.5f);

	return elapsedUs;
}
