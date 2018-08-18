/**
 * Copyright (C) 2018  nhivp
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _MSP_UART_H_
#define _MSP_UART_H_
#include "msp430g2553.h"

#define UART_RX_VECTOR  USCIAB0RX_VECTOR

extern void uart_init(void);
extern void uart_putc(unsigned char character);
extern void uart_puts(char* s);
extern unsigned char uart_getc(void);

#endif /* _MSP_UART_H_ */
