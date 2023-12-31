/*
 * Soundboard.c
 *
 *  Created on: 14.12.2019
 *      Author: schwarzwald
 */


#include "Soundboard.h"
#include "Sound.h"

#include <hw/Flash.h>
#include <hw/Keypad.h>
#include <hw/AudioPWM.h>
#include <hw/Power.h>
#include <hw/USB_MSC.h>
#include <hw/Stopwatch.h>

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>


int Soundboard_LastKeyPressed = -1;

int Soundboard_CurrentSound;
bool Soundboard_Playing = false;

/**
 * Start the sound at the given index
 * @param sound Index of the sound to be played
 */
void Soundboard_StartSound(int sound) {
	Soundboard_CurrentSound = sound;
	Soundboard_Playing = true;
	Sound_Start(sound);
	AudioPWM_ResetBuffer();
}

/**
 * Continue getting samples from flash and push them to the sample buffer.
 * @returns true if playback is supposed to continue
 * 			false if playback is supposed to stop (interrupted or ended)
 */
bool Soundboard_ContinueSound(void) {
	// Check if a key has been released and pressed again
//	int key = Keypad_GetKey();
//	if(key >= 0 && Soundboard_LastKeyPressed == -1) {
//		//Soundboard_Playing = false;
//	}
//	Soundboard_LastKeyPressed = key;

	// Check if sound is continuing
	if(Soundboard_Playing) {
		// TODO there is one undefined sample added because
		// Soundboard_Playing is checked in the next cycle instead
		// of immediately
		int16_t sample;
		Soundboard_Playing = Sound_GetSample(&sample);
		AudioPWM_Sample(sample);
		AudioPWM_EnableSampling(true);
		return true;
	}
	else {
		return false;
	}
}
void Soundboard_StopSound(void) {
	Sound_Stop();
	AudioPWM_EnableSampling(false);
}


void Soundboard_Run() {
	while(1) {
		Soundboard_LastKeyPressed = Keypad_GetKey();
		// Check if any key is pressed
		if(Soundboard_LastKeyPressed >= 0) {
			// Start, continue and stop sound
			Soundboard_StartSound(Soundboard_LastKeyPressed);
			while(Soundboard_ContinueSound());
			AudioPWM_Wait();
			Soundboard_StopSound();
			sleep(1);
		}
		else {
			// Save power until key is pressed
			Power_Standby();
		}

		USB_MSC_Poll();
	}
}
