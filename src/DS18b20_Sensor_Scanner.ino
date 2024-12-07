#include <Arduino.h>
#include <OneWire.h>

// Define the GPIO pin for the 1-Wire bus
#define ONE_WIRE_BUS 16

// Initialize the OneWire object
OneWire oneWire(ONE_WIRE_BUS);

void setup() {
  Serial.begin(115200); // Initialize serial communication at 115200 baud
  delay(500);           // Delay to allow the Serial Monitor to synchronize
  Serial.println("Starting scan for DS18B20 sensors...");
}

void loop() {
  byte address[8];       // Buffer to hold the 1-Wire device address
  int sensorCount = 0;   // Counter for the number of found DS18B20 sensors

  Serial.println("Scanning for sensors...");

  // Start searching for devices on the 1-Wire bus
  while (oneWire.search(address)) {
    // Display the found address in the format 0x[HEX]
    Serial.print("Found device: ");
    for (int i = 0; i < 8; i++) { 
      Serial.print("0x"); 
      if (address[i] < 16) Serial.print("0"); // Add leading zero for single-digit values
      Serial.print(address[i], HEX);
      if (i < 7) Serial.print(", "); // Add a comma between bytes except the last one
    }
    Serial.println();

    // Check if the device is a DS18B20 sensor (Family code: 0x28)
    if (address[0] == 0x28) {
      Serial.println("Device is a DS18B20 sensor.");
      sensorCount++; // Increment the sensor count
    } else {
      Serial.println("Device is not a DS18B20.");
    }

    // Validate the address using CRC (Cyclic Redundancy Check)
    if (OneWire::crc8(address, 7) != address[7]) {
      Serial.println("CRC is not valid!");
    } else {
      Serial.println("CRC is valid.");
    }
  }

  // After scanning, display the total number of DS18B20 sensors found
  Serial.print("Number of sensors found: ");
  Serial.println(sensorCount);

  // Reset the search state for the next scan
  oneWire.reset_search();
  delay(5000); // Wait 5 seconds before starting a new scan
}
