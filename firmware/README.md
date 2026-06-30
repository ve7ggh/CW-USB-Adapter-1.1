# Firmware

This directory contains the application firmware for the <VE7GGH CW USB ADAPTER>.

## Overview
This firmware is written for the ATSAMD21E18 microcontroller. It is designed to be compiled using the Arduino IDE or PlatformIO and uploaded via the UF2 bootloader.

## Dependencies
This firmware relies on the following open-source libraries:
- **Adafruit TinyUSB Library** (MIT License)
- **Adafruit NeoPixel Library** (MIT License)
- [Any other libraries you used, e.g., Arduino HID, etc.]

## Installation
1.  Connect your device to your computer via USB.
2.  Double-tap the Reset button to enter Bootloader mode.
3.  A drive named `<YOUR_DRIVE_NAME>` will appear.
4.  Compile this code in the Arduino IDE (ensure you have the SAMD Core installed).
5.  Upload the `.bin` or `.uf2` file to the device.

## Configuration
Pins used in this firmware:
- `PIN_NEOPIXEL`: PAxx
- `PIN_SWITCH_1`: PAxx
- Microcontroller Pin Mapping

-----------------------------------------------------------------------------------------------------------
The device utilizes the following GPIO mappings for the 3.5 mm TRS jack and onboard peripherals:

3.5 mm Jack (Input/Output)
Function	Physical Contact	GPIO Pin (Arduino)	MCU Pin (SAMD21)
DIT	Tip	PIN_TIP (0)	PA04
DAH	Ring 1	PIN_RING1 (2)	PA09
GND (Comp)	Ring 2	PIN_RING2 (4)	PA08
GND (Main)	Sleeve	PIN_SLEEVE (5)	PA05
Note: For maximum compatibility with TRS (3-conductor) cables, the firmware drives both Ring 2 and Sleeve LOW to act as a solid Ground common.

Onboard Peripherals
Peripheral	Function	GPIO Pin (Arduino)	MCU Pin (SAMD21)
NeoPixel	RGB Status LED	PIN_NEOPIXEL (11)	PA01
Internal USB	D- / D+	—	PA24 / PA25
Firmware Logic Mapping
The software assigns the following HID (Keyboard) modifiers to the physical paddle contacts:

DIT (Tip): Triggers KEYBOARD_MODIFIER_LEFTCTRL
DAH (Ring 1): Triggers KEYBOARD_MODIFIER_RIGHTCTRL
LED Color Indicators
State	LED Color
Idle	Dim Blue
Dit Pressed	Green
Dah Pressed	Amber
Squeeze (Both)	Purple
Bootloader Mode	Oscillating Green

## Commercial Use
This firmware is provided under the MIT License. You are free to use, modify, and distribute this software in both open-source and commercial applications, provided the original copyright notice is retained.

## License
Licensed under the MIT License. See `LICENSE-FIRMWARE` for details.