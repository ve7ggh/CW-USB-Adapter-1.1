VE7GGH CW Adapter
Custom UF2 Bootloader Notes

Project:
https://github.com/ve7ggh/CWUSB-Adapter

Base Source:
Adafruit uf2-samdx1
Version: v4.0.0

------------------------------------------------------------
Reason for this patch
------------------------------------------------------------

The standard uf2-samdx1 source hard-codes several optional feature
definitions in inc/uf2.h.

To build an 8 KB bootloader for the SAMD21E18A without using Link Time
Optimization (LTO), these definitions were changed so they can be
overridden from the build system using compiler command-line options.

This allows optional bootloader features to be disabled while preserving
stable USB enumeration.

------------------------------------------------------------
Modified file
------------------------------------------------------------

inc/uf2.h

------------------------------------------------------------
Changes made
------------------------------------------------------------

Changed:

#define USE_WEBUSB 1

to:

#ifndef USE_WEBUSB
#define USE_WEBUSB 1
#endif


Changed:

#define USE_HID_EXT 1

to:

#ifndef USE_HID_EXT
#define USE_HID_EXT 1
#endif


Changed:

#define USE_HID_HANDOVER 1

to:

#ifndef USE_HID_HANDOVER
#define USE_HID_HANDOVER 1
#endif


Changed:

#define USE_MSC_HANDOVER 1

to:

#ifndef USE_MSC_HANDOVER
#define USE_MSC_HANDOVER 1
#endif

------------------------------------------------------------
Build override
------------------------------------------------------------

The following options are supplied in Makefile.user:

BLD_EXTA_FLAGS = \
    -DUSE_WEBUSB=0 \
    -DUSE_HID_EXT=0 \
    -DUSE_HID_HANDOVER=0 \
    -DUSE_MSC_HANDOVER=0

------------------------------------------------------------
Result
------------------------------------------------------------

The resulting bootloader:

• Fits within the required 8192-byte bootloader region.
• Compiles without Link Time Optimization (-flto).
• Enumerates correctly as a USB UF2 device.
• Preserves normal UF2 drag-and-drop firmware updates.
• Removes only optional bootloader functionality not required by the
  VE7GGH CW Adapter.

------------------------------------------------------------
Notes
------------------------------------------------------------

These modifications affect ONLY the bootloader.

The user application (firmware) is completely unaffected and continues
to operate normally, including USB HID keyboard functionality.