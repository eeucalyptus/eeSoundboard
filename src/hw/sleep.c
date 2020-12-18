/*
 * sleep.c
 *
 *  Created on: 13.12.2020
 *      Author: eike
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h>
#include <unistd.h>
#include <stdint.h>

unsigned sleep (unsigned int __seconds) {
	usleep(__seconds*1000000);
	return 0;
}

int usleep (useconds_t __useconds) {
	int32_t ticksToWait = (int32_t)(__useconds * 10.5f);

	int32_t lastTick = (int32_t)systick_get_value();
	while(ticksToWait > 0) {
		int32_t currentTick = (int32_t)systick_get_value();
		if(currentTick <= lastTick) {
			ticksToWait += currentTick - lastTick;
		}
		else {
			ticksToWait += (currentTick - (1<<24)) - lastTick;
			//break;
		}
		lastTick = currentTick;
	}
	return 0;
}
