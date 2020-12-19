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
#include <string.h>

uint32_t Sound_SamplesLeft;

FATFS fs;

FIL currentSound;

void Sound_Start(int n) {
	// Mount if necessary
	if(fs.fs_type == 0) {
		f_mount(&fs, "", 0);
	}

	char filename[13];
	strcpy(filename, "sound_00.wav");

	filename[6] += (n+1)/10;
	filename[7] += (n+1)%10;

	f_open(&currentSound, filename, FA_READ);

	// TODO check RIFF structure

	f_lseek(&currentSound, 44);
}

void Sound_Stop(void) {
	f_close(&currentSound);
}

bool Sound_GetSample(int16_t *sample) {
	UINT br;
	// TODO Consumes too much time. A double or ring buffer is needed!
	f_read(&currentSound, sample, 2, &br);
	if(br != 2) {
		return false;
	}
	return true;
}
