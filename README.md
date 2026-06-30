# <VE7GGH CW USB ADAPTER V1.0>

An open-source USB device based on the Microchip ATSAMD21E18A microcontroller.

This project is a custom hardware design derived from the Adafruit TRRS Trinkey. It features a redesigned PCB created in EasyEDA Pro, custom branding, board-specific firmware, and a modified UF2 bootloader.

The project is intended to be fully reproducible, allowing anyone to study, modify, manufacture, and improve the hardware and software in accordance with the applicable open-source licenses.

> **Note**
> This project is an independent derivative work and is not affiliated with, sponsored by, or endorsed by Adafruit Industries.

-------------------------------------------------------------------------------------------------
What does this device do?
This device is a USB breakout for the ATSAMD21 microcontroller, featuring a 3.5mm TRRS jack. It allows users to connect external switches, sensors, or triggers to a computer via USB.

Because it utilizes Native USB, it can be programmed to act as a:

USB HID Keyboard/Mouse (e.g., for assistive technology switches).
USB MIDI Controller (e.g., for musical triggers).
USB Serial Device (e.g., for data logging and sensor reading).
Unlike a standard USB Audio adapter, this device is designed for custom hardware interaction and DIY electronics projects.
-------------------------------------------------------------------------------------------------

## Features

- Custom ATSAMD21E18A PCB
- Designed using EasyEDA Pro
- UF2 Bootloader
- USB Device Support
- Open Source Hardware
- Open Source Firmware
- Commercial-friendly licensing
- Manufacturing files included

-------------------------------------------------------------------------------------------------
Open Source USB CW Paddle Interface

Designed by VE7GGH

The CW Adapter is a compact USB device that converts a standard CW iambic paddle into a USB Human Interface Device (HID) keyboard. It allows operators to use their favorite paddle with CW training websites, desktop applications, and Android devices without requiring special drivers or proprietary software.

Built around the Microchip SAMD21 microcontroller and using open-source firmware, the CW Adapter is easy to update, modify, and customize for your own operating preferences.

Features
Plug & Play USB HID keyboard device
Works with standard 3.5 mm TRS CW paddles
Compatible with Windows, macOS, Linux, and Android (OTG)
Compatible with Morse Code World, VBand, DitDahDit, and more.
Open-source firmware.
Drag-and-drop UF2 firmware updates.
No drivers required.
Low latency keying.
Supports iambic paddles.
Onboard RGB status LED.
Compact USB stick form factor.
Powered directly from USB.
Canadian-designed open-source project.
--------------------------------------------------------------------------------------------------
Applications
The CW Adapter is ideal for:

Learning Morse code
Practicing CW sending
Online CW training
Portable CW practice
Club training sessions
Contest preparation
Demonstrating CW at hamfests and public events

Un implemented features --- MIDI compatible input for FlexRadio


How It Works
The CW Adapter appears to the host computer as a standard USB keyboard.

When the paddle is pressed:

Dit paddle → USB keyboard output
Dah paddle → USB keyboard output
Because it uses the standard USB HID keyboard protocol, the adapter works with virtually any software that accepts keyboard input for CW operation.

Firmware
The firmware is completely open source.

Updates are performed using the UF2 bootloader:

Enter bootloader mode.
A USB drive appears.
Drag the new .uf2 firmware file onto the drive.
The adapter automatically reboots into the updated firmware.
No programming hardware is required.

Hardware
Microchip ATSAMD21E18A MCU
USB 2.0 Full-Speed
3.5 mm TRS paddle input
RGB NeoPixel status LED
UF2 bootloader
USB powered
Package Options
Complete Unit

CW Adapter
Protective shell
Ready to use
PCB Version

Bare PCB assembly
Ideal for custom enclosures or embedded projects
Open Source
This project is released as open-source hardware and software.

Contributions, improvements, and community modifications are encouraged.

Author
VE7GGH

Open Source Canadian Ham Project

73!

------------------------------------------------------------------------------------------------

## Hardware

The hardware is based on the Adafruit TRRS Trinkey open-source hardware design and has been substantially modified.

Changes include:

- New PCB layout
- EasyEDA Pro project
- Custom silkscreen
- Custom routing
- Board-specific hardware changes
- Removal of Adafruit branding
- Project-specific improvements

The repository contains:

- EasyEDA Pro source files
- Gerber files
- Bill of Materials (BOM)
- Pick-and-Place (CPL)
- PDF schematics
- PCB documentation

See:

```
hardware/
```

---

## Bootloader

This project uses a modified version of the Adafruit UF2 SAMDx1 Bootloader.

Features include:

- UF2 drag-and-drop firmware updates
- USB Mass Storage Device
- USB Serial (CDC)
- Double-tap reset support
- Board identification

See:

```
bootloader/
```

---

## Firmware

The firmware contains the application code for this hardware platform.

Depending on the project, it may include:

- USB HID
- USB Serial
- Custom device functionality
- LED control
- Button handling
- Application logic

See:

```
firmware/
```

---

## Building

### Hardware

Open the EasyEDA Pro project located in:

```text
hardware/EasyEDA_Source/
```

Generate manufacturing files or use the included Gerbers.

### Bootloader

Compile using the Adafruit UF2 SAMDx1 build environment.

See:

```text
bootloader/README.md
```

### Firmware

Compile using:

- Arduino IDE
- PlatformIO

Upload using UF2 or SWD.

---

## Programming

Firmware can be updated by:

1. Connecting the device via USB.
2. Double-tapping the Reset button.
3. Copying the generated `.uf2` file onto the UF2 drive.

---

## Manufacturing

Included production files:

- Gerbers
- Drill files
- BOM
- Pick-and-Place (CPL)

These files are suitable for PCB fabrication and assembly.

---

## Licensing

This repository contains multiple independently licensed components.