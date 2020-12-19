/*
 * AudioPWM.c
 *
 *  Created on: 16 Sep 2019
 *      Author: schwarzwald
 */

#include "AudioPWM.h"

#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/f4/nvic.h>
#include <libopencm3/stm32/dbgmcu.h>

#include <stdbool.h>
#include <stdint.h>

// TODO Adjust Period
#define AUDIOPWM_SAMPLINGRATE (48000)
#define AUDIOPWM_SUBSAMPLE_COUNT (2)
#define AUDIOPWM_TIMER_FREQUENCY (rcc_apb2_frequency)
#define PWM_PERIOD  (AUDIOPWM_TIMER_FREQUENCY / (AUDIOPWM_SAMPLINGRATE * AUDIOPWM_SUBSAMPLE_COUNT))

static int AudioPWM_SubSampleCount = 0;
static bool AudioPWM_SamplingEnabled = true;

static volatile uint32_t AudioPWM_SampleMissingCount;

uint16_t AudioPWM_Buffer = 0;
volatile bool AudioPWM_BufferFull = false;

void AudioPWM_Init() {
	// Ungate clocks
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_TIM1);

	// Stop TIM1 on debug halt
	*(uint32_t *)(0xE004200C) = 0x1;

	// Init audio GPIO
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, 1 << 14);
	gpio_set_output_options(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, 1 << 14);
	gpio_set_af(GPIOB, GPIO_AF1, 1 << 14);

	// GND-Level for voltage divider
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, 1 << 15);
	gpio_set_output_options(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, 1 << 15);
	gpio_clear(GPIOB, 1 << 15);

	timer_set_mode(TIM1, TIM_CR1_CKD_CK_INT,
		TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	timer_set_prescaler(TIM1, 0);
	timer_set_period(TIM1, PWM_PERIOD);

	timer_set_oc_polarity_high(TIM1, TIM_OC2N);
	timer_set_oc_idle_state_set(TIM1, TIM_OC2N);

	timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_PWM1);
	timer_set_oc_value(TIM1, TIM_OC2, PWM_PERIOD/2);
	timer_enable_oc_output(TIM1, TIM_OC2N);

	timer_enable_break_main_output(TIM1);

	timer_enable_irq(TIM1, TIM_DIER_UIE);
	nvic_enable_irq(NVIC_TIM1_UP_TIM10_IRQ);
}

void AudioPWM_EnableSampling(bool enabled) {
	if(enabled) {
		timer_enable_counter(TIM1);
	}
	else {
		timer_disable_counter(TIM1);
	}
}

void tim1_up_tim10_isr() {
	// Clear interrupt
	timer_clear_flag(TIM1, TIM_SR_UIF);

	// Execute every AUDIOPWM_SUBSAMPLE_COUNT timer interrupts
	if(++AudioPWM_SubSampleCount >= AUDIOPWM_SUBSAMPLE_COUNT) {
		AudioPWM_SubSampleCount = 0;
		if(AudioPWM_SamplingEnabled) {
			// Check for missing samples
			if(AudioPWM_BufferFull) {
//				 Set PWM compare value
				timer_set_oc_value(TIM1, TIM_OC2, AudioPWM_Buffer);
//				TIM_SetCompare1(TIM1, AudioPWM_Buffer);

				// Reset buffer
				AudioPWM_BufferFull = false;
			}
			else
			{
				AudioPWM_SampleMissingCount++;
			}

		}
	}
}

uint16_t AudioPWM_ScaleAndConvert(int16_t signedSample) {
	int32_t sample = ((int32_t)signedSample * (PWM_PERIOD/2)) / 32768;
	sample += PWM_PERIOD/2;
	return (uint16_t)sample;
}

void AudioPWM_Sample(int16_t sample) {
	while(AudioPWM_BufferFull);
	AudioPWM_Buffer = AudioPWM_ScaleAndConvert(sample);
	AudioPWM_BufferFull = true;
}
