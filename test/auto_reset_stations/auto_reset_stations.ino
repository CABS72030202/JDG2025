// auto_reset_stations.ino
// Created on: 2024-10-31
// Author: Sebastien Cabana

/* 
 * This code defines the attributes and functions specific to the stations and includes 
 * the algorithm to automatically reset all stations after a predefined delay. The reset 
 * function replaces the manual button described in the wiring diagram of the machine's 
 * construction manual for the 2025 engineering games project. Note that this implementation
 * requires an independent microcontroller to handle the reset process autonomously.
 */

const int buttonPin = 7;      // Pin connected to the button circuit
const unsigned long interval = 240000;  // Interval of 4 minutes in milliseconds
unsigned long previousMillis = 0;       // Stores last time button was "pressed"

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);  // Set pin as input with pull-up resistor (open circuit)
}

void loop() {
  unsigned long currentMillis = millis();

  // Check if 4 minutes have passed
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Simulate button press
    pinMode(buttonPin, OUTPUT);     // Set pin to output to pull it to ground
    digitalWrite(buttonPin, LOW);   // Pull the pin to ground (press button)
    delay(1000);                    // Keep it pressed for 1 second (adjust as needed)

    // Release button
    pinMode(buttonPin, INPUT_PULLUP);  // Set pin back to input (open circuit)
  }
}
