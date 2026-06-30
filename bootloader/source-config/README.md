Custom UF2 Bootloader Implementation
For this project, I have implemented a custom, branded UF2 bootloader based on the adafruit/uf2-samdx1 (v4.0.0) source. This allows for seamless "drag-and-drop" firmware updates via a USB mass storage drive, tailored specifically for the VE7GGH CW Adapter hardware.

Key Accomplishments:
Custom Branding: Fully localized the bootloader identity. When in bootloader mode, the device identifies as "VE7GGH CW Adapter" (Model) and mounts a drive named "CWUSBUPDATE". The INFO_UF2.TXT file includes a direct link to this GitHub repository.
Hardware Compatibility: Configured for the SAMD21E18A MCU in a crystal-less internal oscillator configuration, ensuring high reliability on custom PCB layouts without external clock requirements.
Code-Size Optimization: To fit the full feature set within the strict 8KB bootloader protection region, I performed manual code-squeezing by disabling non-essential bootloader features (WebUSB, HID-extension, and Handover protocols). This avoided the use of unstable Link Time Optimization (LTO), ensuring robust USB enumeration across all operating systems.
Safety & Persistence: Implemented hardware-level flash protection (NVMCTRL_BOOTPROT) for the first 8KB. This ensures the bootloader cannot be accidentally overwritten or corrupted during normal application firmware updates.
Technical Details:
Target MCU: ATSAMD21E18A
Bootloader Size: 8KB (8192 bytes)
USB Drive Label: CWUSBUPDATE
Build Environment: Compiled with arm-none-eabi-gcc on Linux Mint.
Programming Interface: Verified flashing and fuse configuration via Atmel-ICE and SWD.