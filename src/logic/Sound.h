/*
 * Sound.h
 *
 *  Created on: 28.12.2019
 *      Author: schwarzwald
 */

#ifndef SRC_SOUND_H_
#define SRC_SOUND_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	SOUND_RIFFWAVE,
	SOUND_OGGOPUS,
} Sound_Type_t;

/**
 * Start reading a sound from flash memory.
 * @param n index of the sound
 */
void Sound_Start(int n);

/**
 * Stop reading a sound from flash memory.
 */
void Sound_Stop(void);

/**
 * Get one sample from the sound file.
 *
 * @param sample pointer to the destination buffer
 *
 * @return false if the sound has ended, true if not
 */
bool Sound_GetSample(int16_t *sample);

uint32_t Sound_ReadCurrentFile(uint8_t *buffer, uint32_t size);
void Sound_LseekCurrentFile(uint32_t offset);


#endif /* SRC_SOUND_H_ */
