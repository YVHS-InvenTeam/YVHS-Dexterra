#include "HardwareSerial.h"
/*
 * DRV8833.h - A simple class to control a DRV8833 motor driver
 * for standard Arduino boards.
 */
#ifndef DRV8833_H
#define DRV8833_H

#include <Arduino.h>

class DRV8833 {
public:
  // Constructor: Specifies the two pins connected to IN1 and IN2.
  // These should be PWM-capable pins.
  DRV8833(int in1Pin, int in2Pin) {
    _in1 = in1Pin;
    _in2 = in2Pin;
  }

  // Initializes the pins. Call this inside your setup() function.
  void initialize() {
    pinMode(_in1, OUTPUT);
    pinMode(_in2, OUTPUT);
  }

  // Drives the motor.
  // speed: -255 (full reverse) to 255 (full forward).
  void drive(int speed) {
    speed = constrain(speed, -255, 255);

    if (speed > 0) { // Forward
      analogWrite(_in1, speed);
      analogWrite(_in2, 0);
      
    } else if (speed < 0) { // Reverse
      analogWrite(_in1, 0);
      analogWrite(_in2, -speed);

    } else { // Speed is 0, so brake
      brake();
    }
  }

  // Brakes the motor by setting both inputs high (dynamic braking).
  void brake() {
      analogWrite(_in1, 255);
      analogWrite(_in2, 255);
  }

  // Coasts the motor by setting both inputs low (free-wheeling).
  void coast() {
      analogWrite(_in1, 0);
      analogWrite(_in2, 0);
  }

private:
  int _in1; // Pin connected to the IN1
  int _in2; // Pin connected to the IN2
};

#endif // DRV8833_H
