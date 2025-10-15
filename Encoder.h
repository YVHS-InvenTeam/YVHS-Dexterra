/*
 * Encoder.h - A simple quadrature encoder reader class.
 *
 * This class is designed to read a standard 2-channel quadrature encoder.
 * It's meant to be used with interrupts for reliable reading.
 */
#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

class Encoder {
public:
  // Constructor: Takes the two pin numbers for channel A and B.
  // These pins MUST support external interrupts (e.g., 2 and 3 on an Arduino Uno).
  Encoder(int pinA, int pinB) : _pinA(pinA), _pinB(pinB), _position(0) {}

  // Initializes the pins. Call this inside your setup() function.
  void initialize() {
    pinMode(_pinA, INPUT_PULLUP);
    pinMode(_pinB, INPUT_PULLUP);
    _lastStateA = digitalRead(_pinA);
  }

  // update() should be called from an Interrupt Service Routine (ISR)
  // that triggers on a change on pin A.
  void update() {
    // Basic quadrature decoding logic:
    // Read the current state of channel A.
    int stateA = digitalRead(_pinA);
    // If the state of channel A has changed, a step has occurred.
    if (stateA != _lastStateA) {
      // Check channel B to determine direction.
      if (digitalRead(_pinB) != stateA) {
        _position++; // Clockwise
      } else {
        _position--; // Counter-clockwise
      }
    }
    _lastStateA = stateA; // Remember the last state of A.
  }

  // Returns the current position in ticks.
  // It temporarily disables interrupts to safely read the multi-byte variable.
  long getPosition() {
    long pos;
    noInterrupts(); // Start critical section
    pos = _position;
    interrupts();   // End critical section
    return pos;
  }

  // Resets the current position to zero.
  void setPosition(long newPosition) {
    noInterrupts();
    _position = newPosition;
    interrupts();
  }

private:
  int _pinA;
  int _pinB;
  volatile long _position;     // volatile is crucial for interrupt variables
  volatile int _lastStateA;    // Remembers the last state of pin A
};

#endif // ENCODER_H
