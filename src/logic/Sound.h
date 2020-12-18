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


#endif /* SRC_SOUND_H_ */
