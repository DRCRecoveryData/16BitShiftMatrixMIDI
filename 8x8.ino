#include <MIDIUSB.h>

// Define pin connections for 74HC165 shift registers
#define DATA_IN_PIN   2 // Connect to pin 14 (DS) of the first 74HC165
#define CLOCK_PIN     3 // Connect to pin 11 (SH_CP) of all 74HC165s
#define LATCH_PIN     4 // Connect to pin 12 (ST_CP) of all 74HC165s
#define NUM_REGISTERS 8 // Number of 74HC165 shift registers used

// Define MIDI note numbers for the 8x8 button matrix
const byte noteMap[64] = {
  36, 37, 38, 39, 40, 41, 42, 43,
  44, 45, 46, 47, 48, 49, 50, 51,
  52, 53, 54, 55, 56, 57, 58, 59,
  60, 61, 62, 63, 64, 65, 66, 67,
  68, 69, 70, 71, 72, 73, 74, 75,
  76, 77, 78, 79, 80, 81, 82, 83,
  84, 85, 86, 87, 88, 89, 90, 91,
  92, 93, 94, 95, 96, 97, 98, 99
};

void setup() {
  // Initialize shift register pins
  pinMode(DATA_IN_PIN, INPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
}

void loop() {
  // Read button states
  uint64_t buttonStates = readButtonStates();

  // Send MIDI messages for pressed buttons
  for (int i = 0; i < 64; i++) {
    if (buttonStates & (1ULL << i)) {
      sendNoteOn(noteMap[i], 127); // Send note on message
    } else {
      sendNoteOff(noteMap[i], 0); // Send note off message
    }
  }
}

uint64_t readButtonStates() {
  uint64_t buttonStates = 0;

  // Load parallel data into the shift registers
  digitalWrite(LATCH_PIN, LOW);
  delayMicroseconds(1);
  digitalWrite(LATCH_PIN, HIGH);
  
  // Read the serial data from the shift registers
  for (int i = NUM_REGISTERS - 1; i >= 0; i--) {
    buttonStates |= ((uint64_t)shiftIn(DATA_IN_PIN, CLOCK_PIN) << (i * 8)); // Combine bits into 64-bit integer
  }

  return buttonStates;
}

// Function to read serial data from 74HC165
byte shiftIn(int dataPin, int clockPin) {
  byte value = 0;
  for (int i = 0; i < 8; ++i) {
    digitalWrite(clockPin, HIGH);
    delayMicroseconds(1);
    value |= (digitalRead(dataPin) << (7 - i));
    digitalWrite(clockPin, LOW);
    delayMicroseconds(1);
  }
  return value;
}

// Function to send a MIDI note on message
void sendNoteOn(byte note, byte velocity) {
  Serial.write(0x90); // Note on channel 1 command
  Serial.write(note); // Note number
  Serial.write(velocity); // Velocity
}

// Function to send a MIDI note off message
void sendNoteOff(byte note, byte velocity) {
  Serial.write(0x80); // Note off channel 1 command
  Serial.write(note); // Note number
  Serial.write(velocity); // Velocity
}
