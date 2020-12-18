/*
 * Serial.h
 *
 *  Created on: 13.01.2020
 *      Author: schwarzwald
 */

#ifndef SERIAL_H_
#define SERIAL_H_

/**
 * Erase the flash memory, receive new image and write it to memory
 */
void Serial_Run(void);

void Serial_Init(void);
void Serial_puts(char * str);
void Serial_putc(char c);
char Serial_getchar(void);


#endif /* SERIAL_H_ */
