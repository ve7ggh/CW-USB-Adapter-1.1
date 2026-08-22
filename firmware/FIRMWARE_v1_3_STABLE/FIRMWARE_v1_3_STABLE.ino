/*
 ====
 *  VE7GGH CW USB Adapter — Pure HID Keyboard Interface (v1.3 Stable)
 * ====
 *  Author      : VE7GGH
 *  License     : MIT License (Open Source)
 *  Hardware    : VE7GGH CW USB Adapter (SAMD21E18A)
 *  IDE         : Arduino IDE 1.8.13+ / 2.x
 *  USB Stack   : TinyUSB (Tools > USB Stack > TinyUSB)
 *  VID / PID   : 0x04D8 : 0xE417 (Sublicensed via Microchip Application)
 *  73 de VE7GGH!
 *
 *  Description :
 *    Dedicated, ultra-low-latency USB HID keyboard adapter for CW iambic paddles.
 *    Translates paddle Dit and Dah contacts into Left Ctrl (Dit) and Right Ctrl (Dah)
 *    modifier keypresses for CW keyer software (e.g., Morse Runner, VBand, web keyers).
 *
 *    TRS/TRRS Compatibility:
 *    A standard 3.5mm TRS stereo plug inserted into the TRRS jack grounds both
 *    PIN_RING2 and PIN_SLEEVE, ensuring clean ground reference.
 *
 *  Pin Mapping :
 *    DIT  (Tip)    -> PIN_TIP    (GPIO 0) -> Left Ctrl
 *    DAH  (Ring 1) -> PIN_RING1  (GPIO 2) -> Right Ctrl
 *    GND  (Ring 2) -> PIN_RING2  (GPIO 4) -> Output LOW (TRS compatibility)
 *    GND  (Sleeve) -> PIN_SLEEVE (GPIO 5) -> Output LOW
 *
 *  NeoPixel Feedback :
 *    Idle        -> Dim Blue
 *    Dit Active  -> Green
 *    Dah Active  -> Amber
 *    Both Active -> Purple
 *
 *  MIT License :
 *    Copyright (c) 2025 VE7GGH
 *    Permission is hereby granted, free of charge, to any person obtaining a copy
 *    of this software and associated documentation files (the "Software"), to deal
 *    in the Software without restriction, including without limitation the rights
 *    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the Software is
 *    furnished to do so, subject to the following conditions:
 *
 *    The above copyright notice and this permission notice shall be included in
 *    all copies or substantial portions of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *    THE SOFTWARE.
 * ====
 */





#include <Adafruit_NeoPixel.h>
#include "Adafruit_TinyUSB.h"

// ── USB Identification (must match bootloader/board_config.h) ────
#define USB_VID 0x04D8
#define USB_PID 0xE417

// ── Hardware ────
#define NEOPIXEL_PIN    PIN_NEOPIXEL
#define NEOPIXEL_COUNT  1

// Pin mapping based on your diagnostic results
#define DIT_PIN         PIN_TIP     // 0
#define DAH_PIN         PIN_RING1   // 2
#define GND_COMP_PIN    PIN_RING2   // 4 (Extra ground for TRS compatibility)
#define GND_MAIN_PIN    PIN_SLEEVE  // 5

// ── NeoPixel ────
Adafruit_NeoPixel pixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// ── TinyUSB HID ────
uint8_t const desc_hid_report[] = { TUD_HID_REPORT_DESC_KEYBOARD() };
Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_KEYBOARD, 2, false);

// ── Colours ────
#define COLOR_IDLE   pixel.Color(0,   0,   20)   // Dim Blue
#define COLOR_DIT    pixel.Color(0,   80,  0)   // Green
#define COLOR_DAH    pixel.Color(80,  40,  0)   // Amber
#define COLOR_BOTH   pixel.Color(80,  0,   80)  // Purple

// ── Key mappings ────
#define DIT_MODIFIER  KEYBOARD_MODIFIER_LEFTCTRL
#define DAH_MODIFIER  KEYBOARD_MODIFIER_RIGHTCTRL

// ── Timing (tune here) ────
#define DEBOUNCE_MS         5     // contact must be stable this long before accepted
#define STUCK_KEY_MS         4000  // auto-release a paddle held longer than this (fail-safe)
#define POLL_INTERVAL_MS     2     // main loop polling interval
#define USB_MOUNT_TIMEOUT_MS  3000  // give up waiting for host enumeration after this long

// ── Runtime state ────
static bool     ditState = false, dahState = false;
static bool     ditLastRaw = false, dahLastRaw = false;
static uint32_t ditChangeT = 0, dahChangeT = 0;
static uint32_t ditHeldSince = 0, dahHeldSince = 0;
static uint32_t lastPoll = 0;
static uint8_t  lastModifier = 0xFF; // force first HID report to be sent

void setup() {
  // Set VID/PID to match bootloader.
  // NOTE: On SAMD cores, TinyUSB auto-starts before setup() runs, so we
  // must force a detach/re-attach for the new ID to actually take effect.
  TinyUSBDevice.setID(USB_VID, USB_PID);
  TinyUSBDevice.setManufacturerDescriptor("VE7GGH");
  TinyUSBDevice.setProductDescriptor("VE7GGH CW Adapter");
  TinyUSBDevice.detach();
  delay(10);
  TinyUSBDevice.attach();

  // Set up paddles with internal pull-ups
  pinMode(DIT_PIN, INPUT_PULLUP);
  pinMode(DAH_PIN, INPUT_PULLUP);

  // Force both potential Ground contacts to LOW
  pinMode(GND_COMP_PIN, OUTPUT);
  digitalWrite(GND_COMP_PIN, LOW);
  pinMode(GND_MAIN_PIN, OUTPUT);
  digitalWrite(GND_MAIN_PIN, LOW);

  usb_hid.begin();

  pixel.begin();
  pixel.setBrightness(80);
  pixel.setPixelColor(0, COLOR_IDLE);
  pixel.show();

  // Wait for USB connection (bounded — avoid hanging forever on enumeration failure)
  uint32_t mountStart = millis();
  while (!TinyUSBDevice.mounted()) {
    if (millis() - mountStart > USB_MOUNT_TIMEOUT_MS) {
      // Enumeration failed/timed out — flash red as an error indicator and continue.
      // (loop() still guards on usb_hid.ready(), so no HID reports are sent until
      // the host actually enumerates the device, even if that happens later.)
      pixel.setPixelColor(0, pixel.Color(80, 0, 0));
      pixel.show();
      delay(150);
      pixel.setPixelColor(0, pixel.Color(0, 0, 0));
      pixel.show();
      delay(150);
      break;
    }
    delay(1);
  }

  // Success indicator flash
  pixel.setPixelColor(0, pixel.Color(80, 80, 80));
  pixel.show();
  delay(300);
  pixel.setPixelColor(0, COLOR_IDLE);
  pixel.show();
}

void loop() {
  if (!usb_hid.ready()) return;

  uint32_t now = millis();
  if (now - lastPoll < POLL_INTERVAL_MS) return;
  lastPoll = now;

  bool ditRaw = (digitalRead(DIT_PIN) == LOW);
  bool dahRaw = (digitalRead(DAH_PIN) == LOW);

  // Real debounce: only accept a new state once it has been stable for DEBOUNCE_MS
  if (ditRaw != ditLastRaw) { ditChangeT = now; ditLastRaw = ditRaw; }
  if (dahRaw != dahLastRaw) { dahChangeT = now; dahLastRaw = dahRaw; }
  if (now - ditChangeT > DEBOUNCE_MS) ditState = ditRaw;
  if (now - dahChangeT > DEBOUNCE_MS) dahState = dahRaw;

  // Stuck-key fail-safe: auto-release a paddle held continuously too long
  // (guards against a shorted/stuck contact holding a Ctrl key forever)
  if (ditState) { if (!ditHeldSince) ditHeldSince = now; else if (now - ditHeldSince > STUCK_KEY_MS) ditState = false; }
  else ditHeldSince = 0;
  if (dahState) { if (!dahHeldSince) dahHeldSince = now; else if (now - dahHeldSince > STUCK_KEY_MS) dahState = false; }
  else dahHeldSince = 0;

  uint8_t modifier = 0;
  if (ditState) modifier |= DIT_MODIFIER;
  if (dahState) modifier |= DAH_MODIFIER;

  // NeoPixel Feedback
  if (ditState && dahState) pixel.setPixelColor(0, COLOR_BOTH);
  else if (ditState)        pixel.setPixelColor(0, COLOR_DIT);
  else if (dahState)        pixel.setPixelColor(0, COLOR_DAH);
  else                      pixel.setPixelColor(0, COLOR_IDLE);
  pixel.show();

  // Only send a new HID report when the modifier state actually changes
  if (modifier != lastModifier) {
    uint8_t keycodes[6] = {0};
    usb_hid.keyboardReport(0, modifier, keycodes);
    lastModifier = modifier;
  }
}