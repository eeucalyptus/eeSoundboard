/*
 * clock.h
 *
 *  Created on: 13.12.2020
 *      Author: eike
 */

#ifndef UTIL_CLOCK_H_
#define UTIL_CLOCK_H_

#include <stdint.h>

void Clock_Init(void);

uint32_t Clock_GetDeltaTicks(uint32_t lastTicks);

#endif /* UTIL_CLOCK_H_ */
