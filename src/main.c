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
#include "msp_uart.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/**
 *  Typedef definition
 */
typedef void (*CLI_Command_Function_t)(void);

typedef struct
{
    char Command[32];
    char Command_Desc[64];
    CLI_Command_Function_t Command_Func;
} CLI_Command_t;

/**
 *  Macro definition
 */
#define COMMAND_LEN(x)            sizeof(x)/sizeof(*(&x[0]))
#define COMMAND_STRING_LEN        128

/**
 * Private function prototypes
 */
static void board_init(void);
static void startup_cli(void);
static void CLI_GetCommand(unsigned char* cmd);
static void CLI_Help(void);
static void CLI_Hello(void);
static void CLI_Info(void);

/**
 *  Private variables
 */
static CLI_Command_t command_tbl[] =
{
    /* Command, Description,                    Command_Func */
    { "help"  , "Show a list of command.",      CLI_Help     },
    { "info"  , "Show all features of MCU",     CLI_Info     },
    { "hello" , "Say Hello, World.",            CLI_Hello    }
};

/**
 *  Exported variables
 */
unsigned char parameterString[COMMAND_STRING_LEN];
uint8_t parameterLength;
volatile bool validCommandFlag;
extern volatile uint16_t __m_flash_size;
extern volatile uint16_t __m_ram_size;

/**
 * Main App
 */
int main(void)
{
    unsigned char cmd[32];
    uint8_t cmd_idx;

    board_init();
    uart_init();
    memset(parameterString, '\0', COMMAND_STRING_LEN);
    memset(cmd, '\0', 32);

    /* Show banner */
    startup_cli();
    /* Enable interrupt */
    __bis_SR_register(GIE);

    while (1)
    {
        /* The 'validCommandFlag' is true when the user enter an input command from console */
        while (validCommandFlag)
        {
            CLI_GetCommand(cmd);

            if (cmd[0] == '\0')
            {
                printf("\r\nMissing command!");
            }
            else
            {
                for (cmd_idx = 0; cmd_idx < COMMAND_LEN(command_tbl); cmd_idx++)
                {
                    if (!strcmp((char*)cmd, (char*)command_tbl[cmd_idx].Command))
                    {
                        break;
                    }
                }

                if (cmd_idx < COMMAND_LEN(command_tbl))
                {
                    /* Execute command */
                    command_tbl[cmd_idx].Command_Func();
                }
                else
                {
                    printf("\r\nInvalid command!");
                }
            }

            printf("\r\nMSP430@CLI > ");

            /* Reset receive buffer and flag*/
            memset(parameterString, '\0', parameterLength + 1);
            memset(cmd, '\0', 32);
            parameterLength = 0;
            validCommandFlag = false;
        }
    }
}

/**
 * Initialize the board MSP430G2 LaunchPad
 */
static void board_init(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop WDT
    /* Load calibrated DCO vlues to set CPU clock to 1MHz */
    BCSCTL1 = CALBC1_1MHZ;    // Set DCO to 1MHz
    DCOCTL = CALDCO_1MHZ;     // Set DCO to 1MHz
}

/**
 * Start CLI
 */
static void startup_cli(void)
{
    printf("\r\n");
    printf("*----------------------------------------*\r\n");
    printf("*         MSP-EXP430G2 LaunchPad         *\r\n");
    printf("*         Command Line Interface         *\r\n");
    printf("*----------------------------------------*\r\n");
    printf("\n<< System Info >>\r\n");
    printf("\tMCU:                     MSP430G2553\r\n");
    printf("\tFLASH:                   16kB\r\n");
    printf("\tFLASH used:              %u\r\n", __m_flash_size);
    printf("\tRAM:                     512B\r\n");
    printf("\tRAM used:                %u\r\n", __m_ram_size);
    printf("\tSystem clock:            1MHz\r\n");
    printf("\tSystem console baudrate: 9600bps\r\n");
    printf("\r\n\r\nMSP430@CLI > ");
}

/**
 *  Get a command from the input string.
 */
static void CLI_GetCommand(unsigned char* cmd)
{
    uint8_t cmd_len;

    for (cmd_len = 0; cmd_len < parameterLength; cmd_len++)
    {
        if ((parameterString[cmd_len] == ' ')
            || (parameterString[cmd_len] == '\0')
            || (parameterString[cmd_len] == 0x0D))
        {
            strncpy((char*)cmd, (char*)parameterString, cmd_len);
            break;
        }
    }
}

/**
 * Command executing: help
 */
static void CLI_Help(void)
{
    uint8_t i;
    /* Print all commands and description for usage */
    printf( "\r\nPlease input command as follows:");

    for (i = 0; i < COMMAND_LEN(command_tbl); i++)
    {
        printf("\r\n\t%s: %s", command_tbl[i].Command, command_tbl[i].Command_Desc);
    }
    printf("\r\n");
}

/**
 * Command executing: hello
 */
static void CLI_Hello(void)
{
    /* Say "Hello, World!"" */
    printf("\r\nHello, World!");
    printf("\r\nI'm Nhi Pham. You'll find me on Earth.");
}

/**
 * Command executing: info
 */
static void CLI_Info(void)
{
    printf("\r\n<< Device Info >>");
    printf("\r\n\tCPU:             msp430g2553 @ 16bit RISC Architecture");
    printf("\r\n\tArchitecture:    16bit RISC Architecture");
    printf("\r\n\tCPU clock:       16MHz");
    printf("\r\n\tFlash:           16kB");
    printf("\r\n\tRAM:             512B");
    printf("\r\n\tI/O:             16 I/O Pins");
    printf("\r\n\tTimer:           Two 16-bit Timer_A");
    printf("\r\n\tADC:             10-bit 200-ksps");
    printf("\r\n\tComunication:    I2C/SPI/UART");
    printf("\r\n\tDebug interface: JTAG + Spy-Bi-wire");
}

/**************************************************
 * Interrupt Handler
 **************************************************/
__attribute__ ((interrupt(UART_RX_VECTOR)))
void UART_RX_ISR(void)
{
    parameterString[parameterLength] = uart_getc();
    uart_putc(parameterString[parameterLength]); /* Echo */

    /* Also get the characters '\r\n' */
    if (parameterString[parameterLength++] == '\r')
    {
        validCommandFlag = true;
    }
}
