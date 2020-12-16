/*
 * Power.h
 *
 *  Created on: 14.12.2019
 *      Author: schwarzwald
 */

#ifndef SRC_POWER_H_
#define SRC_POWER_H_

#include <stdbool.h>

/**
 * Save power and wait for user input
 */
void Power_Standby(void);

/**
 * Enable or disable power for the amplifier
 */
void Power_EnableAmplifier(bool enable);


#endif /* SRC_POWER_H_ */
