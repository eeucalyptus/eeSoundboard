/*
 * Power.c
 *
 *  Created on: 14.12.2019
 *      Author: schwarzwald
 */


#include "Power.h"
#include "Keypad.h"

#include <libopencm3/stm32/gpio.h>

void Power_EnableAmplifier(bool enable) {
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, 1<<4);
	gpio_set_output_options(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, 1<<4);
	if(enable) {
		gpio_set(GPIOB, 1<<4);
	}
	else {
		gpio_clear(GPIOB, 1<<4);
	}
}

void Power_Standby(void) {
	Keypad_EnterInterruptMode();
	// WFI
	Keypad_ExitInterruptMode();

}
