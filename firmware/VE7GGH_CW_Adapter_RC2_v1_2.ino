/*
  VE7GGH CW USB Adapter
  Firmware Version: 1.2 RC2

  Author:   VE7GGH
  Project:  CW USB Adapter
  GitHub:   https://github.com/ve7ggh/CW-USB-Adatper-1.1
  License:  See LICENSE.md in project repository

  Description:
    Open-source USB CW paddle interface firmware.
    Converts a standard 3.5mm TRS iambic CW paddle into a USB HID keyboard
    and/or USB MIDI device. No drivers required.

  Features:
    - USB HID Keyboard: Left Ctrl (Dit) / Right Ctrl (Dah)
    - USB MIDI Device: Note On/Off for Dit/Dah (default notes 20/21)
    - Three operating modes: HID Only, MIDI Only, HID + MIDI
    - Serial CLI for real-time configuration
    - Flash persistence: mode and MIDI notes survive power cycles
    - Onboard RGB NeoPixel status indicator

  Compatible software:
    - Morse Code World, VBand, DitDahDit, and any CW keyboard app (HID mode)
    - FlexRadio / NetKeyer (MIDI mode)

  Hardware:
    - Microchip ATSAMD21E18A MCU
    - USB 2.0 Full-Speed Native USB
    - 3.5mm TRS paddle input
    - RGB NeoPixel status LED
    - UF2 drag-and-drop firmware updates

  Build Requirements:
    - Arduino IDE with Adafruit SAMD board package
    - Tools > USB Stack: TinyUSB
    - Libraries: Adafruit NeoPixel, Adafruit TinyUSB, FlashStorage_SAMD (v1.3.2+)

  Serial CLI Commands:
    help                       Show available commands
    status                     Show mode, paddle state, MIDI notes
    mode [hid|midi|both]       Set operating mode (type 'save' to persist)
    midi_notes                 Show current MIDI note assignments
    set_notes <dit> <dah>      Set MIDI notes 0-127 (type 'save' to persist)
    save                       Save current settings to flash
    reset                      Restore factory defaults and save

  Firmware Update:
    1. Double-tap the Reset button
    2. A USB drive will appear on the host
    3. Drag the new .uf2 file onto the drive
    4. Device reboots automatically

  This is an independent open-source project.
  Not affiliated with, sponsored by, or endorsed by Adafruit Industries.

  Canadian-designed open-source ham radio project.
  73 de VE7GGH
*/
#define USB_MANUFACTURER "VE7GGH"
#define USB_PRODUCT      "CW USB Adapter"
#include <Adafruit_NeoPixel.h>
#include "Adafruit_TinyUSB.h"
#include <FlashStorage_SAMD.h>

// Hardware pins
#define NEOPIXEL_PIN    PIN_NEOPIXEL
#define NEOPIXEL_COUNT  1
#define DIT_PIN         PIN_TIP
#define DAH_PIN         PIN_RING1
#define GND_COMP_PIN    PIN_RING2
#define GND_MAIN_PIN    PIN_SLEEVE

// Factory defaults
#define DEFAULT_DIT_NOTE  20
#define DEFAULT_DAH_NOTE  21
#define DEFAULT_MODE      2     // MODE_BOTH

// USB HID keyboard modifier mappings
#define DIT_MODIFIER  KEYBOARD_MODIFIER_LEFTCTRL
#define DAH_MODIFIER  KEYBOARD_MODIFIER_RIGHTCTRL

// Operating modes
enum OperatingMode { MODE_HID_ONLY = 0, MODE_MIDI_ONLY = 1, MODE_BOTH = 2 };

// Persisted settings structure
typedef struct {
  boolean valid;
  uint8_t ditNote;
  uint8_t dahNote;
  uint8_t mode;
} Settings;

FlashStorage(flash_store, Settings);

// Peripherals
Adafruit_NeoPixel pixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

const uint8_t desc_hid_report[] = { TUD_HID_REPORT_DESC_KEYBOARD() };
Adafruit_USBD_HID  usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_KEYBOARD, 2, false);
Adafruit_USBD_MIDI usb_midi;

// Runtime state
OperatingMode currentMode = (OperatingMode)DEFAULT_MODE;
uint8_t       ditNote     = DEFAULT_DIT_NOTE;
uint8_t       dahNote     = DEFAULT_DAH_NOTE;
bool          prevDit     = false;
bool          prevDah     = false;

// Forward declarations
void loadSettings();
void saveSettings();
void resetSettings();
void handleHID(bool dit, bool dah);
void handleMIDI(bool dit, bool dah);
void sendNoteOn(uint8_t ch, uint8_t note, uint8_t vel);
void sendNoteOff(uint8_t ch, uint8_t note, uint8_t vel);
void updatePixel(bool dit, bool dah);
void flashPixel(uint32_t color, int times, int ms);
void handleSerial();
void printWelcome();
void printStatus();

void setup() {
  pinMode(DIT_PIN,      INPUT_PULLUP);
  pinMode(DAH_PIN,      INPUT_PULLUP);
  pinMode(GND_COMP_PIN, OUTPUT);
  pinMode(GND_MAIN_PIN, OUTPUT);
  digitalWrite(GND_COMP_PIN, LOW);
  digitalWrite(GND_MAIN_PIN, LOW);

  
  usb_hid.begin();
  usb_midi.begin();

  pixel.begin();
  pixel.setBrightness(80);
  pixel.setPixelColor(0, pixel.Color(0, 0, 20));
  pixel.show();

  Serial.begin(115200);
  unsigned long t = millis();
  while (!Serial && (millis() - t < 2000)) delay(1);
  while (!TinyUSBDevice.mounted()) delay(1);

  loadSettings();

  // Startup: white double-flash
  flashPixel(pixel.Color(80, 80, 80), 2, 150);
  pixel.setPixelColor(0, pixel.Color(0, 0, 20));
  pixel.show();

  printWelcome();
  printStatus();
}

void loop() {
  bool dit = (digitalRead(DIT_PIN) == LOW);
  bool dah = (digitalRead(DAH_PIN) == LOW);

  if (currentMode == MODE_HID_ONLY  || currentMode == MODE_BOTH) handleHID(dit, dah);
  if (currentMode == MODE_MIDI_ONLY || currentMode == MODE_BOTH) handleMIDI(dit, dah);

  updatePixel(dit, dah);
  handleSerial();

  prevDit = dit;
  prevDah = dah;
  delay(5);
}

// FIX: FlashStorage_SAMD requires read(T &data) — pass struct by reference
void loadSettings() {
  Settings s;
  flash_store.read(s);
  if (s.valid) {
    currentMode = (OperatingMode)constrain(s.mode, 0, 2);
    ditNote     = (uint8_t)constrain(s.ditNote, 0, 127);
    dahNote     = (uint8_t)constrain(s.dahNote, 0, 127);
    Serial.println("Settings loaded from flash.");
  } else {
    currentMode = (OperatingMode)DEFAULT_MODE;
    ditNote     = DEFAULT_DIT_NOTE;
    dahNote     = DEFAULT_DAH_NOTE;
    Serial.println("No saved settings. Using factory defaults.");
  }
}

void saveSettings() {
  Settings s;
  s.valid   = true;
  s.mode    = (uint8_t)currentMode;
  s.ditNote = ditNote;
  s.dahNote = dahNote;
  flash_store.write(s);
  Serial.println("Settings saved to flash.");
  flashPixel(pixel.Color(0, 80, 80), 3, 100);
  pixel.setPixelColor(0, pixel.Color(0, 0, 20));
  pixel.show();
}

void resetSettings() {
  currentMode = (OperatingMode)DEFAULT_MODE;
  ditNote     = DEFAULT_DIT_NOTE;
  dahNote     = DEFAULT_DAH_NOTE;
  saveSettings();
  Serial.println("Factory defaults restored and saved.");
}

void handleHID(bool dit, bool dah) {
  uint8_t mod = 0;
  if (dit) mod |= DIT_MODIFIER;
  if (dah) mod |= DAH_MODIFIER;
  uint8_t keys[6] = {0};
  usb_hid.keyboardReport(0, mod, keys);
}

void sendNoteOn(uint8_t ch, uint8_t note, uint8_t vel) {
  uint8_t packet[4];
  packet[0] = 0x09;
  packet[1] = 0x90 | (ch & 0x0F);
  packet[2] = note & 0x7F;
  packet[3] = vel  & 0x7F;
  usb_midi.write(packet, 4);
}

void sendNoteOff(uint8_t ch, uint8_t note, uint8_t vel) {
  uint8_t packet[4];
  packet[0] = 0x08;
  packet[1] = 0x80 | (ch & 0x0F);
  packet[2] = note & 0x7F;
  packet[3] = vel  & 0x7F;
  usb_midi.write(packet, 4);
}

void handleMIDI(bool dit, bool dah) {
  if ( dit && !prevDit) sendNoteOn (0, ditNote, 127);
  if (!dit &&  prevDit) sendNoteOff(0, ditNote, 0);
  if ( dah && !prevDah) sendNoteOn (0, dahNote, 127);
  if (!dah &&  prevDah) sendNoteOff(0, dahNote, 0);
}

void updatePixel(bool dit, bool dah) {
  uint32_t color;
  if      (dit && dah) color = pixel.Color(80,  0,  80);
  else if (dit)        color = pixel.Color(0,  80,   0);
  else if (dah)        color = pixel.Color(80, 40,   0);
  else                 color = pixel.Color(0,   0,  20);
  pixel.setPixelColor(0, color);
  pixel.show();
}

void flashPixel(uint32_t color, int times, int ms) {
  for (int i = 0; i < times; i++) {
    pixel.setPixelColor(0, color);
    pixel.show();
    delay(ms);
    pixel.setPixelColor(0, 0);
    pixel.show();
    delay(ms);
  }
}

void printStatus() {
  const char* modeStr = (currentMode == MODE_HID_ONLY)  ? "HID Only"  :
                        (currentMode == MODE_MIDI_ONLY) ? "MIDI Only" : "HID + MIDI";
  Serial.print("Mode     : "); Serial.println(modeStr);
  Serial.print("DIT note : "); Serial.println(ditNote);
  Serial.print("DAH note : "); Serial.println(dahNote);
  Serial.print("DIT pin  : "); Serial.println(digitalRead(DIT_PIN) == LOW ? "active" : "inactive");
  Serial.print("DAH pin  : "); Serial.println(digitalRead(DAH_PIN) == LOW ? "active" : "inactive");
}

void handleSerial() {
  if (!Serial.available()) return;

  String line = Serial.readStringUntil('\n');
  line.trim();
  if (line.length() == 0) return;

  String cmd = line;
  cmd.toLowerCase();

  if (cmd == "help") {
    Serial.println("----------------------------------------");
    Serial.println("  VE7GGH CW USB Adapter - Firmware 1.2");
    Serial.println("  github.com/ve7ggh/CW-USB-Adatper-1.1");
    Serial.println("Commands:");
    Serial.println("  help");
    Serial.println("  status");
    Serial.println("  mode [hid|midi|both]");
    Serial.println("  midi_notes");
    Serial.println("  set_notes <dit> <dah>   (0-127)");
    Serial.println("  save");
    Serial.println("  reset");
    Serial.println("----------------------------------------");
  } else if (cmd == "status") {
    printStatus();
  } else if (cmd.startsWith("mode ")) {
    String arg = cmd.substring(5);
    arg.trim();
    if      (arg == "hid")  { currentMode = MODE_HID_ONLY;  Serial.println("Mode: HID Only (type 'save' to persist)"); }
    else if (arg == "midi") { currentMode = MODE_MIDI_ONLY; Serial.println("Mode: MIDI Only (type 'save' to persist)"); }
    else if (arg == "both") { currentMode = MODE_BOTH;      Serial.println("Mode: HID + MIDI (type 'save' to persist)"); }
    else Serial.println("Unknown mode. Use: hid, midi, both");
  } else if (cmd == "midi_notes") {
    Serial.print("DIT note: "); Serial.println(ditNote);
    Serial.print("DAH note: "); Serial.println(dahNote);
  } else if (cmd.startsWith("set_notes ")) {
    String rest = cmd.substring(10);
    rest.trim();
    int sp = rest.indexOf(' ');
    if (sp <= 0) {
      Serial.println("Usage: set_notes <dit> <dah>");
    } else {
      int nd = rest.substring(0, sp).toInt();
      int nh = rest.substring(sp + 1).toInt();
      if (nd < 0 || nd > 127 || nh < 0 || nh > 127) {
        Serial.println("Notes must be 0-127");
      } else {
        ditNote = (uint8_t)nd;
        dahNote = (uint8_t)nh;
        Serial.print("DIT note: "); Serial.println(ditNote);
        Serial.print("DAH note: "); Serial.println(dahNote);
        Serial.println("(type 'save' to persist)");
      }
    }
  } else if (cmd == "save") {
    saveSettings();
  } else if (cmd == "reset") {
    resetSettings();
    printStatus();
  } else {
    Serial.println("Unknown command. Type 'help'");
  }
}

void printWelcome() {
  Serial.println();
  Serial.println("========================================");
  Serial.println("   VE7GGH CW USB Adapter  |  FW 1.2   ");
  Serial.println("   Open Source CW Paddle Interface     ");
  Serial.println("   github.com/ve7ggh/CW-USB-Adatper-1.1");
  Serial.println("   Type 'help' for commands            ");
  Serial.println("========================================");
  Serial.println();
}