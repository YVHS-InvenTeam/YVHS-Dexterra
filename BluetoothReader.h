/*
 * BluetoothReader.h
 *
 * A simple class to handle reading newline-terminated strings
 * from a BluetoothSerial connection on the ESP32.
 */
#ifndef BLUETOOTH_READER_H
#define BLUETOOTH_READER_H

#include "BluetoothSerial.h"

// Check if Bluetooth is available on this board
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run 'make menuconfig' to enable it
#endif

class BluetoothReader {
public:
  // Constructor
  BluetoothReader() {}

  // Initializes the Bluetooth Serial connection with a device name.
  // Call this in your main setup() function.
  void begin(String deviceName) {
    BTSerial.begin(deviceName);
    Serial.println("Bluetooth device is ready to pair.");
  }

  // Checks for and reads a complete command string.
  // Returns the command if a newline is found, otherwise returns an empty string.
  // Call this repeatedly in your main loop().
  String readCommand() {
    String command = "";
    if (BTSerial.available()) {
      // Read the incoming data until a newline character is received
      command = BTSerial.readStringUntil('\n');
      command.trim(); // Remove any leading/trailing whitespace
    }
    return command;
  }

private:
  // The library creates a global instance, we just use it.
  BluetoothSerial BTSerial;
};

#endif // BLUETOOTH_READER_H
