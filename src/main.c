#include <hw/Flash.h>
#include <hw/Serial.h>
#include <hw/Keypad.h>
#include <hw/led.h>
#include <hw/AudioPWM.h>
#include <logic/Soundboard.h>
#include <hw/clock.h>
#include <hw/USB_MSC.h>
#include <hw/Stopwatch.h>

#include <logic/OggOpus.h>
#include <logic/Sound.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


int main()
{
	Clock_Init();
	Led_Init();
	Flash_Init();
	USB_MSC_Init();
	AudioPWM_Init();
	Keypad_Init();
	OggOpus_Init();

//	Flash_EraseChip();

	for(int i = 0; i<1000000; i++);

	// Run soundboard
	Soundboard_Run();

	while(1) {
		USB_MSC_Poll();
		asm volatile("nop");
	}
	return 0;
}
