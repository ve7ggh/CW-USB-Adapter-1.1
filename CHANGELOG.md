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