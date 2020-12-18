/*
 * Serial.c
 *
 *  Created on: 13.01.2020
 *      Author: schwarzwald
 */


#include "Serial.h"
#include "Flash.h"

#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include <stdbool.h>

#define TIMEOUT 100

void Serial_putc(char c) {
	usart_send_blocking(USART2, (uint16_t)c);
}

void Serial_puts(char *str) {
	while(*str != 0) {
		Serial_putc(*str++);
	}
}

char Serial_getchar(void) {
	while(usart_get_flag(USART2, USART_FLAG_RXNE) == 0);
	return usart_recv(USART2);
}


bool Serial_CheckTrigger(void) {
	// Ungate clock
	rcc_periph_clock_enable(RCC_GPIOA);


	// UART GND
	gpio_clear(GPIOA, (1 << 4));
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, (1 << 4));
	gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, (1 << 4));

	// Init trigger pin
	gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, (1 << 6));

	// Wait some cycles...
	for(int i = 0; i < 1000; i++) {asm volatile("nop");}

	return gpio_get(GPIOA, (1 << 6)) == 0;
}

void Serial_Init(void) {
	// Ungate clocks
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_USART2);

	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, (1 << 2) | (1 << 3));
	gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, (1 << 2) | (1 << 3));
	gpio_set_af(GPIOA, GPIO_AF7, (1 << 2) | (1 << 3));

	// Init UART module
	usart_set_baudrate(USART2, 115200);
	usart_set_mode(USART2, USART_MODE_TX_RX);
	usart_set_databits(USART2, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_enable(USART2);

}

uint8_t pageBuffer[256];
void Serial_Run(void) {
	int byteCount = 0;

	// Guard this section by checking for loader pin
	//if(!Serial_CheckTrigger()) {
	//	return;
	//}

	// Initialize serial module and pins
	Serial_Init();

	//if(!Serial_WaitForHost(10000000)) {
	//	Serial_puts("Skip to prevent lock!\n");
	//	return;
	//}
//	Serial_puts("READY\n");
	Serial_putc('R');
	Serial_putc('E');
	Serial_putc('A');
	Serial_putc('D');
	Serial_putc('Y');
	Serial_putc('\n');

	while(byteCount < 0x400000) {
		// Receive page
		for(int i = 0; i < 256; i++) {
			pageBuffer[i] = Serial_getchar();
			//Serial_putc(pageBuffer[i]);
		}

		if(byteCount == 0) {
			Flash_EraseChip(1);
		}

		Flash_StartProgramPage(byteCount);
		for(int i = 0; i < 256; i++) {
			Flash_ProgramPage(pageBuffer[i]);
		}
		Flash_StopProgramPage(1);

		byteCount+=256;

		if(byteCount < 0x400000)
			Serial_puts("NEXT\n");
	}
	Serial_puts("DONE\n");


	// Erase flash
	// Send "READY"
	// Receive and write pages
	// Send "DONE"
	// While(1);
}
