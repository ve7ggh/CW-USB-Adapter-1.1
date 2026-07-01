# VE7GGH CW Adapter Firmware v1.1 Release Notes

Release Date: June 2026

## Overview

Firmware v1.1 is the first public release intended for use with the custom VE7GGH UF2 bootloader. This release focuses on improving usability, branding, and deployment while maintaining compatibility with the ATSAMD21E18A hardware platform.

## New Features

- Compatible with the custom VE7GGH UF2 bootloader.
- Supports drag-and-drop firmware updates using the UF2 bootloader.
- Full USB HID keyboard operation.
- Updated device branding to "VE7GGH CW Adapter."

## Improvements

- Improved compatibility with the custom bootloader.
- Updated project documentation.
- Standardized firmware build process.
- Improved support for future firmware updates through the UF2 bootloader.

## Bootloader Compatibility

This firmware is intended for use with the VE7GGH custom bootloader featuring:

- Product Name: VE7GGH CW Adapter
- UF2 Volume Label: CWUSBUPDATE
- Board ID: SAMD21E18A-CWUSB-v1

## Fixed

- Improved reliability of firmware deployment using UF2.
- Updated project identification strings.
- Corrected project URL embedded in bootloader metadata.

## Known Issues

- None currently identified.

## Upgrade Instructions

1. Connect the CW Adapter to the computer.
2. Double-press the RESET button to enter UF2 bootloader mode.
3. Wait for the `CWUSBUPDATE` drive to appear.
4. Drag and drop the new firmware `.uf2` file onto the drive.
5. The device will automatically reboot into the updated firmware.

## Compatibility

- ATSAMD21E18A
- Custom VE7GGH CW Adapter hardware
- Custom VE7GGH UF2 Bootloader
- Windows, Linux, and macOS

Thank you to everyone who tested and provided feedback during development.