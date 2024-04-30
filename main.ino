#include <MIDIUSB.h>

// Define pins for shift register connections
#define DATA_PIN 2
#define CLOCK_PIN 3
#define LATCH_PIN 4

void setup() {
  // Set shift register pins as OUTPUT
  pinMode(DATA_PIN, INPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
}

void loop() {
  // Read button states
  byte buttonStates[2] = {0};
  readButtons(buttonStates);

  // Process button states
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 8; j++) {
      int buttonIndex = i * 8 + j;
      bool pressed = bitRead(buttonStates[i], j);
      if (pressed) {
        // Button pressed, send MIDI note
        midiEventPacket_t noteOn = {0x09, 0x90 | 0, buttonIndex, 127};
        MidiUSB.sendMIDI(noteOn);
      } else {
        // Button released, send MIDI note off
        midiEventPacket_t noteOff = {0x08, 0x80 | 0, buttonIndex, 0};
        MidiUSB.sendMIDI(noteOff);
      }
    }
  }
}

void readButtons(byte* buttonStates) {
  // Shift in button states from shift registers
  digitalWrite(LATCH_PIN, LOW);
  delayMicroseconds(1);
  digitalWrite(LATCH_PIN, HIGH);
  delayMicroseconds(1);
  
  for (int i = 0; i < 2; i++) {
    buttonStates[i] = shiftIn(DATA_PIN, CLOCK_PIN);
  }
}

byte shiftIn(int dataPin, int clockPin) {
  byte bits = 0;
  for (int i = 0; i < 8; ++i) {
    digitalWrite(clockPin, LOW);
    delayMicroseconds(1);
    bitWrite(bits, i, digitalRead(dataPin));
    digitalWrite(clockPin, HIGH);
    delayMicroseconds(1);
  }
  return bits;
}
