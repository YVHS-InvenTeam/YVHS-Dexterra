#include "PID.h"
#include "DRV8833.h"
#include "Encoder.h"
#include "BluetoothReader.h" // Include the new Bluetooth class

// --- Pin Definitions (ESP32 GPIO) ---
// Motor 1
const int M1_ENCODER_A = 22;
const int M1_ENCODER_B = 23; 
const int M1_DRIVER_IN1 = 33;
const int M1_DRIVER_IN2 = 32;
// Motor 2
const int M2_ENCODER_A = 15;
const int M2_ENCODER_B = 4; 
const int M2_DRIVER_IN1 = 13;
const int M2_DRIVER_IN2 = 12;
// Motor 3
const int M3_ENCODER_A = 19;
const int M3_ENCODER_B = 21;
const int M3_DRIVER_IN1 = 26;
const int M3_DRIVER_IN2 = 25;
// Motor 4
const int M4_ENCODER_A = 5;
const int M4_ENCODER_B = 18; 
const int M4_DRIVER_IN1 = 35;
const int M4_DRIVER_IN2 = 34;
// // Motor 5
// const int M5_ENCODER_A = 19;
// const int M5_ENCODER_B = 21;
// const int M5_DRIVER_IN1 = 26;
// const int M5_DRIVER_IN2 = 25;

// --- PID Configuration (can be tuned for each motor) ---
double Kp = .8, Ki = 0.6, Kd = 0.0;

double setpoint1 = 0, setpoint2 = 0, setpoint3 = 0, setpoint4 = 0, setpoint5 = 0;
double mult1 = 42.22, mult2 = 8.89, mult3 = 8.89, mult4 = 15, mult5 = 0;
double tolerance = 15;

// --- Object Creation ---
Encoder encoder1(M1_ENCODER_A, M1_ENCODER_B);
PID     pid1(Kp, Ki, Kd);
DRV8833 motor1(M1_DRIVER_IN1, M1_DRIVER_IN2);

Encoder encoder2(M2_ENCODER_A, M2_ENCODER_B);
PID     pid2(Kp, Ki, Kd);
DRV8833 motor2(M2_DRIVER_IN1, M2_DRIVER_IN2);

Encoder encoder3(M3_ENCODER_A, M3_ENCODER_B);
PID     pid3(Kp, Ki, Kd);
DRV8833 motor3(M3_DRIVER_IN1, M3_DRIVER_IN2);

Encoder encoder4(M4_ENCODER_A, M4_ENCODER_B);
PID     pid4(Kp, Ki, Kd);
DRV8833 motor4(M4_DRIVER_IN1, M4_DRIVER_IN2);

// Encoder encoder5(M3_ENCODER_A, M3_ENCODER_B);
// PID     pid5(Kp, Ki, Kd);
// DRV8833 motor5(M3_DRIVER_IN1, M3_DRIVER_IN2);

BluetoothReader btReader; // Create an object for our Bluetooth reader

// A helper function to parse commands, avoiding duplicate code
void parseAndSetSetpoint(String command) {
  if (command.length() == 0) return;

  int colonIndex = command.indexOf(':');
  if (colonIndex > 0) {
    int motorNum = command.substring(0, colonIndex).toInt();
    double newSetpoint = command.substring(colonIndex + 1).toDouble();

    if (motorNum == 1) {
      setpoint1 = newSetpoint;
      Serial.print("New setpoint for Motor 1: ");
      Serial.println(setpoint1);
    } else if (motorNum == 2) {
      setpoint2 = newSetpoint;
      Serial.print("New setpoint for Motor 2: ");
      Serial.println(setpoint2);
    } else if (motorNum == 3) {
      setpoint3 = newSetpoint;
      Serial.print("New setpoint for Motor 3: ");
      Serial.println(setpoint3);
    } else if (motorNum == 4) {
      setpoint2 = newSetpoint;
      Serial.print("New setpoint for Motor 4: ");
      Serial.println(setpoint4);
    } else if (motorNum == 5) {
      setpoint3 = newSetpoint;
      Serial.print("New setpoint for Motor 5: ");
      Serial.println(setpoint5);
    }
  }
}

void bluetoothParseAndSetSetpoint(String command) {
  // Exit if the command is empty
  if (command.length() == 0) return;

  int currentIndex = 0;
  while (currentIndex < command.length()) {
    char commandChar = ' ';
    int valueStartIndex = -1;

    // Find the next letter in the string
    for (int i = currentIndex; i < command.length(); i++) {
      if (isalpha(command.charAt(i))) {
        commandChar = command.charAt(i);
        valueStartIndex = i + 1;
        break;
      }
    }

    // If no more command characters are found, we're done parsing
    if (valueStartIndex == -1) {
      break;
    }

    // Find where the number ends (either at the next letter or the end of the string)
    int valueEndIndex = command.length();
    for (int i = valueStartIndex; i < command.length(); i++) {
      if (isalpha(command.charAt(i))) {
        valueEndIndex = i;
        break;
      }
    }

    // Extract the number part of the command
    String valueString = command.substring(valueStartIndex, valueEndIndex);
    
    // If we extracted a valid number, process it
    if (valueString.length() > 0) {
      double newSetpoint = valueString.toDouble();

      // Map the command character to the correct motor
      if (commandChar == 'm') {
        setpoint1 = newSetpoint * mult1;
        Serial.print("New setpoint for Motor 1 (m): ");
        Serial.println(setpoint1);
      } else if (commandChar == 'i') { // Using 'i' for Motor 2 from your example
        setpoint2 = newSetpoint * mult2;
        Serial.print("New setpoint for Motor 2 (i): ");
        Serial.println(setpoint2);
      } else if (commandChar == 'r') { // Using 'p' for Motor 3 from your example
        setpoint3 = newSetpoint * mult3;
        Serial.print("New setpoint for Motor 3 (r): ");
        Serial.println(setpoint3);
      } else if (commandChar == 't') {
        setpoint4 = newSetpoint * mult4;
        Serial.print("New setpoint for Motor 4 (t): ");
        Serial.println(setpoint4);
      } else if (commandChar == 'p') {
        setpoint5 = newSetpoint * mult5;
        Serial.print("New setpoint for Motor 5 (p): ");
        Serial.println(setpoint5);
      }
    }

    // Move the index to the start of the next potential command
    currentIndex = valueEndIndex;
  }
}


void setup() {
  Serial.begin(115200); // ESP32 can handle higher baud rates
  btReader.begin("Dexterra"); // Start Bluetooth with a device name
  
  Serial.println("PID Three-Motor Position Controller (ESP32 - analogWrite)");
  Serial.println("Enter target position (e.g., '1:2000' or '2:-500')");

  // Initialize Motor 1 components
  motor1.initialize();
  encoder1.initialize();
  pid1.setOutputLimits(-255, 255);
  pid1.setTolerance(tolerance);

  // Initialize Motor 2 components
  motor2.initialize();
  encoder2.initialize();
  pid2.setOutputLimits(-255, 255);
  pid2.setTolerance(tolerance);

  // Initialize Motor 3 components
  motor3.initialize();
  encoder3.initialize();
  pid3.setOutputLimits(-255, 255);
  pid3.setTolerance(tolerance);

  // Initialize Motor 4 components
  motor4.initialize();
  encoder4.initialize();
  pid4.setOutputLimits(-255, 255);
  pid4.setTolerance(tolerance);
}

void loop() {
  // --- Check for new commands from both sources ---
  if (Serial.available()) {
    parseAndSetSetpoint(Serial.readStringUntil('\n'));
  }
  
  // Read from Bluetooth and print if data exists
  String btCommand = btReader.readCommand();
  if (btCommand.length() > 0) {
    Serial.print("Received via Bluetooth: '");
    Serial.print(btCommand);
    Serial.println("'");
    bluetoothParseAndSetSetpoint(btCommand);
  }


  // --- Update Encoders via Polling ---
  encoder1.update();
  encoder2.update();
  encoder3.update();
  encoder4.update();

  // --- Control System for Motor 1 ---
  double pos1 = encoder1.getPosition();
  double speed1 = pid1.compute(pos1, setpoint1);
  motor1.drive((int)speed1);
  
  // --- Control System for Motor 2 ---
  double pos2 = encoder2.getPosition();
  double speed2 = pid2.compute(pos2, setpoint2);
  motor2.drive((int)speed2);

  // --- Control System for Motor 3 ---
  double pos3 = encoder3.getPosition();
  double speed3 = pid3.compute(pos3, setpoint3);
  motor3.drive((int)speed3);

  // --- Control System for Motor 4 ---
  double pos4 = encoder4.getPosition();
  double speed4 = pid4.compute(pos4, setpoint4);
  motor4.drive((int)speed4);

  // --- Reporting ---
  static unsigned long lastPrintTime = 0;
  if (millis() - lastPrintTime > 200) {
    Serial.print("| Ring (SP:"); Serial.print(setpoint3); Serial.print(",Pos:"); Serial.print(pos3); Serial.print(")"); //Motor 3
    Serial.print("Middle (SP:"); Serial.print(setpoint1); Serial.print(",Pos:"); Serial.print(pos1); Serial.print(") "); //Motor 1
    Serial.print("| Index (SP:"); Serial.print(setpoint2); Serial.print(",Pos:"); Serial.print(pos2); Serial.print(") "); //Motor 2
    Serial.print("| Thumb (SP:"); Serial.print(setpoint4); Serial.print(",Pos:"); Serial.print(pos4); Serial.println(")"); //Motor4
    lastPrintTime = millis();
  }
}
