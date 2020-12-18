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
 * User defined. This function will be called to signal that the last
 * value has been sampled. AudioPWM_Sample has to be called within 1/8kHz.
 */
extern void AudioPWM_NeedSampleCallback(void);


#endif /* AudioPWM_H_ */
