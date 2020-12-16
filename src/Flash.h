/*
 * Flash.h
 *
 *  Created on: 14.12.2019
 *      Author: schwarzwald
 */

#ifndef SRC_FLASH_H_
#define SRC_FLASH_H_

#include <stdint.h>

/**
 * Initialize the flash memory driver
 */
void Flash_Init(void);

/**
 * Set the write enable latch of the memory.
 *
 * Blocks until the WEL is set.
 *
 * @param n The index of the memory chip (for chip selection)
 */
void Flash_WriteEnable();

/**
 * Start reading to the flash memory.
 *
 * Blocks until the memory is ready to read data from the memory using Flash_Read.
 *
 * @param n The index of the memory chip (for chip selection)
 * @param address defines the start address for reading
 */
void Flash_StartRead(uint32_t address);

/**
 * Read one byte from the flash memory and increase source address.
 *
 * Blocks until one byte is receivedand the memory is ready to
 * read data from the memory using Flash_Read again.
 *
 * @return Byte read from flash memory
 */
uint8_t Flash_Read(void);

/**
 * Stop reading from the flash memory
 *
 * Blocks until reading has been stopped.
 */
void Flash_StopRead(void);

/**
 * Read the manufacturer and device id from flash.
 *
 * Blocks until the id is received.
 *
 * @param n The index of the memory chip (for chip selection)
 * @param manufacturer is the return pointer for the manufacturer id
 * @param device is the return pointer for the device id
 */
void Flash_ReadID(uint8_t *manufacturer, uint16_t *device);

/**
 * Start reading the status register.
 * Needs to be called before calling Flash_ReadStatus.
 * Flash_StopReadStatus needs to be called before calling
 * any other memory function.
 *
 * Blocks until the status is ready to be received using Flash_ReadStatus.
 *
 * @param n The index of the memory chip (for chip selection)
 */
void Flash_StartReadStatus();

/**
 * Read the status register of the memory once.
 * Can be called repeatedly without restarting the
 * reading of the status register.
 *
 * Blocks until the status is read and ready to be received again.
 */
uint8_t Flash_ReadStatus(void);

/**
 * Stop reading the status register.
 * Needs to be called after calling Flash_StartReadStatus
 * before any other memory function can be called again.
 *
 * Blocks until reading the status is stopped.
 */
void Flash_StopReadStatus(void);

/**
 * Erase a 64k Block of the memory. Has to be called for each block
 * of the Memory before programming it.
 *
 * Blocks until erasing the block has finished.
 *
 * @param n The index of the memory chip (for chip selection)
 * @param address The address of one byte within the block (usually the first byte)
 */
void Flash_Erase64k(uint32_t address);

/**
 * Erase a whole memory chip.
 *
 * Blocks until erasing the memory has finished.
 *
 * @param n The index of the memory chip (for chip selection)
 */
void Flash_EraseChip();

/**
 * Program a page within the memory. The start address and the
 * end address (start address + number of calls of Flash_ProgramPage)
 * have to be within the same 256 byte page (only last byte of the address
 * may differ)
 *
 * @param n The index of the memory chip (for chip selection)
 * @param address The address of one byte within the block (usually the first byte)
 */
void Flash_StartProgramPage(uint32_t address);

/**
 * Send one byte to be programmed to the memory.
 *
 * Blocks until the byte is sent.
 *
 * @param data The byte to be sent
 */
void Flash_ProgramPage(uint8_t data);

/**
 * Program the previously sent bytes to the memory.
 *
 * Blocks until programming the page has finished.
 *
 * @param n The index of the memory chip (for chip selection)
 */
void Flash_StopProgramPage();

#endif /* SRC_FLASH_H_ */
