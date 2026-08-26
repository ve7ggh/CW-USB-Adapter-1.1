# VE7GGH CW USB Adapter — User Features and Firmware Configuration

## Overview

The VE7GGH CW USB Adapter connects an iambic CW paddle to a computer as a standard USB HID keyboard. It converts the **Dit** paddle contact to **Left Ctrl** and the **Dah** contact to **Right Ctrl**, allowing compatible CW practice, contest, and web keyer software to receive direct paddle input.

## Features and Benefits

### Driverless USB HID operation

The adapter presents itself as a standard USB keyboard.

**Benefit:** It is intended to work without installing a dedicated driver on operating systems that support USB HID keyboards.

### Direct paddle-to-keyboard mapping

- **Dit (Tip):** Left Ctrl
- **Dah (Ring 1):** Right Ctrl

**Benefit:** CW applications that accept separate left and right modifier-key inputs can use the paddle directly.

### TRS and TRRS paddle-plug support

The firmware configures both the Ring 2 and Sleeve contacts as low-level ground references.

**Benefit:** This supports the adapter’s intended use with both standard 3.5 mm TRS stereo plugs and TRRS plugs, subject to the paddle cable’s wiring matching the stated pin mapping.

### Contact debounce

The firmware reads paddle inputs at a short polling interval and accepts an input change only once the contact has remained stable for the configured debounce time.

**Benefit:** Helps suppress false transitions caused by mechanical contact bounce.

### Stuck-key safety release

If a paddle input remains active longer than the configured stuck-key timeout, the firmware releases that key state.

**Benefit:** Reduces the risk that a contaminated contact, cable fault, or accidental short leaves Ctrl held continuously on the host computer.

### NeoPixel status indication

| Adapter state | NeoPixel color |
|---|---|
| Idle / ready | Dim blue |
| Dit active | Green |
| Dah active | Amber |
| Both paddles active | Purple |
| USB connection success during startup | White flash |
| USB mount timeout | Red blink |

**Benefit:** Provides immediate visual confirmation of paddle activity and startup state.

### Bounded USB mount wait

At startup, the firmware waits for USB enumeration only for the configured timeout rather than waiting indefinitely.

**Benefit:** Avoids a permanent startup hang if a host does not enumerate the device promptly.

### Efficient HID reporting

The firmware sends a HID keyboard report only when the modifier-key state changes.

**Benefit:** Avoids redundant USB reports while retaining normal paddle response.

## Firmware Configuration

The configuration values below are located near the top of `FIRMWARE_v1_3_STABLE.ino`. Edit them in the Arduino IDE, then compile and upload the firmware.

### Change Dit/Dah keyboard mapping

The default key mappings are:

```cpp
#define DIT_MODIFIER  KEYBOARD_MODIFIER_LEFTCTRL
#define DAH_MODIFIER  KEYBOARD_MODIFIER_RIGHTCTRL
```

To reverse the output mapping, use:

```cpp
#define DIT_MODIFIER  KEYBOARD_MODIFIER_RIGHTCTRL
#define DAH_MODIFIER  KEYBOARD_MODIFIER_LEFTCTRL
```

This changes which Ctrl key the firmware reports for each paddle input. It does not alter the physical wiring of the Tip and Ring 1 contacts.

### Use different modifier keys

For software configured to use Shift or Alt, change the mapping constants. For example:

```cpp
// Shift mapping
#define DIT_MODIFIER  KEYBOARD_MODIFIER_LEFTSHIFT
#define DAH_MODIFIER  KEYBOARD_MODIFIER_RIGHTSHIFT
```

```cpp
// Alt mapping
#define DIT_MODIFIER  KEYBOARD_MODIFIER_LEFTALT
#define DAH_MODIFIER  KEYBOARD_MODIFIER_RIGHTALT
```

Confirm that the target application supports the selected key combination before using a custom mapping.

### Adjust debounce, polling, and safety timing

```cpp
#define DEBOUNCE_MS          5
#define STUCK_KEY_MS         4000
#define POLL_INTERVAL_MS     2
#define USB_MOUNT_TIMEOUT_MS 3000
```

| Setting | Default | Meaning | Typical adjustment |
|---|---:|---|---|
| `DEBOUNCE_MS` | 5 ms | Time a contact must remain stable before being accepted | Lower values may feel more immediate but can admit bounce; higher values can help noisy contacts. |
| `STUCK_KEY_MS` | 4000 ms | Continuous active time before the firmware releases a paddle state | Increase only if a legitimate use case needs a very long continuous hold. |
| `POLL_INTERVAL_MS` | 2 ms | Interval between paddle scans | Keep low for responsive operation; increasing it reduces scan frequency. |
| `USB_MOUNT_TIMEOUT_MS` | 3000 ms | Maximum startup wait for USB mounting | Increase if a particular host consistently needs more time to enumerate the device. |

### Change NeoPixel brightness

In `setup()`, locate:

```cpp
pixel.setBrightness(80);
```

Use a value from `0` to `255`:

- `0`: off
- Lower values: dimmer
- Higher values: brighter

### Change NeoPixel colors

The color definitions use `pixel.Color(red, green, blue)`, with each RGB channel normally ranging from `0` to `255`.

```cpp
#define COLOR_IDLE   pixel.Color(0,   0,   20)
#define COLOR_DIT    pixel.Color(0,   80,  0)
#define COLOR_DAH    pixel.Color(80,  40,  0)
#define COLOR_BOTH   pixel.Color(80,  0,   80)
```

For example, a red idle state could be defined as:

```cpp
#define COLOR_IDLE pixel.Color(20, 0, 0)
```

## Uploading Modified Firmware

1. Open `FIRMWARE_v1_3_STABLE.ino` in Arduino IDE 1.8.13+ or Arduino IDE 2.x.
2. Select the appropriate SAMD21 board target. Custom CW Adapter target coming late 2026 to Github.
3. Select **Tools → USB Stack → TinyUSB**.
4. If necessary, double-tap the adapter’s reset button to enter bootloader mode.
5. Select the appropriate port and choose **Upload**.
6. Confirm the startup white flash and idle blue status after the device enumerates.

## Important Notes

- Keep the USB VID/PID values aligned with the applicable bootloader and board configuration.
- Changes to modifier keys can affect normal keyboard shortcuts on the host. Test with a CW application before general desktop use.
- The firmware is designed as a paddle-to-HID adapter. It does not itself generate iambic timing, decode Morse, or provide an onboard WPM setting.
