#----------------------------------------#
#-- Makefile for MSP430G2 Launchpad    --#
#-- Created by Nhi Van Pham @ 2018     --#
#----------------------------------------#

SRCS      := $(wildcard src/*.c)
OBJECTS   := $(subst src,build,$(SRCS:.c=.o))
DEPS      := $(subst src,build,$(SRCS:.c=.d))
BUILD_DIR := build
SRC_DIR   := src
TARGET    := cli_main

# The support files consist of linker script (*.ld), include header files and devices.csv.
SUPPORT_FILE_DIRECTORY := ~/tools/msp430-elf

# Definition of compiler toolchain
DEVICE  := msp430g2553
CC      := msp430-elf-gcc
LD      := msp430-elf-gcc
GDB     := msp430-elf-gdb
FLASH   := mspdebug

#  Options for C Compiler
# Details:
#      -std=c99 -Wpedantic: Issue all the warnings demanded by strict ISO C; reject all programs that use forbidden extensions, and some other programs that do not follow ISO C.
#                           For ISO C, follows the version of the ISO C standard specified by any -std option used.
#      -Wshadow: Warn whenever a local variable or type declaration shadows another variable,
#                parameter, type, or whenever a built-in function is shadowed.
#      -Wextra: This enables some extra warning flags that are not enabled by -Wall.
#      -Werror: Make all warnings into errors.
#      -ggdb:
#      -gdwarf-2: Source-level debugging (this is the specified DWARF standard) that use a compatible debugger to load, run, and debug images.
#                  For more information --> http://www.keil.com/support/man/docs/armclang_ref/armclang_ref_chr1409753002958.htm
#      --gc-sections: Remove unused sections
CFLAGS =
CFLAGS += -I $(SUPPORT_FILE_DIRECTORY)/include -I ./inc -L $(SUPPORT_FILE_DIRECTORY)/include -mmcu=$(DEVICE)
CFLAGS += -O0 -g -ggdb -gdwarf-2
CFLAGS += -Wall -Wextra -Wshadow -std=c99 -Wpedantic

#  Options for Linker
#  Details:
#     -nostartfiles: Do not use the standard system startup files when linking. The standard system libraries are used normally
LFLAGS =
LFLAGS += -L $(SUPPORT_FILE_DIRECTORY)/include -I $(SUPPORT_FILE_DIRECTORY)/include
LFLAGS += -T msp430g2553.ld -mmcu=$(DEVICE)
LFLAGS += -Wl,-Map=$(BUILD_DIR)/$(TARGET).map

compile: $(TARGET)

all: $(BUILD_DIR)/$(TARGET) upload

$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	$(LD) $(LFLAGS) $(OBJECTS) -o $(@).elf
	msp430-elf-objdump -Sd -W $(@).elf > $(@).lss
	msp430-elf-size $(@).elf
	msp430-elf-objcopy -O ihex $(@).elf $(@).hex

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -MD -MF $(subst src,build,$(subst .c,.d,$<)) $< -c -o $@

# MSP430G2 Launchpad integrated with the eZ430-RF2500 USB Debugging Interface
# Options:
#       --force-reset:  When using a FET device, always send a reset during initialization.
#                       By default, an initialization without reset will be tried first.
#       rf2500:         Connect to an eZ430-RF2500, Launchpad or Chronos device. Only USB connection is supported.
# For more information: http://manpages.ubuntu.com/manpages/xenial/man1/mspdebug.1.html
upload:
	$(FLASH) --force-reset rf2500 "prog $(BUILD_DIR)/$(TARGET).elf"

debug:
	clear
	@echo -e "--------------------------------------------------------------------------------"
	@echo -e "-- Make sure you are running mspdebug in another window                       --"
	@echo -e "--------------------------------------------------------------------------------"
	@echo -e "$$ # you can start it like this:"
	@echo -e "$$ mspdebug rf2500 gdb\n"
	$(GDB) $(BUILD_DIR)/$(TARGET).elf

clean:
	rm -rf build/*

.PHONY: clean upload debug compile
