/*
 * PID.h - A simple PID controller class for Arduino.
 * This version includes a non-destructive deadband and a fix for the derivative calculation.
 */
#ifndef PID_H
#define PID_H

#include <Arduino.h>
#include <math.h> // Required for fabs()

class PID {
public:
  // Constructor: Initializes the PID controller with given gains.
  PID(double kp, double ki, double kd) {
    setTunings(kp, ki, kd);
    _lastTime = millis();
    _integral = 0;
    _lastError = 0;
    _tolerance = 0;
    setOutputLimits(0, 255); // Default output range
  }

  // Computes the PID output based on the current input and setpoint.
  double compute(double input, double setpoint) {
    unsigned long now = millis();
    double dt = (double)(now - _lastTime) / 1000.0; // Time difference in seconds

    double error = setpoint - input;

    // --- Deadband/Tolerance Check ---
    // If the error is within the tolerance, return 0 to stop the motor.
    // We DON'T reset the integral, preserving its state for the next move.
    if (fabs(error) <= _tolerance) {
      _lastError = error; // Update lastError so atSetpoint() is accurate
      _lastTime = now;
      return 0.0;
    }

    // Proportional term
    double p_term = _kp * error;

    // Integral term calculation
    _integral += _ki * error * dt;

    // Anti-windup via Integral Clamping
    if (_integral > _outMax) _integral = _outMax;
    else if (_integral < _outMin) _integral = _outMin;
    
    // Derivative term calculation (FIXED)
    // This now correctly uses the error from the *previous* loop cycle.
    double derivative = 0;
    if (dt > 0) {
      derivative = _kd * (error - _lastError) / dt;
    }

    // Calculate total output
    double output = p_term + _integral + derivative;

    // Final clamp on the total output
    if (output > _outMax) output = _outMax;
    else if (output < _outMin) output = _outMin;

    // Save current state for the NEXT iteration
    _lastError = error;
    _lastTime = now;

    return output;
  }

  // Sets new PID tuning parameters (Kp, Ki, Kd).
  void setTunings(double kp, double ki, double kd) {
    if (kp < 0 || ki < 0 || kd < 0) return;
    _kp = kp;
    _ki = ki;
    _kd = kd;
  }
  
  // Sets the output limits (e.g., -255 to 255 for a motor driver).
  void setOutputLimits(double min, double max) {
    if (min >= max) return;
    _outMin = min;
    _outMax = max;

    if (_integral > _outMax) _integral = _outMax;
    else if (_integral < _outMin) _integral = _outMin;
  }

  // Sets the acceptable error range.
  void setTolerance(double tolerance) {
    _tolerance = tolerance;
  }

  // Returns true if the absolute error is within the defined tolerance.
  bool atSetpoint() {
    return fabs(_lastError) <= _tolerance;
  }

private:
  double _kp, _ki, _kd;
  double _lastError, _integral;
  unsigned long _lastTime;
  double _tolerance;
  double _outMin, _outMax;
};

#endif // PID_H
