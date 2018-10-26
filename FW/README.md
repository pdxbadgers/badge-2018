# badge firmware

## Overview
The badge is based on an ATTiny861, which is very similar to the much more common ATTiny85 but has lots more GPIO pins.

The arduino framework is implemented as part of ATTinyCore, which natively supports the chip. Right now, you'll still need a dedicated programmer to use ATTinyCore, like and Arduino-as-ISP or a USBASP.

The bootloader is micronucleus, which is a software-based USB bootloader that supports many ATTiny chips. The configuration for the ATTiny861 is currently in the forked submodule, but should be pulled into mainline once we make sure it's stable.

## Sample programs

boardtest contains a script that will cycle through all of the LEDs and each of the PWM leds

RGB_PWM_test contains a script that...


## Coding on your badge
First, you need to install Arduino and the ATTinyCore board files. See their [instructions](https://github.com/SpenceKonde/ATTinyCore/blob/master/Installation.md).

Next, you need to configure your arduino settings properly under the "tools" menu:
* Board: ATTiny261/461/861
* Save EEPROM: EEPROM Retained
* Timer 1 Clock: CPU
* LTO: Disabled
* BOD: Disabled
* Chip: ATTiny861
* Clock: 16MHz Internal

In the meantime, you'll need to use an external programmer - like the Arduino as ISP or USBASP - until we get micronucleus built into the arduino board files. 

### Pin mappings

![x61 pin mapping](http://drazzy.com/e/img/PinoutT861a.jpg "Arduino Pin Mapping for ATtiny x61 series")

The ATTiny861 has 20 pins - 4 for power/ground and 16 I/O pins. Be careful, each pin has multiple names:
* The package pin number (1 to 20)
* The AVR GPIO name (PA0-7 and PB0-7)
* The Arduino digital number (0-14)
* The Arduino analog pin number (A0-10)

For the most part, you want the Arduino digital number in your code.

Here's the pin names as they're used on the badge. Someone really ought to make this into a header file.

```c
// ATtiny861 outputs
const int ROW1 = 0; // PA0
const int ROW2 = 1; // PA1
const int ROW3 = 2; // PA2
const int ROW4 = 14; // PA3

const int EN_RGB1 = 10; // PA4
const int EN_RGB2 = 11; // PA5
const int EN_RGB3 = 12; // PA6
const int EN_RGB4 = 13; // PA7
const int R_PWM = 8; // PB1
const int G_PWM = 6; // PB3
const int B_PWM = 4; // PB5

const int SAO_SDA = 9; // PB0
const int SAO_SCL = 7; // PB2

const int SPI_MOSI = 9; // PB0
const int SPI_MISO = 8; // PB1
const int SPI_SCK = 7; // PB2

const int DM = 5; // PB4
const int DP = 4; // PB5

const int L_SWITCH = 5; // PB4
const int R_SWITCH = 3; // PB6

const int RESET = 15; // PB7
```

### Flashing
To use the USB bootloader, you need a separate program, micronucleus. Someone really ought to add it to the Arduino IDE, but in the meantime, you can [download/build it yourself](https://github.com/micronucleus/micronucleus/tree/master/commandline)

Once you have it, you need a .hex file from arduiono:
1. Check the box under "File->Preferences->Show verbose output during upload"
2. Press 'play' to flash your code. It'll throw an error, but give you the command line it tried to use to flash, something like 'avrdude -C/opt/arduino-1.8.6/hardware/tools/avr/etc/avrdude.conf -v -pattiny861 -carduino -P/dev/ttyACM0 -b19200 -Uflash:w:/tmp/arduino_build_60862/Blink.ino.hex:i'
3. Run micronucleus with the hex file from the string above 'micronucleus /tmp/arduino_build_60862/Blink.ino.hex'
4. Hold down the S2 button and plug the USB cable into your badge. 

## Building and flashing the bootloader
If you want to get fancy you can build and flash your own bootloader too. Look in the micronucleus submodule. 'firmware' contains the bootloader code, 'commandline' contains the programming utility's code.

1. cd micronucleus/firmware
2. Make changes you want to the files in 'configuration/t861_default/'
3. make clean
4. make CONFIG=t861_default
5. flash it to your badge with a command like 'avrdude -carduino -P/dev/ttyACM0 -pt861 -e -Uflash:w:main.hex -U lfuse:w:0xe1:m -U hfuse:w:0xdd:m -U efuse:w:0xfe:m'

## flashing with micronucleous
1. cd micronucleous/commandline
2. start micronucleous in a loop like ```for i in $(seq 1 100); do echo $i; ./micronucleus ../../button_test/button_test.hex ; if [ $? -eq 0 ] ; then break; fi; done```
3. hold down button s1 on badge
4. plug badge in
