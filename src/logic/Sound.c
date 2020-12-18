/*
 * Sound.c
 *
 *  Created on: 29.12.2019
 *      Author: schwarzwald
 */


#include "Sound.h"

#include <hw/Flash.h>
#include <fatfs/ff.h>

#include <stdbool.h>
#include <stdint.h>

uint32_t Sound_SamplesLeft;

FATFS fs;

FIL currentSound;

void Sound_Start(int n) {
	// Mount if necessary
	if(fs.fs_type == 0) {
		f_mount(&fs, "", 0);
	}

	f_open(&currentSound, "data.wav", FA_READ);

	// TODO check RIFF structure

	f_lseek(&currentSound, 44);
}

void Sound_Stop(void) {
	f_close(&currentSound);
}

bool Sound_GetSample(int16_t *sample) {
	UINT br;
	f_read(&currentSound, sample, 2, &br);
	if(br != 2) {
		return false;
	}
	return true;
}
