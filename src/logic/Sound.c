/*
 * Sound.c
 *
 *  Created on: 29.12.2019
 *      Author: schwarzwald
 */


#include "Sound.h"

#include "OggOpus.h"
#include "Wave.h"
#include <hw/Flash.h>
#include <fatfs/ff.h>

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

FATFS fs;
FIL Sound_CurrentFile;
Sound_Type_t Sound_CurrentType;

void Sound_LseekCurrentFile(uint32_t offset) {
	f_lseek(&Sound_CurrentFile, offset);
}

uint32_t Sound_ReadCurrentFile(uint8_t *buffer, uint32_t size) {
	uint32_t bytesread;
	f_read(&Sound_CurrentFile, buffer, size, (UINT *)&bytesread);
	return bytesread;
}

void Sound_Start(int n) {
	// Mount if necessary
	if(fs.fs_type == 0) {
		f_mount(&fs, "", 0);
	}

	// TODO Open either wav or opus file

	char filename[14];
	strcpy(filename, "sound_00.opus");
	Sound_CurrentType = SOUND_OGGOPUS;

	filename[6] += (n+1)/10;
	filename[7] += (n+1)%10;

	volatile FRESULT res = f_open(&Sound_CurrentFile, "sound_01.ogg", FA_READ);

	switch(Sound_CurrentType) {
	case SOUND_RIFFWAVE:
		Wave_StartSound();
		break;
	case SOUND_OGGOPUS:
		OggOpus_StartSound();
		break;
	}
}

void Sound_Stop(void) {
	f_close(&Sound_CurrentFile);
}

bool Sound_GetSample(int16_t *sample) {
	if(Sound_CurrentType == SOUND_RIFFWAVE) {
		return Wave_GetSample(sample);
	}
	else if(Sound_CurrentType == SOUND_OGGOPUS) {
		return OggOpus_GetSample(sample);
	}
	else return false;
}
