/*
 * Power.c
 *
 *  Created on: 14.12.2019
 *      Author: schwarzwald
 */


#include "Power.h"
#include "Keypad.h"

void Power_EnableAmplifier(bool enable) {

}

void Power_Standby(void) {
	AudioPWM_EnableSampling(false);
	Keypad_EnterInterruptMode();
	// WFI
	Keypad_ExitInterruptMode();

}
