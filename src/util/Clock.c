/*
 * Clock.c
 *
 *  Created on: 13.12.2020
 *      Author: eike
 */

#include "clock.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h>

void Clock_Init() {
	rcc_clock_setup_pll(&rcc_hse_25mhz_3v3[RCC_CLOCK_3V3_84MHZ]);
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
	systick_set_reload((uint32_t)-1);
	systick_counter_enable();
}



uint32_t Clock_GetDeltaTicks(uint32_t lastTicks) {
	uint32_t currentTicks = systick_get_value();
	uint32_t elapsedTicks;
	if(currentTicks < lastTicks) {
		// not wrapped
		elapsedTicks = lastTicks - currentTicks;
	}
	else {
		// wrapped
		elapsedTicks = currentTicks - lastTicks;
	}
	return elapsedTicks;
}

