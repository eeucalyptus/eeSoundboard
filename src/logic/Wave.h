/*
 * Wave.h
 *
 *  Created on: 30.12.2020
 *      Author: eike
 */

#ifndef LOGIC_WAVE_H_
#define LOGIC_WAVE_H_

#include <stdint.h>
#include <stdbool.h>

void Wave_StartSound(void);
bool Wave_GetSample(int16_t *sample);

#endif /* LOGIC_WAVE_H_ */
