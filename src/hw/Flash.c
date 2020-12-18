/*
 * Flash.c
 *
 *  Created on: 14.12.2019
 *      Author: schwarzwald
 */


#include "Flash.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>

#include <stdbool.h>

// avoiding oppressive language
#define spi_init_central spi_init_master

void Flash_Init(void) {

	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_SPI1);

	// COPI, CIPO, CLK
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, (1 << 5) | (1 << 6) | (1 << 7));
	gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, (1 << 5) | (1 << 6) | (1 << 7));
	gpio_set_af(GPIOA, GPIO_AF5, (1 << 5) | (1 << 6) | (1 << 7));

	// nCS
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, (1 << 4));
	gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, (1 << 4));

	spi_init_central(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_2, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
			SPI_CR1_CPHA_CLK_TRANSITION_1, SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);
	spi_enable(SPI1);

}

void Flash_CS_Low() {
	gpio_clear(GPIOA, 1 << 4);
}
void Flash_CS_High() {
	gpio_set(GPIOA, 1 << 4);
}

uint8_t Flash_SendRecv(uint8_t sendData) {
	uint8_t recvData = (uint8_t)spi_xfer(SPI1, sendData);
	return recvData;
}

void Flash_WriteEnable() {
	Flash_CS_Low();
	Flash_SendRecv(0x06);
	Flash_CS_High();
}

void Flash_StartReadStatus() {
	Flash_CS_Low();
	Flash_SendRecv(0x05);
}

uint8_t Flash_ReadStatus() {
	return Flash_SendRecv(0x00);
}

void Flash_StopReadStatus() {
	Flash_CS_High();
}

void Flash_Erase4k(uint32_t address) {
	bool done = false;
	uint8_t status;

	Flash_WriteEnable();

	Flash_CS_Low();
	Flash_SendRecv(0x20);
	Flash_SendRecv((address >> 16) & 0xff);
	Flash_SendRecv((address >>  8) & 0xff);
	Flash_SendRecv((address >>  0) & 0xff);
	Flash_CS_High();

	Flash_StartReadStatus();
	while(!done) {
		status = Flash_ReadStatus();
		done = ((status & 0x01) == 0);
	}
	Flash_StopReadStatus();
}

void Flash_EraseChip() {
	bool done = false;
	uint8_t status;

	Flash_WriteEnable();

	Flash_CS_Low();
	Flash_SendRecv(0xC7);
	Flash_CS_High();

	Flash_StartReadStatus();
	while(!done) {
		status = Flash_ReadStatus();
		done = ((status & 0x01) == 0);
	}
	Flash_StopReadStatus();
}

void Flash_StartProgramPage(uint32_t address) {
	Flash_WriteEnable();

	Flash_CS_Low();
	Flash_SendRecv(0x02);
	Flash_SendRecv((address >> 16) & 0xff);
	Flash_SendRecv((address >>  8) & 0xff);
	Flash_SendRecv((address >>  0) & 0xff);
}
void Flash_ProgramPage(uint8_t data) {
	Flash_SendRecv(data);
}

void Flash_StopProgramPage() {
	bool done = false;
	uint8_t status;

	Flash_CS_High();

	Flash_StartReadStatus();
	while(!done) {
		status = Flash_ReadStatus();
		done = ((status & 0x01) == 0);
	}
	Flash_StopReadStatus();
}

void Flash_ReadID(uint8_t *manufacturer, uint16_t *device) {
	Flash_CS_Low();
	Flash_SendRecv(0x9F);
	*manufacturer = Flash_SendRecv(0);
	uint16_t device1 = Flash_SendRecv(0);
	uint16_t device2 = Flash_SendRecv(0);
	*device = (device1 << 8) | device2;
	(void) device2;
	Flash_CS_High();
}

void Flash_StartRead(uint32_t address) {
	Flash_CS_Low();
	Flash_SendRecv(0x03);
	Flash_SendRecv((address >> 16) & 0xff);
	Flash_SendRecv((address >> 8) & 0xff);
	Flash_SendRecv((address >> 0) & 0xff);
}

uint8_t Flash_Read() {
	return Flash_SendRecv(0x00);
}

void Flash_StopRead() {
	Flash_CS_High();
}
