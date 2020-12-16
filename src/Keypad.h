/*
 * Keypad.h
 *
 *  Created on: 19 Sep 2019
 *      Author: schwarzwald
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "stdbool.h"
#include "stdint.h"

/**
 * Initialize the keypad
 */
void Keypad_Init(void);

/**
 * Get current key, returns -1 if no key is pressed
 */
int Keypad_GetKey(void);

/**
 * Enable interrupt for all keys. The core will wake up from interrupt.
 */
void Keypad_EnterInterruptMode(void);

/**
 * Disable interrupt for all keys and return to normal polling.
 */
void Keypad_ExitInterruptMode(void);


#endif /* KEYPAD_H_ */
