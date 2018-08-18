# msp430-cli

A Command Line Interface (CLI) for MSP430G2 LaunchPad

## Environment

* PC:        Kubuntu-18.04
* Compiler:  msp430-elf-gcc
* Flasher:   mspdebug
* IDE:       None
* Build:     Makefile
* Serial Console: screen

## How to compile and upload the program

To compile and load program into the board, you need to install some requisite tools such as: *msp430-elf-gcc*, *mspdebug* and *make*.
After above tools have already installed completely, you perform the guide as follows:

* Compile and upload the program on MCU

  ```shell
  make all
  ```

* Debug (if needed)

  * Start gdb server with port 2000

    ```shell
    sudo mspdebug rf2500
    ```

  * Another windows, start gdb client

    ```shell
    make debug
    ```

  * Finally, connect to gdb server and debug.

    ```shell
    (gdb) target remote localhost:2000
    (gdb) load build/cli_main.elf
    (gdb) continue
    ```

* Clean the project

  ```shell
  make clean
  ```

* Using the *screen* for serial interface

  ```shell
  sudo screen /dev/ttyACM0 -b 9600
  ```

## Screenshot

![MSP430-CLI](https://raw.githubusercontent.com/nhivp/msp430-cli/master/docs/imgs/msp430-cli.png "MSP430-CLI")

## Useful links

* [A list of embedded command-line-interface projects](http://www.dalescott.net/an-embedded-command-line-interface/)

Feel free to create pull requests and I will gladly accept them!

Good luck, and happy hacking?! :yum:
