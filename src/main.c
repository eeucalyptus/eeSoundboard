#include "Flash.h"
#include "Serial.h"
#include "Keypad.h"
#include "led.h"
#include "AudioPWM.h"
#include "Soundboard.h"
#include "util/clock.h"
#include "USB_MSC.h"
#include "fatfs/ff.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h>
#include <unistd.h>
#include <stdint.h>

FATFS fs;

int main()
{
	Clock_Init();
	Flash_Init();
	f_mount(&fs, "", 0);
	FIL f;
	f_open(&f, "data.wav", FA_READ);

	static uint8_t data[4096];
	UINT br;

	f_read(&f, data, 4096, &br);

	while(1);
}
//{
//	Clock_Init();
//	Led_Init();
//	Flash_Init();
//
//	// Run serial rom loader if needed
//	Serial_Run();
//
//	AudioPWM_Init();
//	Keypad_Init();
//
//	for(int i = 0; i<1000000; i++);
//
//	// Run soundboard
//	Soundboard_Run();
//
//	while(1) {
//		asm volatile("nop");
//	}
//	return 0;
//}
