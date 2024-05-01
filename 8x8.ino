#include <MIDIUSB.h>

// Define pin connections for 74HC165 shift registers
#define DATA_IN_PIN   4 // Connect to SHFTSDA (QH)
#define CLOCK_PIN     6 // Connect to SHFTCLK (CLK)
#define LATCH_PIN     5 // Connect to SHFTSERIAL (SERIAL_SHIFT/PARALLEL_LOAD)
#define NUM_REGISTERS 8 // Number of 74HC165 shift registers used (8 bits per register)

// Define MIDI note numbers for the 8x8 button matrix
const byte noteMap[64] = {
  39, 38, 37, 36, 43, 42, 41, 40,
  47, 46, 45, 44, 51, 50, 49, 48,
  55, 54, 53, 52, 59, 58, 57, 56,
  63, 62, 61, 60, 67, 66, 65, 64,
  71, 70, 69, 68, 75, 74, 73, 72,
  79, 78, 77, 76, 83, 82, 81, 80,
  87, 86, 85, 84, 91, 90, 89, 88,
  95, 94, 93, 92, 99, 98, 97, 96
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