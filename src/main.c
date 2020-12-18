#include "Flash.h"
#include "Serial.h"
#include "Keypad.h"
#include "led.h"
#include "AudioPWM.h"
#include "Soundboard.h"
#include "util/clock.h"
#include "USB_MSC.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h>
#include <unistd.h>
#include <stdint.h>

int main()
//{
//	Clock_Init();
//	Flash_Init();
//	USB_MSC_Init();
//	while(1) {
//		USB_MSC_Poll();
//	}
//}
{
	Clock_Init();
	Led_Init();
	Flash_Init();

	// Run serial rom loader if needed
	Serial_Run();

	AudioPWM_Init();
	Keypad_Init();

	for(int i = 0; i<1000000; i++);

	// Run soundboard
	Soundboard_Run();

	while(1) {
		asm volatile("nop");
	}
	return 0;
}
