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

#include "msp_printf.h"
#include "stdarg.h"

static void xtoa(unsigned long x, int width, const int *wp, const unsigned long *dp);
static void puth(unsigned n);

static const unsigned long dv[] =
{
    // 4294967296 // 32 bit unsigned max
    1000000000, // +0
    100000000, // +1
    10000000, // +2
    1000000, // +3
    100000, // +4
    // 65535 // 16 bit unsigned max
    10000, // +5
    1000, // +6
    100, // +7
    10, // +8
    1, // +9
};

static const int dw[] =
{ // Width of integer
    // 4294967296 // 32 bit unsigned max
    10, // +0
    9, // +1
    8, // +2
    7, // +3
    6, // +4
    // 65535 // 16 bit unsigned max
    5, // +5
    4, // +6
    3, // +7
    2, // +8
    1, // +9
};

static void xtoa(unsigned long x, int width, const int *wp, const unsigned long *dp)
{
    char c;
    unsigned long d;

    if(x)
    {
        while(x < *dp)
        {
            ++dp;
            if (width >= *wp) uart_putc('0');
            ++wp;
        }

        do
        {
            d = *dp++;
            c = '0';
            while(x >= d) ++c, x -= d;
            uart_putc(c);
        } while(!(d & 1));
    }
    else
    {
        while (width--) uart_putc('0');
    }
}

static void puth(unsigned n)
{
    static const char hex[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    uart_putc(hex[n & 15]);
}

void printformat(char *format, ...)
{
    char formatChar;
    int i;
    long n;
    int width = 1;

    va_list a;
    va_start(a, format);
    while(*format)
    {
        formatChar = *format++;
        if(formatChar == '%')
        {
            formatChar = *format++;
            if ((formatChar > 48) && (formatChar < 58) )
            {
                width = formatChar-48;
                formatChar = *format++;
            }
            switch(formatChar)
            {
                case 's': // String
                    uart_puts(va_arg(a, char*));
                    break;
                case 'c': // Char
                    uart_putc(va_arg(a, unsigned));
                    break;
                case 'i': // 16 bit Integer
                case 'u': // 16 bit Unsigned
                    i = va_arg(a, int);
                    if(formatChar == 'i' && i < 0) i = -i, uart_putc('-');
                    xtoa((unsigned)i, width, dw + 5, dv + 5); // Send 16 bit integer to xtoa and point to +5 element of dv
                    break;
                case 'l': // 32 bit Long
                case 'n': // 32 bit unsigned long
                    n = va_arg(a, long);
                    if(formatChar == 'l' && n < 0) n = -n, uart_putc('-');
                    xtoa((unsigned long)n, width, dw, dv);
                    break;
                case 'x': // 16 bit hexadecimal
                    i = va_arg(a, int);
                    puth(i >> 12);
                    puth(i >> 8);
                    puth(i >> 4);
                    puth(i);
                    break;
                case 0: return;
                default: goto bad_fmt;
            }
        }
        else
            bad_fmt: uart_putc(formatChar);
    }
    va_end(a);
}
