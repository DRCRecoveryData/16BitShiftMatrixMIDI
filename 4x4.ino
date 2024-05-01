#include <MIDIUSB.h>

// Define pin connections for 74HC165 shift registers
#define DATA_IN_PIN   4 // Connect to SHFTSDA (QH)
#define CLOCK_PIN     6 // Connect to SHFTCLK (CLK)
#define LATCH_PIN     5 // Connect to SHFTSERIAL (SERIAL_SHIFT/PARALLEL_LOAD)
#define NUM_REGISTERS 1 // Number of 74HC165 shift registers used (4 bits per register)

// Define MIDI note numbers for the 4x4 button matrix
const byte noteMap[16] = {
  48, 49, 50, 51,
  44, 45, 46, 47,
  40, 41, 42, 43,
  36, 37, 38, 39
};

void setup() {
  // Initialize shift register pins
  pinMode(DATA_IN_PIN, INPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
}

void loop() {
  // Read button states
  uint16_t buttonStates = readButtonStates();

  // Send MIDI messages for pressed buttons
  for (int i = 0; i < 16; i++) {
    if (buttonStates & (1 << i)) {
      sendNoteOn(noteMap[i], 127); // Send note on message
    } else {
      sendNoteOff(noteMap[i], 0); // Send note off message
    }
  }
}

uint16_t readButtonStates() {
  uint16_t buttonStates = 0;

  // Load parallel data into the shift registers
  digitalWrite(LATCH_PIN, LOW);
  delayMicroseconds(1);
  digitalWrite(LATCH_PIN, HIGH);
  
  // Read the serial data from the shift registers
  for (int i = NUM_REGISTERS - 1; i >= 0; i--) {
    buttonStates |= ((uint16_t)shiftIn(DATA_IN_PIN, CLOCK_PIN) << (i * 8)); // Combine bits into 16-bit integer
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