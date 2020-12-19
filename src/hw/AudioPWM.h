/*
 * Audio.h
 *
 *  Created on: 16 Sep 2019
 *      Author: schwarzwald
 */

#ifndef AudioPWM_H_
#define AudioPWM_H_

#include "stdint.h"
#include "stdbool.h"

/**
 * Initialize PWM audio. Starts calling AudioPWM_NeedSampleCallback every 1/8kHz
 */
void AudioPWM_Init(void);

/**
 * Set the next value to be sampled
 */
void AudioPWM_Sample(int16_t nextSample);

/**
 * Enable/disable sampling. While enabled, samples will be requested by
 * calling AudioPWM_NeedSampleCallback(). Disabling sets all samples to a fixed value.
 */
void AudioPWM_EnableSampling(bool enabled);

/**
 * Wait until buffer emptied. Used to synchronize with playback.
 */
void AudioPWM_Wait(void);

/**
 * Reset buffer to play new samples immediately
 */
void AudioPWM_ResetBuffer(void);


#endif /* AudioPWM_H_ */
