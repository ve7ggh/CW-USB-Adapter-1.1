# VE7GGH CW Adapter Firmware v1.3 Release Notes

## Overview

Firmware v1.3 is the stable USB HID release for the **VE7GGH CW Adapter**. It is intended for use with the custom VE7GGH UF2 bootloader on ATSAMD21E18A-based hardware.

This release concentrates on a focused CW paddle interface: it reads Dit and Dah contacts from the TRRS jack and presents them to the host as USB keyboard modifiers. Unlike the separate feature-development release-candidate branch, v1.3 does **not** include USB MIDI, a serial command-line interface, or flash-stored user settings.

## New in v1.3

- Sets the application USB Vendor ID and Product ID to match the custom bootloader:
  - Vendor ID: `0x04D8`
  - Product ID: `0xE417`
- Sets application USB descriptors to:
  - Manufacturer: `VE7GGH`
  - Product: `VE7GGH CW Adapter`
- Performs a TinyUSB detach/attach cycle after applying the USB identity so the host can re-enumerate the device with the intended identifiers.
- Establishes v1.3 as the stable, HID-only firmware line for the CW Adapter.

## HID Paddle Operation

- **Dit** (`PIN_TIP`) sends `Left Ctrl`.
- **Dah** (`PIN_RING1`) sends `Right Ctrl`.
- Pressing both contacts sends both modifier states together.
- Paddle inputs use internal pull-up resistors; a contact is active when pulled LOW.
- The firmware checks that the HID interface is ready before sending keyboard reports.

## Hardware and Visual Feedback

- Supports the ATSAMD21E18A-based VE7GGH CW Adapter hardware.
- Maintains TRS plug compatibility by driving both ground-related jack contacts LOW:
  - `PIN_RING2`
  - `PIN_SLEEVE`
- NeoPixel status colors:
  - Idle: dim blue
  - Dit active: green
  - Dah active: amber
  - Both active: purple
- Provides a white startup confirmation flash after USB mounting.

## USB Identity and Licensing

This release uses the following USB identity:

| Field | Value |
| :--- | :--- |
| Vendor ID | `0x04D8` |
| Product ID | `0xE417` |
| Manufacturer | `VE7GGH` |
| Product | `VE7GGH CW Adapter` |

The VID/PID assignment is used under the project's limited Microchip USB licensing arrangement for SAMD-based products, with a total production limit of **10,000 units**. Distribution and production should remain within the terms of that arrangement.

## Bootloader Compatibility

Firmware v1.3 is intended for the custom VE7GGH UF2 bootloader with the following known configuration:

- Product name: `VE7GGH CW Adapter`
- UF2 volume label: `CWUSBUPDATE`
- Board ID: `SAMD21E18A-CWUSB-v1`
- USB VID/PID: `0x04D8:0xE417`

## Not Included in This Stable Release

The following capabilities are intentionally not part of the v1.3 stable HID firmware:

- USB MIDI paddle output
- Serial CLI configuration commands
- User-configurable MIDI notes or operating modes
- Flash-persistent settings

These remain separate feature-branch capabilities and are not required for standard HID CW keying.

## Upgrade Instructions

1. Connect the CW Adapter to the computer.
2. Double-press the RESET button to enter UF2 bootloader mode.
3. Wait for the `CWUSBUPDATE` drive to appear.
4. Drag and drop the v1.3 firmware `.uf2` file onto the drive.
5. Wait for the device to reboot automatically into the updated firmware.

## Compatibility

- ATSAMD21E18A
- VE7GGH CW Adapter hardware
- Custom VE7GGH UF2 bootloader
- TinyUSB HID keyboard stack
- Windows, Linux, and macOS hosts

## Known Issues

- None currently identified for the HID-only v1.3 release.

Thank you to everyone who tested and provided feedback during development.
