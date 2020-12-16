/*
 * Keypad.c
 *
 *  Created on: 19 Sep 2019
 *      Author: schwarzwald
 */

#include "Keypad.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	uint32_t Pin;
	uint32_t Port;
} Keypad_Gpio_t;

/**
 * Pin definitions for the keypad row pins // TODO Set correct pins, add RCC calculation
 */
Keypad_Gpio_t Keypad_RowPins[4] = {
		{.Port = GPIOB, .Pin = 5},
		{.Port = GPIOB, .Pin = 6},
		{.Port = GPIOB, .Pin = 7},
		{.Port = GPIOB, .Pin = 8},
};

/**
 * Pin definitions for the keypad column pins
 */
Keypad_Gpio_t Keypad_ColPins[4] = {
		{.Port = GPIOA, .Pin = 15},
		{.Port = GPIOA, .Pin = 12},
		{.Port = GPIOA, .Pin = 11},
		{.Port = GPIOA, .Pin = 10},
};


/**
 * Currently selected row
 */
static int Keypad_ActiveCol = -1;

/**
 * Select one keypad row by pulling it low while pulling up all others
 *
 * @param row Row to be selected (1 to 4)
 */
void Keypad_SetActiveCol(int col) {
	// Save new active row
	Keypad_ActiveCol = col;

	// Set all col pins as high
	for(int i = 0; i < 4; i++) {
		gpio_set(Keypad_ColPins[i].Port, 1 << Keypad_ColPins[i].Pin);
	}
	// Set active col low
	gpio_clear(Keypad_ColPins[Keypad_ActiveCol].Port, 1 << Keypad_ColPins[Keypad_ActiveCol].Pin);
}


void Keypad_Init(void) {
	// Enable GPIO Peripheral clock
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);


	// Set all row pins as pull-up
	for(int i = 0; i < 4; i++) {
		gpio_mode_setup(Keypad_RowPins[i].Port, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, 1 << Keypad_RowPins[i].Pin);
	}
	// Set all col pins as high
	for(int i = 0; i < 4; i++) {
		gpio_set(Keypad_ColPins[i].Port, 1 << Keypad_ColPins[i].Pin);
		gpio_mode_setup(Keypad_ColPins[i].Port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, 1 << Keypad_ColPins[i].Pin);
		gpio_set_output_options(Keypad_ColPins[i].Port, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, 1 << Keypad_ColPins[i].Pin);
	}

	// Configure pins
	Keypad_SetActiveCol(0);
}

int Keypad_GetKey() {
	for(int col = 0; col <= 3; col++) {
		// Set active row
		Keypad_SetActiveCol(col);
		// active wait for some us
		for(int i = 0; i<10000; i++);

		// Check all cols
		for(int row = 0; row < 4; row++) {
			uint32_t in = gpio_port_read(Keypad_RowPins[row].Port);
			if((in & (1 << Keypad_RowPins[row].Pin)) == 0) {
				return row*4+col;
			}
		}
	}
	return -1;
}


void Keypad_EnterInterruptMode(void) {
	// TODO Implement
}

void Keypad_ExitInterruptMode(void) {
	// TODO Implement
}
