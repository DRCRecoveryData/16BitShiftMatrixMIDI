# MIDI Button Controller

This Arduino sketch allows you to create a MIDI button controller using shift registers. It reads the state of buttons connected to shift registers and sends MIDI notes accordingly.

## Requirements

- Arduino board compatible with MIDIUSB library
- MIDI USB cable
- Shift registers (e.g., 74HC595)
- Momentary push buttons

## Circuit Diagram

![Circuit Diagram](circuit_diagram.png)

## Connecting a 4x4 Button Matrix to Two 74HC165 Shift Registers

Certainly! Here's how you can connect a 4x4 button matrix to two 74HC165 shift registers in a grid layout:

### Button Matrix:
| Button | Connection                                |
|--------|-------------------------------------------|
| 1      | PI0 (Parallel Input 0) of first 74HC165  |
| 2      | PI1 (Parallel Input 1) of first 74HC165  |
| 3      | PI2 (Parallel Input 2) of first 74HC165  |
| 4      | PI3 (Parallel Input 3) of first 74HC165  |
| 5      | PI4 (Parallel Input 4) of first 74HC165  |
| 6      | PI5 (Parallel Input 5) of first 74HC165  |
| 7      | PI6 (Parallel Input 6) of first 74HC165  |
| 8      | PI7 (Parallel Input 7) of first 74HC165  |
| 9      | PI0 (Parallel Input 0) of second 74HC165 |
| 10     | PI1 (Parallel Input 1) of second 74HC165 |
| 11     | PI2 (Parallel Input 2) of second 74HC165 |
| 12     | PI3 (Parallel Input 3) of second 74HC165 |
| 13     | PI4 (Parallel Input 4) of second 74HC165 |
| 14     | PI5 (Parallel Input 5) of second 74HC165 |
| 15     | PI6 (Parallel Input 6) of second 74HC165 |
| 16     | PI7 (Parallel Input 7) of second 74HC165 |

### Shift Register Connections:
| Pin   | Connection                                |
|-------|-------------------------------------------|
| Q7    | Serial Data Input (DS) of second 74HC165 |
| CLK   | Clock (CLK) of Arduino (or microcontroller)|
| ST_CP | Latch (ST_CP) of Arduino (or microcontroller)|
| VCC   | +5V power supply                          |
| GND   | Ground                                    |


## Button Matrix Layout

In a 4x4 button matrix layout, the buttons are typically numbered sequentially from left to right, top to bottom. Here's how the buttons would be numbered in a 4x4 grid:

```
 1  2  3  4
 5  6  7  8
 9 10 11 12
13 14 15 16
```

Each number corresponds to a physical button in the matrix. For example, button 1 is the top-left button, button 2 is the button next to it to the right, and so on. This numbering scheme allows you to easily identify each button's position in the matrix.

## Usage

1. Connect your Arduino board to your computer via USB.
2. Upload the sketch to your Arduino board.
3. Connect the shift registers and buttons according to the circuit diagram.
4. Use the buttons to trigger MIDI notes. Pressing a button sends a MIDI note-on message, and releasing it sends a MIDI note-off message.

## How It Works

The sketch uses the `MIDIUSB` library to send MIDI messages via USB. It reads the state of buttons connected to shift registers by shifting in their states using the `shiftIn` function. When a button is pressed, it sends a MIDI note-on message, and when it's released, it sends a MIDI note-off message.

## Credits

This sketch is adapted from [Arduino MIDI Button Controller](https://github.com/arduino-libraries/MIDIUSB/blob/master/examples/MIDIUSB_ButtonController/MIDIUSB_ButtonController.ino) example provided by the `MIDIUSB` library.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

