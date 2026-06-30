/*
 * ============================================================
 *  TRRS Trinkey Iambic Paddle HID Interface
 * ============================================================
 *  Author      : VE7GGH
 *  License     : MIT License (Open Source)
 *  Hardware    : Adafruit TRRS Trinkey M0
 *  IDE         : Arduino IDE 2.3.2
 *  USB Stack   : TinyUSB (set in Tools > USB Stack)
 *  73 de VE7GGH!
 *  Description :
 *    Turns an Adafruit TRRS Trinkey into a USB HID keyboard
 *    device for use with a CW iambic morse paddle. The paddle
 *    Dit and Dah contacts are read from the TRRS jack and
 *    translated into Left Ctrl (Dit) and Right Ctrl (Dah)
 *    keypresses, allowing the paddle to interface with any
 *    CW keyer software that accepts keyboard input.
 *
 *    A TRS plug is used in the TRRS jack. PIN_RING2 is driven
 *    LOW alongside PIN_SLEEVE to ensure the TRS sleeve contact
 *    bridges correctly to the circuit ground.
 *
 *  Pin Mapping :
 *    DIT  (Tip)   -> PIN_TIP   (GPIO 0) -> Left Ctrl
 *    DAH  (Ring)  -> PIN_RING1 (GPIO 2) -> Right Ctrl
 *    GND  (Comp)  -> PIN_RING2 (GPIO 4) -> Output LOW (TRS compat)
 *    GND  (Main)  -> PIN_SLEEVE(GPIO 5) -> Output LOW
 *
 *  NeoPixel LED :
 *    Idle         -> Dim Blue
 *    Dit active   -> Green
 *    Dah active   -> Amber
 *    Both active  -> Purple
 *
 *  Libraries   :
 *    - Adafruit NeoPixel
 *    - Adafruit TinyUSB Library (built into board package)
 *
 *  MIT License :
 *    Copyright (c) 2025 VE7GGH
 *    Permission is hereby granted, free of charge, to any
 *    person obtaining a copy of this software and associated
 *    documentation files (the "Software"), to deal in the
 *    Software without restriction, including without limitation
 *    the rights to use, copy, modify, merge, publish,
 *    distribute, sublicense, and/or sell copies of the
 *    Software, and to permit persons to whom the Software is
 *    furnished to do so, subject to the following conditions:
 *
 *    The above copyright notice and this permission notice
 *    shall be included in all copies or substantial portions
 *    of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
 *    ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 *    TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 *    PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 *    DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 *    CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 *    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *    IN THE SOFTWARE.
 * ============================================================
 */





#include <Adafruit_NeoPixel.h>
#include "Adafruit_TinyUSB.h"

// ── Hardware ──────────────────────────────────────────────
#define NEOPIXEL_PIN    PIN_NEOPIXEL
#define NEOPIXEL_COUNT  1

// Pin mapping based on your diagnostic results
#define DIT_PIN         PIN_TIP     // 0
#define DAH_PIN         PIN_RING1   // 2
#define GND_COMP_PIN    PIN_RING2   // 4 (Extra ground for TRS compatibility)
#define GND_MAIN_PIN    PIN_SLEEVE  // 5

// ── NeoPixel ──────────────────────────────────────────────
Adafruit_NeoPixel pixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// ── TinyUSB HID ───────────────────────────────────────────
uint8_t const desc_hid_report[] = { TUD_HID_REPORT_DESC_KEYBOARD() };
Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_KEYBOARD, 2, false);

// ── Colours ───────────────────────────────────────────────
#define COLOR_IDLE   pixel.Color(0,   0,   20)   // Dim Blue
#define COLOR_DIT    pixel.Color(0,   80,  0)   // Green
#define COLOR_DAH    pixel.Color(80,  40,  0)   // Amber
#define COLOR_BOTH   pixel.Color(80,  0,   80)  // Purple

// ── Key mappings ──────────────────────────────────────────
#define DIT_MODIFIER  KEYBOARD_MODIFIER_LEFTCTRL
#define DAH_MODIFIER  KEYBOARD_MODIFIER_RIGHTCTRL

void setup() {
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

  // Wait for USB connection
  while (!TinyUSBDevice.mounted()) delay(1);

  // Success indicator flash
  pixel.setPixelColor(0, pixel.Color(80, 80, 80));
  pixel.show();
  delay(300);
  pixel.setPixelColor(0, COLOR_IDLE);
  pixel.show();
}

void loop() {
  if (!usb_hid.ready()) return;

  bool ditActive = (digitalRead(DIT_PIN) == LOW);
  bool dahActive = (digitalRead(DAH_PIN) == LOW);

  uint8_t modifier = 0;
  if (ditActive) modifier |= DIT_MODIFIER;
  if (dahActive) modifier |= DAH_MODIFIER;

  // NeoPixel Feedback
  if (ditActive && dahActive) pixel.setPixelColor(0, COLOR_BOTH);
  else if (ditActive)          pixel.setPixelColor(0, COLOR_DIT);
  else if (dahActive)          pixel.setPixelColor(0, COLOR_DAH);
  else                         pixel.setPixelColor(0, COLOR_IDLE);
  pixel.show();

  // Send the HID report
  uint8_t keycodes[6] = {0};
  usb_hid.keyboardReport(0, modifier, keycodes);

  delay(10); // Debounce / Polling
}
