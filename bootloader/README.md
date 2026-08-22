# Bootloader

## Overview

This directory contains a customized version of the **Adafruit UF2 SAMDx1 Bootloader** specifically configured for the **VE7GGH CW USB Adapter**.

The UF2 bootloader provides a robust system for firmware updates, including:
- **Drag-and-Drop Updates**: Appears as a USB Mass Storage drive for easy .uf2 file deployment.
- **Resiliency**: Support for double-tap reset to enter bootloader mode.
- **Board Identification**: Correct USB descriptors and branding for this hardware.
- **Native USB**: No external hardware programmers (like J-Link) are required for standard updates.

## Upstream Project

This bootloader is a derivative work of the Adafruit UF2 SAMDx1 project:
[https://github.com/adafruit/uf2-samdx1](https://github.com/adafruit/uf2-samdx1)

We gratefully acknowledge the work of Microsoft Corporation, Adafruit Industries, and the community contributors who maintain the UF2 project.

## Project Modifications

This version includes board-specific changes for the CW USB Adapter hardware including:
- Custom Pin mappings and LED behaviors.
- Board-specific USB VID/PID and Strings.
- Optimized build configurations for the ATSAMD21E18A.
- USB Architecture & Microchip Sublicensed Identity
- USB Vendor ID (VID): 0x04D8 (Microchip Technology Inc. sublicensed tier)
- USB Product ID (PID): 0xE417
- Product Name: VE7GGH CW Adapter
- Manufacturer: VE7GGH

## Licensing

This project is open-source. Due to the nature of embedded bootloaders, it incorporates code under several licenses.

- **This Derivative Work**: Licensed under the **MIT License**. See [LICENSE.md](LICENSE.md).
- **Original UF2 Code**: Licensed by Microsoft and Adafruit under the **MIT License**. See [LICENSE-MIT.txt](LICENSE-MIT.txt).
- **Third-Party Components**: Includes hardware-layer code from Atmel (Microchip) and ARM. Original licenses and notices are preserved in [THIRD-PARTY-NOTICES.txt](THIRD-PARTY-NOTICES.txt).

## Building and Flashing

For instructions on how to compile this bootloader and flash it to bare-metal hardware via SWD, please refer to the documentation in the `/Documentation` directory or the original upstream repository.

## Disclaimer

This project is an independent work and is not affiliated with, sponsored by, or endorsed by Adafruit Industries.