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
	systick_set_reload((1 << 24) - 1);
	systick_counter_enable();
}
