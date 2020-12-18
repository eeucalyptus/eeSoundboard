/*
 * Sound.c
 *
 *  Created on: 29.12.2019
 *      Author: schwarzwald
 */


#include "Sound.h"

#include <hw/Flash.h>

#include <stdbool.h>
#include <stdint.h>

uint32_t Sound_SamplesLeft;

void Sound_Start(int n) {
	Flash_StopRead();
	Flash_StartRead(0);
	Sound_SamplesLeft = 512;

	return;
	uint8_t manufacturer;
	uint8_t device;

	Flash_ReadID(&manufacturer, &device);

	int i;
	uint8_t buf[4];
	uint32_t length = 0;
	uint32_t nextStartAddress = 0;

	// Find the start of the sample recursively
	for(i = 0; i <= n; i++) {
		// Restart reading from flash at the next start address
		Flash_StopRead();
		for(int i = 0; i < 10000; i++);
		Flash_StartRead(nextStartAddress);

		buf[0] = Flash_Read();
		buf[1] = Flash_Read();
		buf[2] = Flash_Read();
		buf[3] = Flash_Read();

		// Read sound length
		*((uint8_t *)(&length) + 0) = buf[0];
		*((uint8_t *)(&length) + 1) = buf[1];
		*((uint8_t *)(&length) + 2) = buf[2];
		*((uint8_t *)(&length) + 3) = buf[3];

		// calculate next start address
		nextStartAddress += 4 + length*2;
	}

	Sound_SamplesLeft = length;
}

void Sound_Stop(void) {
	Flash_StopRead();
}

static int16_t testsample = 0;
static int i = 0;
bool Sound_GetSample(int16_t *sample) {
	uint8_t data[2];

	// Check if sound has ended
	if(Sound_SamplesLeft == 0) {
		// Sound has ended
		return false;
	}
	else {
		// Read two bytes to buffer
		data[0] = Flash_Read();
		data[1] = Flash_Read();

		// Write sample
		int16_t x = testsample + 901;
		if(testsample > x) {
			i++;
		}
		testsample += 901;
		*sample = (int16_t)(((uint16_t)data[0] << 8) | ((uint16_t)data[1]));
//		*sample = testsample;

		// Decrease numbers of samples left in sound
		Sound_SamplesLeft--;

		return true;
	}
}
