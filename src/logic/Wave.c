/*
 * Wave.c
 *
 *  Created on: 30.12.2020
 *      Author: eike
 */


#include "Wave.h"

#include "Sound.h"

#include <stdint.h>
#include <stdbool.h>

uint32_t Wave_SamplesLeft;

void Wave_StartSound() {
	// TODO check RIFF structure
	Sound_LseekCurrentFile(44);
}

bool Wave_GetSample(int16_t *sample) {
	uint32_t br =	Sound_ReadCurrentFile(sample, 2);
	if(br != 2) {
		return false;
	}
	return true;
}
