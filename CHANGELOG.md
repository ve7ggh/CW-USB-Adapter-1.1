# Changelog

All notable changes to the VE7GGH CW Adapter project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

---

## [1.0.0] - 2026-06

### Added

- Initial public release of the VE7GGH CW Adapter project.
- Custom UF2 bootloader based on Adafruit `uf2-samdx1` v4.0.0.
- Support for the ATSAMD21E18A microcontroller.
- Custom board definition `ve7ggh_cw_adapter`.
- Custom USB product name:
  - `VE7GGH CW Adapter`
- Custom UF2 drive label:
  - `CWUSBUPDATE`
- Custom board identifier:
  - `SAMD21E18A-CWUSB-v1`
- Custom `INFO_UF2.TXT` containing the project GitHub URL.
- Linux Mint build environment documentation.
- Complete build instructions.
- SWD programming procedure.
- GitHub repository documentation.

### Changed

- Created a dedicated board configuration derived from the Adafruit TRRS Trinkey M0.
- Updated bootloader branding from Adafruit to VE7GGH.
- Replaced default USB identification strings with project-specific branding.
- Updated repository URL embedded within the bootloader.

### Fixed

- Corrected FAT volume label length to comply with the FAT filesystem's 11-character maximum.
- Resolved bootloader size overflow beyond the 8 KB bootloader region.
- Eliminated unreliable USB enumeration caused by GCC Link Time Optimization (`-flto`).
- Restored reliable USB enumeration by disabling optional bootloader features instead of using LTO.
- Successfully compiled within the 8192-byte bootloader limit with approximately 460 bytes remaining.

### Technical

Modified `inc/uf2.h` to allow build-time feature overrides using compiler definitions.

Wrapped the following feature definitions with `#ifndef` guards:

- `USE_WEBUSB`
- `USE_HID_EXT`
- `USE_HID_HANDOVER`
- `USE_MSC_HANDOVER`

Added build-time overrides through `Makefile.user`:

- Disabled WebUSB.
- Disabled HID Extended commands.
- Disabled HID handover.
- Disabled MSC handover.

These changes reduce code size while preserving:

- UF2 drag-and-drop firmware updates.
- USB Mass Storage support.
- Stable USB enumeration.

### Verified

Successfully tested:

- Linux Mint build environment.
- GCC ARM Embedded 13.2.1.
- Atmel-ICE programming via SWD.
- Bootloader flashing and verification.
- USB enumeration under Windows.
- Custom USB product name.
- Custom UF2 volume label.
- Custom `INFO_UF2.TXT` contents.
- Protection of the bootloader region using `NVMCTRL_BOOTPROT`.

---

## Future

Planned improvements include:

- Assignment of a dedicated USB VID/PID.
- Additional MIDI control to be compatible with FlexRadio controls.
- Automated patching of `uf2.h` during the build process.
- Continuous Integration (CI) builds using GitHub Actions.
- Automated release generation with pre-built bootloader binaries.
- Additional firmware examples and documentation.

## Release Candidate 1.2 July 5, 2026
[1.2 RC2] — 2026-07-05
Added
USB MIDI interface — device now enumerates as both a USB HID Keyboard and a USB MIDI device simultaneously (no drivers required)
MIDI Note On/Off for Dit and Dah paddle inputs (default: Dit = Note 20, Dah = Note 21)
Three operating modes: HID Only, MIDI Only, HID + MIDI (Both)
Flash persistence via FlashStorage_SAMD — operating mode and MIDI note assignments survive power cycles
Serial CLI over USB virtual COM port with the following commands:
help — list available commands
status — show current mode, MIDI notes, and live paddle pin states
mode [hid|midi|both] — change operating mode at runtime
midi_notes — display current MIDI note assignments
set_notes <dit> <dah> — reassign MIDI notes (0–127)
save — persist current settings to flash
reset — restore factory defaults and save
USB device descriptors — manufacturer set to VE7GGH, product set to CW USB Adapter
Startup double-flash (white) on NeoPixel to confirm successful boot
Serial welcome banner printed on first Serial connection
Full status dump printed to Serial on boot
Changed
Loop polling delay reduced from 10 ms to 5 ms for improved responsiveness
HID ready() check replaced with non-blocking logic
NeoPixel color macros replaced with inline pixel.Color() calls to avoid macro expansion conflicts
Code refactored into modular functions with explicit forward declarations
FlashStorage_SAMD read() call corrected to pass struct by reference (required by library v1.3.2+)
Dependencies Added
FlashStorage_SAMD v1.3.2 or later
Compatible Software
HID mode: Morse Code World, VBand, DitDahDit, any CW keyboard app
MIDI mode: FlexRadio / NetKeyer, any MIDI-capable CW application