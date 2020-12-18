/*
 * led.c
 *
 *  Created on: 15 Sep 2019
 *      Author: schwarzwald
 */

#include "led.h"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

void Led_StatusLed_Init(void) {
	// Init status LED

	// Enable GPIO Peripheral clock
	rcc_periph_clock_enable(RCC_GPIOC);

	Led_StatusLed_Off();

	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, 1 << 13);
	gpio_set_output_options(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, 1 << 13);
}

int led_on = 0;
void Led_StatusLed_On(void) {
	gpio_clear(GPIOC, 1 << 13);
	led_on = 1;
}

void Led_StatusLed_Off(void) {
	gpio_set(GPIOC, 1 << 13);
	led_on = 0;
}

void Led_StatusLed_Toggle(void) {
	if(led_on) Led_StatusLed_Off();
	else Led_StatusLed_On();
}


void Led_Init(void) {
	Led_StatusLed_Init();
}
