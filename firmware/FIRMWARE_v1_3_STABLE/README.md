# VE7GGH CW USB Adapter — v1.3 → v1.3STABLE Improvements

## Summary
This update improves firmware robustness and USB HID efficiency without changing pin mapping, USB VID/PID, or NeoPixel color scheme. All changes are backward-compatible with the original v1.3 Stable design.

## Changes

### 1. Real Debounce Logic
- **Before:** A blind `delay(10)` at the end of `loop()` was labeled "Debounce / Polling" but performed no actual debounce.
- **After:** Each paddle input now requires `DEBOUNCE_MS` (default 5 ms) of continuous stability before a state change is accepted, using `millis()`-based timing.

### 2. Non-Blocking Polling
- **Before:** `delay(10)` blocked the entire loop every cycle.
- **After:** Polling interval (`POLL_INTERVAL_MS`, default 2 ms) is handled via `millis()` comparison, keeping the loop responsive and non-blocking.

### 3. Stuck-Key Fail-Safe
- **New:** If a paddle contact remains closed longer than `STUCK_KEY_MS` (default 4000 ms) — e.g. due to a short, moisture, or mechanical failure — the corresponding modifier key is automatically released to prevent an indefinitely "stuck" Ctrl key on the host system.

### 4. Bounded USB Enumeration Wait
- **Before:** `while (!TinyUSBDevice.mounted()) delay(1);` could hang forever in `setup()` if USB enumeration failed.
- **After:** Wait is capped at `USB_MOUNT_TIMEOUT_MS` (default 3000 ms). On timeout, the NeoPixel flashes red twice as a visual error indicator, then `setup()` continues (the main loop still won't send HID reports until `usb_hid.ready()` is actually true).

### 5. Reduced Redundant HID Traffic
- **Before:** A full HID keyboard report was sent every loop cycle (every ~10 ms), regardless of whether the key state changed.
- **After:** A report is only sent when the modifier byte actually changes (`lastModifier` tracking), reducing unnecessary USB traffic.

### 6. Centralized Timing Constants
- All tunable timing values are now grouped under a single `// ── Timing (tune here) ──` block:
  - `DEBOUNCE_MS`
  - `STUCK_KEY_MS`
  - `POLL_INTERVAL_MS`
  - `USB_MOUNT_TIMEOUT_MS`

## Unchanged
- Pin mapping (`DIT_PIN`, `DAH_PIN`, `GND_COMP_PIN`, `GND_MAIN_PIN`)
- USB VID/PID (`0x04D8` / `0xE417`)
- NeoPixel idle/dit/dah/both color scheme
- Overall program structure and license (MIT)

## Not Yet Implemented (Future Considerations)
- Hardware watchdog timer (SAMD21 WDT) as an additional lockup guard.
- Configurable debounce/timeout values via a settings file or serial command interface.
- Return-value checking on `usb_hid.begin()` / `pixel.begin()` for init-failure indication.