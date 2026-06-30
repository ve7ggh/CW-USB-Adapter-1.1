# Building the Custom UF2 Bootloader

This document describes how to build the custom UF2 bootloader for the VE7GGH CW Adapter, based on the Adafruit `uf2-samdx1` bootloader (v4.0.0).

## Overview

A custom bootloader was created to provide

- Custom USB product name VE7GGH CW Adapter
- Custom UF2 drive label CWUSBUPDATE
- Custom board identification
- Custom GitHub support URL
- Native UF2 drag-and-drop firmware updates
- Full compatibility with the ATSAMD21E18A MCU

The hardware is electrically identical to the Adafruit TRRS Trinkey M0.

---

# Development Environment

Operating System

- Linux Mint

Compiler

- arm-none-eabi-gcc 13.2.1

Source

- Adafruit `uf2-samdx1`
- Version v4.0.0

---

# Installing Build Tools

```bash
sudo apt update
sudo apt install git make python3 gcc-arm-none-eabi binutils-arm-none-eabi
```

Verify the compiler

```bash
arm-none-eabi-gcc --version
```

---

# Download the Source

```bash
git clone --branch v4.0.0 httpsgithub.comadafruituf2-samdx1
cd uf2-samdx1
git submodule update --init --recursive
```

---

# Create the Custom Board

Copy the TRRS Trinkey board configuration.

```bash
cp -r boardstrrs_trinkey_m0 boardsve7ggh_cw_adapter
```

Edit

```
boardsve7ggh_cw_adapterboard_config.h
```

Update the branding information.

Example

```c
#define VENDOR_NAME    VE7GGH
#define PRODUCT_NAME   VE7GGH CW Adapter
#define VOLUME_LABEL   CWUSBUPDATE
#define INDEX_URL      httpsgithub.comve7gghCWUSB-Adapter
#define BOARD_ID       SAMD21E18A-CWUSB-v1
```

---

# board.mk

The custom board uses the original board definition.

```
CHIP_FAMILY = samd21
CHIP_VARIANT = SAMD21E18A
```

---

# Source Code Patch

The standard Adafruit source hard-codes several optional features.

To allow these features to be disabled from the build system, the following
definitions in `incuf2.h` were modified.

Change

```c
#define USE_WEBUSB 1
```

to

```c
#ifndef USE_WEBUSB
#define USE_WEBUSB 1
#endif
```

Change

```c
#define USE_HID_EXT 1
```

to

```c
#ifndef USE_HID_EXT
#define USE_HID_EXT 1
#endif
```

Change

```c
#define USE_HID_HANDOVER 1
```

to

```c
#ifndef USE_HID_HANDOVER
#define USE_HID_HANDOVER 1
#endif
```

Change

```c
#define USE_MSC_HANDOVER 1
```

to

```c
#ifndef USE_MSC_HANDOVER
#define USE_MSC_HANDOVER 1
#endif
```

These changes allow compiler command-line overrides.

---

# Makefile.user

Create a file named

```
Makefile.user
```

Contents

```make
BOARD=ve7ggh_cw_adapter

BLD_EXTA_FLAGS=-DUSE_WEBUSB=0 
               -DUSE_HID_EXT=0 
               -DUSE_HID_HANDOVER=0 
               -DUSE_MSC_HANDOVER=0
```

This disables optional bootloader features that are not required by this
project while preserving normal UF2 functionality.

No Link Time Optimization (LTO) is used.

---

# Building

Compile using

```bash
make clean
make
```

A successful build reports approximately

```
Space left 460
```

---

# Output Files

Compiled files are located in

```
buildve7ggh_cw_adapter
```

Important files

```
bootloader-ve7ggh_cw_adapter-v4.0.0+.bin
```

Used to flash a blank ATSAMD21E18A using SWD.

```
update-bootloader-ve7ggh_cw_adapter-v4.0.0+.uf2
```

Used to update an existing UF2 bootloader.

---

# Verifying the Build

Check the embedded strings

```bash
strings buildve7ggh_cw_adapterbootloader-ve7ggh_cw_adapter-v4.0.0+.bin  
grep -E VE7GGHCWUSBgithub
```

Expected output includes

```
VE7GGH
VE7GGH CW Adapter
CWUSBUPDATE
Model VE7GGH CW Adapter
Board-ID SAMD21E18A-CWUSB-v1
httpsgithub.comve7gghCWUSB-Adapter
```

---

# Programming

Program the `.bin` file using an Atmel-ICE (or compatible SWD programmer).

Recommended sequence

1. Set `NVMCTRL_BOOTPROT` to 0 Bytes
2. Perform Chip Erase
3. Program the bootloader
4. Verify
5. Set `NVMCTRL_BOOTPROT` to 8192 Bytes
6. Power cycle the board

---

# USB Enumeration

After programming

The board should enumerate as

```
VE7GGH CW Adapter
```

The UF2 drive should appear as

```
CWUSBUPDATE
```

The `INFO_UF2.TXT` file contains

- Model name
- Board ID
- GitHub project URL

---

# Design Notes

The original attempt used GCC Link Time Optimization (`-flto`) to reduce code
size enough to fit inside the 8 KB bootloader region.

Although this produced a binary that fit, USB enumeration became unreliable.

The final solution was to disable several optional bootloader features instead
of relying on LTO.

Disabled features

- WebUSB
- Extended HID commands
- HID handover
- MSC handover

This reduced the bootloader size sufficiently while maintaining reliable USB
operation and UF2 drag-and-drop firmware updates.

The HID keyboard functionality of the application firmware is completely
unaffected because these changes apply only to the bootloader.

---

# Repository

Project

httpsgithub.comve7gghCWUSB-Adapter

Bootloader Base

httpsgithub.comadafruituf2-samdx1

Version

v4.0.0