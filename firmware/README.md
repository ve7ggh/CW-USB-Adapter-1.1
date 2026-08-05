# Firmware <!-- omit from toc -->

- [Overview](#overview)
- [Features](#features)
- [Dependencies](#dependencies)
- [Building](#building)
  - [Requirements](#requirements)
  - [Compile](#compile)
  - [Firmware Update](#firmware-update)
- [Hardware Interface](#hardware-interface)
  - [3.5 mm TRRS Connector](#35-mm-trrs-connector)
  - [Onboard Peripherals](#onboard-peripherals)
  - [Firmware Operation](#firmware-operation)
    - [USB HID Mapping](#usb-hid-mapping)
    - [NeoPixel Status](#neopixel-status)
- [Repository Contents](#repository-contents)
- [License](#license)


## Overview

This directory contains the application firmware for the **VE7GGH CW USB Adapter**.

The firmware is written for the Microchip ATSAMD21E18A microcontroller and transforms the device into a USB Human Interface Device (HID) keyboard for use with CW iambic paddles.

When a paddle contact is closed:

- Dit → USB Keyboard Left Ctrl
- Dah → USB Keyboard Right Ctrl

This allows the adapter to work with a wide variety of Morse code training software and amateur radio applications without requiring custom drivers.

The firmware is designed to be compiled using the Arduino IDE or PlatformIO and uploaded using the UF2 bootloader.

## Features

- USB HID Keyboard device
- Low-latency paddle input
- Iambic paddle support
- NeoPixel RGB status indicator
- Native USB support
- UF2 drag-and-drop firmware updates
- Compatible with Windows, macOS, Linux, and Android (OTG)
- Open-source firmware
- Commercial-friendly MIT licensing

## Dependencies

This firmware uses the following open-source libraries:

- Adafruit TinyUSB Library (MIT License)
- Adafruit NeoPixel Library (MIT License)
- Arduino SAMD Core

## Building

### Requirements

- Arduino IDE 2.x or PlatformIO
- Adafruit SAMD Board Package
- TinyUSB selected as the USB Stack

### Compile

1. Open the firmware project in Arduino IDE.
2. Select the appropriate board.
3. Select **TinyUSB** as the USB Stack.
4. Verify and compile the project.

### Firmware Update

1. Connect the device to your computer.
2. Double-tap the Reset button to enter UF2 Bootloader mode.
3. A USB drive will appear.
4. Copy the compiled `.uf2` firmware file onto the drive.
5. The device will automatically reboot with the updated firmware.

No external programmer is required for normal firmware updates.

## Hardware Interface

### 3.5 mm TRRS Connector

| Function | Contact | Arduino Pin | MCU Pin |
| :-- | :-- | :-- | :-- |
| Dit | Tip | PIN_TIP | PA04 |
| Dah | Ring 1 | PIN_RING1 | PA09 |
| Ground (Compatibility) | Ring 2 | PIN_RING2 | PA08 |
| Ground | Sleeve | PIN_SLEEVE | PA05 |
| For maximum compatibility with standard 3-conductor (TRS) paddle cables, the firmware actively drives both Ring 2 and Sleeve LOW to provide a common ground. |

### Onboard Peripherals

| Peripheral | Arduino Pin | MCU Pin |
| :-- | :-- | :-- |
| NeoPixel RGB LED | PIN_NEOPIXEL | PA01 |
| Native USB | — | PA24 / PA25 |

###  Firmware Operation

#### USB HID Mapping
| Paddle | USB Output |
| :-- | :-- |
| Dit | Left Ctrl |
| Dah | Right Ctrl |

#### NeoPixel Status

| State | LED Color |
| :-- | :-- |
| Idle | Dim Blue |
| Dit Pressed | Green |
| Dah Pressed | Amber |
| Both Paddles Pressed | Purple |
| Bootloader Mode | Green (UF2 Bootloader) |

## Repository Contents

This directory typically contains:

- Arduino source code
- Project configuration files
- Firmware documentation
- Release firmware (.uf2)
- Commercial Use
- This firmware is licensed under the MIT License.

You are free to:

- Use
- Modify
- Redistribute
- Manufacture products
- Sell products
- Create derivative works provided the original copyright notice and license are retained.

## License

The firmware contained in this directory is licensed under the MIT License.

See:

LICENSE — Firmware License
Author
VE7GGH

Open Source Canadian Amateur Radio Project

73!