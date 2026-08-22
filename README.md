# VE7GGH CW USB Adapter <!-- omit from toc -->

- [Instructions on How to Use](#instructions-on-how-to-use)
  - [Practice Software](#practice-software)
- [Project Structure](#project-structure)
- [Hardware](#hardware)
- [Firmware](#firmware)
- [Bootloader](#bootloader)
- [Building](#building)
  - [Hardware](#hardware-1)
  - [Firmware](#firmware-1)
  - [Bootloader](#bootloader-1)
- [Updating Firmware](#updating-firmware)
- [Licensing](#licensing)
- [Acknowledgements](#acknowledgements)
- [Author](#author)

An open-source USB CW paddle interface based on the Microchip ATSAMD21E18A microcontroller.

The VE7GGH CW USB Adapter converts a standard Morse code paddle or straight key into a USB Human Interface Device (HID) keyboard, allowing it to work with a wide variety of amateur radio applications, Morse code trainers, and operating systems without requiring proprietary drivers.

This project is a complete open-source hardware and software design, including custom hardware, open-source firmware, a modified UF2 bootloader, manufacturing files, and documentation.

> **NOTE**: This project is an independent derivative work based on the Adafruit TRRS Trinkey open-source hardware and software ecosystem and is not affiliated with or endorsed by Adafruit Industries.
            - In honour of them this projects Bootloader and Firmware v1.x works on their hardware for half price, however if you want to support a HAM to continue inventing radio related open source devices, then I thank you for your support, it means so much to me.
            -Small batches of working v1.x are being produced. 			
             
			I am a beginner when it comes to development, all this was new to me 6 months ago, please accept there may be errors or conflicting information in the heavy documentation, I am learning and struggling with my own design.
			As development continues I will learn to organize as best as possible and follow version control to make things clear for the beginner to follow and contribute.
			
			Version 1.x series is proof of design utilizing 3D printed resin shell and simple updates to solve the issue of cheap chinese adapters being a waste of time and money.
				-Features added beyond basic paddle control for online/app training of CW.
                -Addded MIDI capabilities to work with FLEX Radio
				-Sublicense from Microchip inc for USB HID/VID limited to 10K units.
            Version 2.x series will change to injection molded case and be larger like USB thumb drive including protective cap and lanyard attachment.
                -Larger board for ease of manufacturing, adding improvements and reducing cost.
                -Color choices of shell, grey or transparent.
                -Switching 3.5mm jack to mid mount TRS.
                -ESD protections for microcontroller.				

---

## Instructions on How to Use

The CW USB Adapter works with Morse Code **Paddles** that use a TRS 3.5mm cable. The adapter is a plug-and-play device - no drivers need to be installed.
Wiring follows the standard: Tip (DIT), Ring (DAH), Sleeve (Ground)
Once you plug in the adapter, the paddle works like a keyboard:

* **Dits** are the Left Control (`L-Ctrl`) key
* **Dahs** are the Right Control (`R-Ctrl`) key

### Practice Software

The CW USB adapter can be used with (but is not limited to) the following web applications:

* [MoreCode.World Keyer](https://morsecode.world/international/trainer/keyer.html)
* [DiDahDit](https://didahdit.com/)
* [VBand](https://hamradio.solutions/vband/)

---

## Project Structure

| Directory | Description |
|:---|:---|
| `/hardware` | PCB design, schematics, Gerbers, BOM, EasyEDA source |
| `/firmware` | UF2 Application firmware |
| `/bootloader` | UF2 bootloader |
| `/files` | 3D models, mechanical drawings and supporting files |

Detailed documentation is available in the README contained within each directory.

---

## Hardware

The hardware is based on the Adafruit TRRS Trinkey PCB and has been substantially redesigned.

Included are:

- EasyEDA Pro source
- Schematics
- PCB layout
- Gerbers
- BOM
- Pick-and-place files

See: [hardware/README.md](hardware/README.md)

---

## Firmware

The firmware converts paddle closures into USB HID keyboard events while providing visual feedback using the onboard RGB Led.

Features include:
- UF2 drag and drop firmware (for easy updates)
- USB HID Keyboard (simulates the device) CTRL LEFT/CTRL RIGHT
- Iambic paddle support
- NeoPixel RGB LED indicator
- Low-latency operation

See: [firmware/README.md](firmware/README.md)

---

## Bootloader

The project uses a customized version of the Adafruit UF2 SAMDx1 Bootloader.

See: [bootloader/README.md](bootloader/README.md)

---

## Building

### Hardware

Manufacture the PCB using the supplied Gerber files or open the EasyEDA Pro project for modifications.

### Firmware

Compile using:

- Arduino IDE
- PlatformIO

### Bootloader

Compile using the Adafruit UF2 SAMDx1 build environment.

---

## Updating Firmware

1. Connect the adapter via USB.
2. Double-tap the onboard Reset button.
3. The UF2 drive will appear on your computer.
4. Drag the new .uf2 firmware file onto the drive.
5. The device automatically reboots into the updated firmware.

---

## Licensing

This repository contains multiple independently licensed components.

| Component | License |
|:---|:---|
| Firmware | MIT |
| Hardware | CC BY-SA 3.0 Unported |
| Bootloader | MIT with preserved third-party notices |

See the LICENSE file within each directory for complete licensing information.

Third-party acknowledgements are contained in NOTICE.md.

---

## Acknowledgements

This project builds upon the excellent open-source work of:

- Adafruit Industries
- Microsoft
- Microchip Technology
- ARM
- Arduino
- TinyUSB contributors

Their commitment to open source has made this project possible.

---

## Author

**VE7GGH**

Open Source Canadian Amateur Radio Project

73!
