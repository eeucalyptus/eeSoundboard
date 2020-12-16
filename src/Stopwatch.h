/*
 * Stopwatch.h
 *
 *  Created on: 17 Sep 2019
 *      Author: schwarzwald
 */

#ifndef STOPWATCH_H_
#define STOPWATCH_H_

#include "stdint.h"

/**
 * Starts the stopwatch for microsecond timing analysis
 */
void Stopwatch_Start(void);
/**
 * Stops the stopwatch and returns the elapsed time in us.
 * Warning: maximum period is 0,233 s
 */
uint32_t Stopwatch_Stop(void);

#endif /* STOPWATCH_H_ */
