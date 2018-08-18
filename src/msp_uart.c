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

#include "msp_uart.h"

/**
 * Initialize UART
 */
void uart_init(void)
{
    /* Configure hardware UART */
    P1SEL = BIT1 + BIT2 ;  // P1.1 = RXD, P1.2=TXD
    P1SEL2 = BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD
    UCA0CTL1 |= UCSSEL_2;  // Use SMCLK
    UCA0BR0 = 104;         // Set baud rate to 9600 with 1MHz clock (Data Sheet 15.3.13)
    UCA0BR1 = 0;           // Set baud rate to 9600 with 1MHz clock
    UCA0MCTL = UCBRS0;     // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST;  // Initialize USCI state machine
    IE2 |= UCA0RXIE;       // Enable USCI_A0 RX interrupt
}

/**
 * Send a single byte out through UART
 */
void uart_putc(unsigned char character)
{
    while (!(IFG2 & UCA0TXIFG)); // USCI_A0 TX buffer ready?
    UCA0TXBUF = character;       // TX -> RXed character
}

/**
 * Sends a string out through UART
 */
void uart_puts(char* s)
{
    while (*s != '\0')
    {
        uart_putc(*s);
        s++;
    }
}

/**
 * Receive a single byte out through UART
 */
unsigned char uart_getc(void)
{
    return UCA0RXBUF;
}
